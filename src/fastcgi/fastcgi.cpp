#include "fastcgi.h"
#include <iostream>
#include <boost/asio.hpp>
#include <boost/thread/thread.hpp>
#include <boost/bind.hpp>
#include <boost/date_time/posix_time/posix_time.hpp>
#include <boost/make_shared.hpp>
#include <boost/asio/spawn.hpp>
#include <boost/asio/yield.hpp>
#include <boost/asio/coroutine.hpp>
using std::string;
using std::cout;
using std::endl;
using boost::asio::coroutine;

cgi_strand::cgi_strand(const string& addr,const string& port):m_io_service(),m_strand(m_io_service),m_acceptor(m_io_service),m_socket(m_io_service),m_data(1024,0)
{
	boost::asio::ip::tcp::resolver res(m_io_service);
	boost::asio::ip::tcp::endpoint end=*res.resolve({addr,port});
	m_acceptor.open(end.protocol());
	m_acceptor.set_option(boost::asio::ip::tcp::acceptor::reuse_address(true));
	m_acceptor.bind(end);
	m_acceptor.listen();
	do_accept();

}
void cgi_strand::do_accept()
{
	m_acceptor.async_accept(m_socket,[this](boost::system::error_code ec)
  {
    if(!m_acceptor.is_open())
    {
      cout<<"acceptor fail"<<endl;
      return;
    }
    if(!ec)
    {
      // boost::asio::spawn(m_strand,boost::bind(&test_strand::do_some,this,_1));
      do_some_stackless(ec);
    }
  });

}
void cgi_strand::do_some(boost::asio::yield_context yields)
{
	try
	{
	  std::vector<char> data(128,0);
	  for(;;)
	  {
	    size_t len=m_socket.async_read_some(boost::asio::buffer(data),yields);
	    boost::asio::async_write(m_socket,boost::asio::buffer(data,len),yields);
	  }
	}
	catch(std::exception& e)
	{
	  cout<<e.what()<<endl;
	}
}
void cgi_strand::do_some_stackless(boost::system::error_code ec, std::size_t n)
{
	reenter(m_coro)
	{
	  for(;;)
	  {
	    yield m_socket.async_read_some(boost::asio::buffer(m_data),boost::bind(&cgi_strand::do_some_stackless,this,_1,_2));
	    yield boost::asio::async_write(m_socket,boost::asio::buffer(m_data,n),boost::bind(&cgi_strand::do_some_stackless,this,_1,_2));
	  }
	}
}
cgi_strand::~cgi_strand()
{
	cout<<"~cgi_strand"<<endl;
}
void cgi_strand::run()
{
	boost::thread_group threads;
	for (int i = 0; i < 3; ++i)
	    threads.create_thread(boost::bind(&boost::asio::io_service::run,&m_io_service));
	m_io_service.run();
	threads.join_all();
}
