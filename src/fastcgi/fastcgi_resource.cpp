#include "fastcgi_resource.h"

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
        BOOST_LOG_SEV(slg, notification)<<__LINE__<<": "<<e.what();
  }
}

void serverRedisResource(HttpServer& server)
{
  try
  {
     cluster_p = HiredisCommand<ThreadPoolCluster>::createCluster( get_config->m_redis_host.c_str(),get_config->m_redis_port);
    defaultindex(server);
  }
  catch(exception& e) 
  {
    BOOST_LOG_SEV(slg, notification)<<__LINE__<<": "<<e.what();
  }
  catch(...) 
  {
         
  }
}