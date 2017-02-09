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
#include <boost/enable_shared_from_this.hpp>  
using std::string;
using std::cout;
using std::endl;
using boost::asio::coroutine;
using boost::asio::ip::tcp;
class session:public boost::enable_shared_from_this<session>
{
public:
  explicit session(tcp::socket so):
  m_socket(std::move(so)),
  m_timer(m_socket.get_io_service()),
  m_strand(m_socket.get_io_service())
  {

  }
  void go()
  {
    cout<<"is this happend?"<< ":"<<__FILE__<<":"<<__LINE__<<endl;
    boost::shared_ptr<session> self(boost::shared_prt<session>(shared_from_this()));

    boost::asio::spawn(m_strand,[this,self](boost::asio::yield_context yields)
      {
        try
        {
          // std::vector<char> data(128,0);
          // char data[128]={0};
          char data[128];
          cout<<"is this happend?"<< ":"<<__FILE__<<":"<<__LINE__<<endl;
          for(;;)
          {
            m_timer.expires_from_now(boost::posix_time::seconds(10));
            size_t n=m_socket.async_read_some(boost::asio::buffer(data),yields);
            boost::asio::async_write(m_socket,boost::asio::buffer(data,n),yields);
          }
        }
        catch(std::exception& e)
        {
          cout<<"is this happend?"<< ":"<<__FILE__<<":"<<__LINE__<<endl;
          m_socket.close();
          m_timer.cancel();
        }
      });
    boost::asio::spawn(m_strand,[this,self](boost::asio::yield_context yields)
      {
        while(m_socket.is_open())
        {
          cout<<"is this happend?"<< ":"<<__FILE__<<":"<<__LINE__<<endl;
          boost::system::error_code ignored_ec;
          m_timer.async_wait(yields[ignored_ec]);
          if(m_timer.expires_from_now()<=boost::posix_time::seconds(0))
          {
            m_socket.close();
            // boost::system::error_code ec;
            // m_socket.lowest_layer().shutdown(boost::asio::ip::tcp::socket::shutdown_both, ec);
            // m_socket.lowest_layer().close();
          }
        }
      });
  }
  ~session(){;}
private:
  tcp::socket m_socket;
  boost::asio::deadline_timer m_timer;
  boost::asio::io_service::strand m_strand;
};
void test1()
{
  boost::asio::io_service io;
  boost::asio::spawn(io,[&io](boost::asio::yield_context yields)
    {
      boost::asio::ip::tcp::acceptor acc(io,boost::asio::ip::tcp::endpoint(boost::asio::ip::tcp::v4(),8888));
      for(;;)
      {
        boost::system::error_code ec;
        boost::asio::ip::tcp::socket so(io);
        acc.async_accept(so,yields[ec]);
        if(!ec) std::make_shared<session>(std::move(so))->go();
      }
    });
  io.run();
}

void test()
{
  test1();
}