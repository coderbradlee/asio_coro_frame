#include "config.hpp"
#include "fastcgi.h"

int main(int argc, char* argv[])
{
  try
  {
    FCGX_Init();
    // boost::thread_group m_thread_group;
    // for (size_t i = 0; i < 8; ++i)
    //     m_thread_group.create_thread(boost::bind(doit));
    
    // m_thread_group.join_all();
    doit();
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
