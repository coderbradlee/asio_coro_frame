#include "config.hpp"
#include "serverResource.hpp"

int main() {
	try
	{
	    HttpServer server(get_config->m_port,get_config->m_threads);
	    serverRedisResource(server,get_config->m_redis_host,get_config->m_redis_port,"",get_config->m_url);
	    thread server_thread([&server]()
	    {
	        server.start();
	    });

		server.initsink->flush();
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