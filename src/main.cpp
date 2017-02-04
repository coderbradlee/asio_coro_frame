
#include "serverResource.hpp"

int main() {
	try
	{
	//read config.ini
	boost::property_tree::ptree pt;
	boost::property_tree::ini_parser::read_ini("config.ini", pt);
	unsigned short port=boost::lexical_cast<unsigned short>(pt.get<std::string>("webserver.port"));
	size_t threads=boost::lexical_cast<size_t>(pt.get<std::string>("webserver.threads"));
	url=pt.get<std::string>("webserver.url");
	redisHost=pt.get<std::string>("redis.host");
	redisPort=pt.get<std::string>("redis.port");
	flow_number_param1=pt.get<std::string>("flow_number.param1_name");
	flow_number_param2=pt.get<std::string>("flow_number.param2_name");
	//redisPassword=pt.get<std::string>("redis.password");
	redisPassword="";
	/*Connection conn(redisHost, redisPort, redisPassword);
	connRedis=&conn;*/
    //HTTP-server at port 8080 using 4 threads
    HttpServer server(port,threads);
    //serverResource(server);
    serverRedisResource(server,redisHost,redisPort,redisPassword,url);
    thread server_thread([&server](){
        //Start server
        server.start();
    });


	
	server.initsink->flush();
    server_thread.join();
	delete cluster_p;
	}
	catch(exception& e) {
            cout<< e.what();
        }
	catch(...) 
	{
         
	}
    return 0;
}