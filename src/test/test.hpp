#pragma once 
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
class test_strand:public boost::enable_shared_from_this<test_strand>
{
public:
  test_strand(const string& addr,const string& port):m_io_service(),m_strand(m_io_service),m_acceptor(m_io_service),m_socket(m_io_service),m_data(1024,0)
  {
    boost::asio::ip::tcp::resolver res(m_io_service);
    boost::asio::ip::tcp::endpoint end=*res.resolve({addr,port});
    m_acceptor.open(end.protocol());
    m_acceptor.set_option(boost::asio::ip::tcp::acceptor::reuse_address(true));
    m_acceptor.bind(end);
    m_acceptor.listen();
    do_accept();
    
  }
  void do_accept()
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
  void do_some(boost::asio::yield_context yields)
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
  void do_some_stackless(boost::system::error_code ec = boost::system::error_code(), std::size_t n = 0)
  {
    reenter(m_coro)
    {
      for(;;)
      {
        yield m_socket.async_read_some(boost::asio::buffer(m_data),do_some_stackless);
        yield boost::asio::async_write(m_socket,boost::asio::buffer(m_data,m_data.size()),do_some_stackless);
      }
    }
  }
  ~test_strand()
  {
    cout<<"~test_strand"<<endl;
  }
  void run()
  {
    boost::thread_group threads;
    for (int i = 0; i < 3; ++i)
        threads.create_thread(boost::bind(&boost::asio::io_service::run,&m_io_service));
    m_io_service.run();
    threads.join_all();
  }
  
private:
  coroutine m_coro;
  boost::asio::io_service m_io_service;
  boost::asio::strand m_strand;
  boost::asio::ip::tcp::acceptor m_acceptor;
  boost::asio::ip::tcp::socket m_socket;
  std::vector<char> m_data;
};
void test1()
{
  test_strand t("127.0.0.1","8000");
  t.run();
}

void test()
{
  test1();
}