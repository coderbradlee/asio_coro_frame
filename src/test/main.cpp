#include <iostream>
#include <boost/asio.hpp>
// #include <boost/asio/yield.hpp>
#include <boost/enable_shared_from_this.hpp>  
#include <boost/shared_ptr.hpp> 
#include "test.hpp"
using std::string;
using std::cout;
using std::endl;
using boost::asio::coroutine;
class server:public boost::enable_shared_from_this<server>
{
public:
  server(const string& addr,const string& port):m_io_service(),m_acceptor(m_io_service),m_socket(m_io_service)
  {
    boost::asio::ip::tcp::resolver res(m_io_service);
    boost::asio::ip::tcp::endpoint end=*res.resolve({addr,port});
    m_acceptor.open(end.protocol());
    m_acceptor.set_option(boost::asio::ip::tcp::acceptor::reuse_address(true));
    m_acceptor.bind(end);
    m_acceptor.listen();
    do_accept();
  }
  void run()
  {
    m_io_service.run();
  }
  void do_accept()
  {
     // reenter(m_coro)
     // {
     //    yield m_acceptor.async_accept(m_socket,
     //        [this](boost::system::error_code ec)
     //        {
     //          if(!m_acceptor.is_open())
     //          {
     //            return;
     //          }
     //          if(!ec)
     //          {
     //            cout<<"acceptor"<<endl;
     //          }
     //          do_accept();
     //        });     
     // }
  }
  
private:
  coroutine m_coro;
  boost::asio::io_service m_io_service;
  boost::asio::ip::tcp::acceptor m_acceptor;
  boost::asio::ip::tcp::socket m_socket;
};
class client
{
public:
  client(){;}
  ~client(){}
private:
  coroutine m_coro;
};
int main(int argc, char* argv[])
{
  try
  { 
    test();
  }
  catch (std::exception& e)
  {
    //cout << diagnostic_information(e) << endl;
    cout << e.what() << endl;
  }
  catch (...)
  {
    cout << "unknown error" << endl;
  }
  return 0;
}