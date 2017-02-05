#include <iostream>
#include <boost/asio/yield.hpp>
using std::string;
using std::cout;
using std::endl;
class test_coro:coroutine
{
public:
  void operator()()
  {
    reenter(this)
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
      t2();
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