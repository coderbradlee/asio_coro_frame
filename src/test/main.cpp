#include <iostream>
#include <boost/asio.hpp>
#include <boost/asio/yield.hpp>
using std::string;
using std::cout;
using std::endl;
using boost::asio::coroutine;
class test_coro
{
public:
  test_coro():i(0){}
  void operator()()
  {
    reenter(m_coro)
    {
      while(i<10)
      {
         std::cout<<i<<std::endl;
         ++i;
         yield;
         std::cout<<i<<std::endl;
         ++i;
      }
    }
  }
private:
  coroutine m_coro;
  int i;
};
int main(int argc, char* argv[])
{
  try
  {
    test_coro t1,t2;
    for(;;)
    {
      t1();
      // t2();
    }
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