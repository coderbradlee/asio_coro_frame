#include "config.hpp"
#include "serverResource.hpp"
boost::shared_ptr< file_sink > initsink=initlog();//init log;
src::severity_logger< severity_level > slg;
int main() {
	try
	{
	    HttpServer server(get_config->m_port,get_config->m_threads);
	    serverRedisResource(server);

	    boost::timer::cpu_timer pass;
		pass.start();
		
	    for(int i=0;i<100000;++i)
	    {
	    	BOOST_LOG_SEV(slg, notification)<<i;
	    }
	    std::cout << "now time elapsed:" << pass.format(6) << std::endl;
	    initsink->flush();

	    thread server_thread([&server]()
	    {
	        server.start();
	    });
	    server_thread.join();
	}
	catch(exception& e) 
	{
        cout<< e.what();
    }
	catch(...) 
	{
         
	}
    return 0;
}