#include "config.hpp"
#include "fastcgi.hpp"
#include "pdf.hpp"
int main(int argc, char* argv[])
{
  try
  {
    redis_api r;
    pdf_api p;
    boost::thread_group m_thread_group;
    for (size_t i = 0; i < 1; ++i)
    {
        m_thread_group.create_thread(boost::bind(&redis_api::run,&r));
        m_thread_group.create_thread(boost::bind(&pdf_api::run,&p));
    }
    
    m_thread_group.join_all();
    

    
    // doit();
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
