#include "config.hpp"
#include "log.h"
#include "serverResource.hpp"

int main() {
	try
	{
	    HttpServer server(get_config->m_port,get_config->m_threads);
	    serverRedisResource(server);
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