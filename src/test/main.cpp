#include <iostream>
#include <boost/asio.hpp>
// #include <boost/asio/yield.hpp>
#include <boost/enable_shared_from_this.hpp>  
#include <boost/shared_ptr.hpp> 
#include "test1.hpp"
using std::string;
using std::cout;
using std::endl;
using boost::asio::coroutine;

int main(int argc, char* argv[])
{
  try
  { 
    test();
  }
  catch (std::exception& e)
  {
    //cout << diagnostic_information(e) << endl;
    cout << e.what() << ":"<<__FILE__<<":"<<__LINE__<<endl;
  }
  catch (...)
  {
    cout << "unknown error"<< ":"<<__FILE__<<":"<<__LINE__<<endl;
  }
  return 0;
}