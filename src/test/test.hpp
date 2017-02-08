#pragma once 
#include <iostream>
#include <boost/asio.hpp>
#include <boost/thread/thread.hpp>
#include <boost/bind.hpp>
#include <boost/date_time/posix_time/posix_time.hpp>
#include <boost/make_shared.hpp>
using std::string;
using std::cout;
using std::endl;
using boost::asio::coroutine;
class test_strand
{
public:
  test_strand(boost::asio::io_service& m_io_service):m_strand(m_io_service),m_timer1(m_io_service),m_timer2(m_io_service),m_count(0)
  {
    m_timer1.async_wait(m_strand.wrap(boost::bind(&test_strand::print1,this)));
    m_timer2.async_wait(m_strand.wrap(boost::bind(&test_strand::print2,this)));
  }
  ~test_strand()
  {
    cout<<"last:"<<m_count<<endl;
  }
  
  void print1()
  {
    if(m_count<10)
    {
      cout<<"timer 1:"<<m_count<<endl;
      ++m_count;
      m_timer1.expires_at(m_timer1.expires_at()+boost::posix_time::seconds(1));
      m_timer1.async_wait(m_strand.wrap(boost::bind(&test_strand::print1,this)));
    }
  }
  void print2()
  {
    if(m_count<10)
    {
      cout<<"timer 2:"<<m_count<<endl;
      ++m_count;
      m_timer2.expires_at(m_timer2.expires_at()+boost::posix_time::seconds(1));
      m_timer2.async_wait(m_strand.wrap(boost::bind(&test_strand::print2,this)));
    }
  }
private:
  boost::asio::io_service::strand m_strand;
  boost::asio::deadline_timer m_timer1;
  boost::asio::deadline_timer m_timer2;
  int m_count;
};
void test()
{
  boost::asio::io_service io;
  // auto p=boost::make_shared<test_strand>(io);
  test_strand t(io);
  boost::thread_group threads;
  for (int i = 0; i < 3; ++i)
      threads.create_thread(boost::bind(&boost::asio::io_service::run,&io));
  io.run();//main thread
  threads.join_all();
}