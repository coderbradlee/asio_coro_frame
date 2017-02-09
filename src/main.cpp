#include "config.hpp"
#include "server_http.hpp"
int main() 
{
	try
	{
	    HttpServer server(get_config->m_port,get_config->m_threads);
	    serverRedisResource(server);
		thread server_thread([&server](){
	        //Start server
	        server.start();
	    });

	    boost::timer::cpu_timer pass;
		pass.start();
		
	    // for(int i=0;i<900000;++i)
	    // {
	    // 	BOOST_LOG_SEV(slg, notification)<<i;
	    // }
	    std::cout << "now time elapsed:" << pass.format(6) << std::endl;
	    // initsink->flush();
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