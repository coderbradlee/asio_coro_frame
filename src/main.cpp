#include "config.hpp"
#include "server.hpp"
int main() 
{
	try
	{
	    // HttpServer server(get_config->m_port,get_config->m_threads);
	    // serverRedisResource(server);
		http::server::server s("0.0.0.0",get_config->m_port,"web",7);
    	s.run();


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