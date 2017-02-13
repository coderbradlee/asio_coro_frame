#include <boost/program_options.hpp>
#include "log.h"
#include "attendance.hpp"
using std::string;
using std::cout;
using std::endl;

int main(int argc, char* argv[])
{
  try
  {
    const char* v1="test1";
    std::string vs1=v1;
    const char* v2="test2";
    vs1=v2;
    std::cout<<vs1<<std::endl;
    start_report();
    sleep(10);
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