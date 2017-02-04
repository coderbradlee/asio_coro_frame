#ifndef SERVER_RESOURCE_HPP
#define	SERVER_RESOURCE_HPP
#define BOOST_SPIRIT_THREADSAFE

#include "renesolalog.h"
#include "redispp.h"
#include <boost/date_time/posix_time/posix_time.hpp>
#include <iostream>
#include <boost/bind.hpp>
#include <list>
#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/ini_parser.hpp>
#include <boost/lexical_cast.hpp>
//#include <queue>
//#include <thread>
#include <condition_variable>
#include <assert.h>

#include "hirediscommand.h"
using namespace RedisCluster;
using namespace redispp;
using namespace boost::posix_time;

#include "server_http.hpp"
#include "client_http.hpp"

#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/json_parser.hpp>
#include <boost/algorithm/string/split.hpp>
#include <boost/algorithm/string/classification.hpp>
//Added for the default_resource example
#include<fstream>
using namespace std;
//Added for the json:
using namespace boost::property_tree;

typedef SimpleWeb::Server<SimpleWeb::HTTP> HttpServer;
typedef SimpleWeb::Client<SimpleWeb::HTTP> HttpClient;

template<typename redisConnection>
class ThreadedPool
{
    static const int poolSize_ = 10;
    typedef Cluster<redisConnection, ThreadedPool> RCluster;
    // We will save our pool in std::queue here
    typedef std::queue<redisConnection*> ConQueue;
    // Define pair with condition variable, so we can notify threads, when new connection is released from some thread
    typedef std::pair<std::condition_variable, ConQueue> ConPool;
    // Container for saving connections by their slots, just as
    typedef std::map <typename RCluster::SlotRange, ConPool*, typename RCluster::SlotComparator> ClusterNodes;
    // Container for saving connections by host and port (for redirecting)
    typedef std::map <typename RCluster::Host, ConPool*> RedirectConnections;
    // rename cluster types
    typedef typename RCluster::SlotConnection SlotConnection;
    typedef typename RCluster::HostConnection HostConnection;
    
public:
    
    ThreadedPool( typename RCluster::pt2RedisConnectFunc conn,
                 typename RCluster::pt2RedisFreeFunc disconn,
                 void* userData ) :
    data_( userData ),
    connect_(conn),
    disconnect_(disconn)
    {
    }
    
    ~ThreadedPool()
    {
        disconnect();
    }
    
    // helper function for creating connections in loop
    inline void fillPool( ConPool &pool, const char* host, int port )
    {
        for( int i = 0; i < poolSize_; ++i )
        {
            redisConnection *conn = connect_( host,
                                            port,
                                            data_ );
            
            if( conn == NULL || conn->err )
            {
                throw ConnectionFailedException();
            }
            pool.second.push( conn );
        }
    }
    
    // helper for fetching connection from pool
    inline redisConnection* pullConnection( std::unique_lock<std::mutex> &locker, ConPool &pool )
    {
        redisConnection *con = NULL;
        // here we wait for other threads for release their connections if the queue is empty
        while (pool.second.empty())
        {
            // if queue is empty here current thread is waiting for somethread to release one
            pool.first.wait(locker);
        }
        // get a connection from queue
        con = pool.second.front();
        pool.second.pop();
        
        return con;
    }
    // helper for releasing connection and placing it in pool
    inline void pushConnection( std::unique_lock<std::mutex> &locker, ConPool &pool, redisConnection* con )
    {
        pool.second.push(con);
        locker.unlock();
        // notify other threads for their wake up in case of they are waiting
        // about empty connection queue
        pool.first.notify_one();
    }
    
    // function inserts new connection by range of slots during cluster initialization
    inline void insert( typename RCluster::SlotRange slots, const char* host, int port )
    {
        std::unique_lock<std::mutex> locker(conLock_);
        
        ConPool* &pool = nodes_[slots];
        pool = new ConPool();
        fillPool(*pool, host, port);
    }
    
    // function inserts or returning existing one connection used for redirecting (ASKING or MOVED)
    inline HostConnection insert( string host, string port )
    {
        std::unique_lock<std::mutex> locker(conLock_);
        string key( host + ":" + port );
        HostConnection con = { key, NULL };
        try
        {
            con.second = pullConnection( locker, *connections_.at( key ) );
        }
        catch( const std::out_of_range &oor )
        {
        }
        // create new connection in case if we didn't redirecting to this
        // node before
        if( con.second == NULL )
        {
            ConPool* &pool = connections_[key];
            pool = new ConPool();
            fillPool(*pool, host.c_str(), std::stoi(port));
            con.second = pullConnection( locker, *pool );
        }
        
        return con;
    }
    
    
    inline SlotConnection getConnection( typename RCluster::SlotIndex index )
    {
        std::unique_lock<std::mutex> locker(conLock_);
        
        typedef typename ClusterNodes::iterator iterator;
        iterator found = DefaultContainer<redisConnection>::template searchBySlots(index, nodes_);
        
        return { found->first, pullConnection( locker, *found->second ) };
    }
    
    // this function is invoked when library whants to place initial connection
    // back to the storage and the connections is taken by slot range from storage
    inline void releaseConnection( SlotConnection conn )
    {
        std::unique_lock<std::mutex> locker(conLock_);
        pushConnection( locker, *nodes_[conn.first], conn.second );
    }
    // same function for redirection connections
    inline void releaseConnection( HostConnection conn )
    {
        std::unique_lock<std::mutex> locker(conLock_);
        pushConnection( locker, *connections_[conn.first], conn.second );
    }
    
    // disconnect both thread pools
    inline void disconnect()
    {
        disconnect<ClusterNodes>( nodes_ );
        disconnect<RedirectConnections>( connections_ );
    }
    
    template <typename T>
    inline void disconnect(T &cons)
    {
        std::unique_lock<std::mutex> locker(conLock_);
        if( disconnect_ != NULL )
        {
            typename T::iterator it(cons.begin()), end(cons.end());
            while ( it != end )
            {
                for( int i = 0; i < poolSize_; ++i )
                {
                    // pullConnection will wait for all connection
                    // to be released
                    disconnect_( pullConnection( locker, *it->second) );
                }
                if( it->second != NULL )
                {
                    delete it->second;
                    it->second = NULL;
                }
                ++it;
            }
        }
        cons.clear();
    }
    
    void* data_;
private:
    typename RCluster::pt2RedisConnectFunc connect_;
    typename RCluster::pt2RedisFreeFunc disconnect_;
    RedirectConnections connections_;
    ClusterNodes nodes_;
    std::mutex conLock_;
};

typedef Cluster<redisContext, ThreadedPool<redisContext> > ThreadPoolCluster;

//set global variable value ThreadPoolCluster::ptr_t cluster_p;，set value through timer
ThreadPoolCluster::ptr_t cluster_p;
///////////////redis cluster thread pool//////////////////////

void deal_with_flow_number(HttpServer::Response& response, std::shared_ptr<HttpServer::Request> request)
{
     try 
        {
            string temp_flowno="/flowNo/";
            string left_path=request->path.substr(temp_flowno.length(), request->path.length());
            cout<<left_path<<endl;
            std::vector<std::string> one_pair;
            boost::split(one_pair,left_path , boost::is_any_of("/"));


            string company=one_pair[0];
            string type=one_pair[1];
            string id_name="{"+company+"_"+type+"_"+"flow_number}:id";
            string incr_command="incr "+id_name;
            string get_command="get "+id_name;
            cout<<id_name<<endl;
            cout<<incr_command<<endl;
            cout<<get_command<<endl;
            
            redisReply * incr=static_cast<redisReply*>( HiredisCommand<ThreadPoolCluster>::Command( cluster_p, id_name.c_str(), incr_command.c_str()));
            freeReplyObject(incr);
            cout<<__FILE__<<""<<__LINE__<<endl;
            redisReply * reply=static_cast<redisReply*>( HiredisCommand<ThreadPoolCluster>::Command( cluster_p, id_name.c_str(), get_command.c_str()));
            string value="";
            //cout<<__LINE__<<endl;
            if(reply->str!=nullptr)
            {
                //cout<<reply->type<<endl;
              value+=reply->str;
              //retJson.put<std::string>("flow_number",value);
            }
            freeReplyObject(reply);
            cout<<value<<":"<<__FILE__<<""<<__LINE__<<endl;
            ptime now = second_clock::local_time();  
            string now_str  =  to_iso_extended_string(now.date()) + " " + to_simple_string(now.time_of_day());  
            string temp="{\"flowNo\":\""+value+"\",\"replyTime\" : \""+now_str+"\"}";

            cout<<temp<<":"<<__FILE__<<""<<__LINE__<<endl;
            // std::stringstream ss;
            // write_json(ss, retJson);
            // //在这里判断里面的children及childrens的值，如果为空，设置为空数组,用replace
            // string temp=ss.str();
            response <<"HTTP/1.1 200 OK\r\nContent-Length: " << temp.length() << "\r\n\r\n" <<temp;
        }
        catch(json_parser_error& e) 
        {
            string temp="json error";
            response << "HTTP/1.1 400 Bad Request\r\nContent-Length: " << temp.length()<< "\r\n\r\n" << temp;
        }
        catch(exception& e) {
            response << "HTTP/1.1 400 Bad Request\r\nContent-Length: " << strlen(e.what()) << "\r\n\r\n" << e.what();
        }
        catch(...) {
            response << "HTTP/1.1 400 Bad Request\r\nContent-Length: " << strlen("unknown error") << "\r\n\r\n" << "unknown error";
        }
}
void defaultindex(HttpServer& server)
{
	try
	{
        server.default_resource["GET"]=[](HttpServer::Response& response, std::shared_ptr<HttpServer::Request> request) {
		string filename="web";
        
		string path=request->path;
        //cout<<path<<endl;
        string temp="/flowNo/";
        if(path.compare(0,temp.length(),temp) == 0)
        {
            deal_with_flow_number(response,request);
            return;
        }
		//Replace all ".." with "." (so we can't leave the web-directory)
		size_t pos;
		while((pos=path.find(".."))!=string::npos) 
        {
			path.erase(pos, 1);
		}
        
		filename+=path;
		ifstream ifs;
		//A simple platform-independent file-or-directory check do not exist, but this works in most of the cases:
		if(filename.find('.')==string::npos) {
			if(filename[filename.length()-1]!='/')
				filename+='/';
			filename+="index.html";
		}
		ifs.open(filename, ifstream::in);
        
		if(ifs) 
        {
			ifs.seekg(0, ios::end);
			size_t length=ifs.tellg();
            
			ifs.seekg(0, ios::beg);

			response << "HTTP/1.1 200 OK\r\nContent-Length: " << length << "\r\n\r\n";
            
			//read and send 128 KB at a time if file-size>buffer_size
			size_t buffer_size=131072;
			if(length>buffer_size) 
            {
				vector<char> buffer(buffer_size);
				size_t read_length;
				while((read_length=ifs.read(&buffer[0], buffer_size).gcount())>0) 
                {
					response.stream.write(&buffer[0], read_length);
					response << HttpServer::flush;
				}
			}
			else
				response << ifs.rdbuf();

			ifs.close();
		}
		else 
        {
			string content="Could not open file "+filename;
			response << "HTTP/1.1 400 Bad Request\r\nContent-Length: " << content.length() << "\r\n\r\n" << content;
		}
	};

	}
	catch(exception& e) 
	{
          BOOST_LOG(test_lg::get())<<__LINE__<<": "<<e.what();
	}
}

void serverRedisResource(HttpServer& server)
{
	try
	{
		//init redis connection pool

		 cluster_p = HiredisCommand<ThreadPoolCluster>::createCluster( get_config->m_redis_host.c_str(),get_config->m_redis_port);
		defaultindex(server);
	}
	catch(exception& e) 
	{
          BOOST_LOG(test_lg::get())<<__LINE__<<": "<<e.what();
	}
	catch(...) 
	{
         
	}
}




std::string&   replace_all(std::string&   str,const   std::string&   old_value,const   std::string&   new_value)     
{     
    while(true)   {     
        std::string::size_type   pos(0);     
        if(   (pos=str.find(old_value))!=std::string::npos   )     
            str.replace(pos,old_value.length(),new_value);     
        else   break;     
    }     
    return   str;     
}     
std::string&   replace_all_distinct(std::string&   str,const   std::string&   old_value,const   std::string&   new_value)     
{     
    for(std::string::size_type   pos(0);   pos!=std::string::npos;   pos+=new_value.length())   {     
        if(   (pos=str.find(old_value,pos))!=std::string::npos   )     
            str.replace(pos,old_value.length(),new_value);     
        else   break;     
    }     
    return   str;     
} 
#endif	