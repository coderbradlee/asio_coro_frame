#include <boost/program_options.hpp>
#include "log.h"
#include "excel_to_mysql.hpp"
using std::string;
using std::cout;
using std::endl;
void start_inspector_thread();

int main(int argc, char* argv[])
{
  try
  {
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