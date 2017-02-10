#include "config.hpp"
#include "fastcgi.h"

int main(int argc, char* argv[])
{
  try
  {
    cgi_strand t("127.0.0.1","8000");
    t.run();
  }
  catch(std::exception& e) 
  {
        std::cout<< e.what()<<std::endl;
  }
  catch(...) 
  {
         std::cout<< "unknown error"<<std::endl;
  }
    return 0;
}
