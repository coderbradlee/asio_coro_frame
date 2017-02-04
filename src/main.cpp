#include "config.hpp"
#include "log.h"
#include "serverResource.hpp"
int main() {
	try
	{
	    HttpServer server(get_config->m_port,get_config->m_threads);
	    serverRedisResource(server);

	    boost::timer::timer tm;
		
	    for(int i=0;i<100000;++i)
	    {
	    	BOOST_LOG(slg::get())<<i;
	    }
	    double duration = tm.elapsed();
		cout << duration << endl;
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