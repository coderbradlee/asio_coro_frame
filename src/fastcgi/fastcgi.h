#ifndef FASTCGI_HPP
#define FASTCGI_HPP

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
class cgi_strand:public boost::enable_shared_from_this<cgi_strand>
{
public:
  cgi_strand(const string& addr,const string& port);
  void do_accept();
  void do_some(boost::asio::yield_context yields);
  void do_some_stackless(boost::system::error_code ec = boost::system::error_code(), std::size_t n = 0);
  ~cgi_strand();
  void run();
  
private:
  coroutine m_coro;
  boost::asio::io_service m_io_service;
  boost::asio::strand m_strand;
  boost::asio::ip::tcp::acceptor m_acceptor;
  boost::asio::ip::tcp::socket m_socket;
  std::vector<char> m_data;
};
#endif  /* SERVER_HTTP_HPP */