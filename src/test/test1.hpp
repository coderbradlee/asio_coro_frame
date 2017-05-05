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
    // boost::shared_ptr<session> self(shared_from_this());
    auto self(shared_from_this());

    boost::asio::spawn(m_strand,[this,self](boost::asio::yield_context yields)
      {
        try
        {
          std::vector<char> data(128,0);
          // char data[128]={0};
          // char data[128];
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
          cout<<e.what()<< ":"<<__FILE__<<":"<<__LINE__<<endl;
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
            // m_socket.close();
            boost::system::error_code ec;
            m_socket.lowest_layer().shutdown(boost::asio::ip::tcp::socket::shutdown_both, ec);
            m_socket.lowest_layer().close();
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
void server()
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
        if(!ec) 
          {
            boost::make_shared<session>(std::move(so))->go();
          }
      }
    });
  boost::thread_group m_thread_group;
  for (size_t i = 0; i < 8; ++i)
      m_thread_group.create_thread(boost::bind(&boost::asio::io_service::run,&io));
  io.run();
  m_thread_group.join_all();
}
void client()
{
  sleep(2);
  try
  {

    boost::asio::io_service io_service;

    tcp::socket s(io_service);
    tcp::resolver resolver(io_service);
    boost::asio::ip::tcp::resolver::query query("localhost", "8888"); 
    boost::asio::connect(s, resolver.resolve(query));
     
    std::cout << "Enter message: ";
    const int max_length=1024;
    char request[1024];
    std::cin.getline(request, max_length);
    size_t request_length = std::strlen(request);
    boost::asio::write(s, boost::asio::buffer(request, request_length));

    char reply[1024];
    size_t reply_length = boost::asio::read(s,
        boost::asio::buffer(reply, request_length));
    std::cout << "Reply is: ";
    std::cout.write(reply, reply_length);
    std::cout << "\n";
  }
  catch (std::exception& e)
  {
    std::cerr << "Exception: " << e.what() << "\n";
  }
}
struct task:boost::asio::coroutine
{
  task(int i):m_i(i){}
  void operator()()
  {
    reenter (this)
    {
      for(;;)
      {
        printf("%d\n", m_i++);
        yield;
        printf("%d\n", m_i++);
        yield;
        if(m_i>10)
        {
          printf("break\n");
          yield break;
        }
      }
    }
  }
private:
  int m_i;
};
void test_coro()
{
  task t1(0);
  // boost::thread_group m_thread_group;
  // for (size_t i = 0; i < 8; ++i)
  //     m_thread_group.create_thread(t1);
  // m_thread_group.join_all();
  t1();
}
void test()
{
  // boost::thread_group m_thread_group;
  // m_thread_group.create_thread(server);
  // m_thread_group.create_thread(client);
  // m_thread_group.join_all();
  test_coro();
}