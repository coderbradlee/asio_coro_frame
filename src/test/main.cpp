#include <iostream>
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
    int a={6,7,8,9,10};
    int *pa=a;
    *(pa++)+=123;
    printf("%d,%d\n",*pa,*(++pa));
    //sleep(10);
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