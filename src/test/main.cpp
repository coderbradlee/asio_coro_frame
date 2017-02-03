#include <boost/program_options.hpp>
using std::string;
using std::cout;
using std::endl;
int i=1;
int main(int argc, char* argv[])
{
  try
  {
    int i=i;
    printf("%d\n", i);
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