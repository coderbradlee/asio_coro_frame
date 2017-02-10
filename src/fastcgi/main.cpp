#include "fastcgi_resource.h"

int main(int argc, char* argv[])
{
  unsigned short port=boost::lexical_cast<unsigned short>(get_config->m_port);
      HttpServer server(port,get_config->m_threads);
    serverRedisResource(server);
    thread server_thread([&server]()
    {
          server.start();
    });

    boost::timer::cpu_timer pass;
    pass.start();
    
    std::cout << "now time elapsed:" << pass.format(6) << std::endl;
    
    server_thread.join();
}
