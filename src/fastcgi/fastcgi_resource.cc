#include "fastcgi_resource.h"

using namespace muduo::net;

request_parser_base::request_parser_base(const string& query_string,const string& content):m_content(content)
{
  std::vector<std::string> param_pair;
  boost::split(param_pair,query_string , boost::is_any_of("&"));
  for(auto& p:param_pair)
  {
     std::vector<std::string> one_pair;
     boost::split(one_pair,p , boost::is_any_of("="));
     m_query_string.insert(std::make_pair(one_pair[0],one_pair[1]));
  }
}

request_parser_get::request_parser_get(const string& query_string,const string& content):request_parser_base(query_string,content){}


  get_rate::get_rate(const string& src,const string& des,const string& times,boost::shared_ptr<mysql_connect> m_):m_src(src),m_des(des),m_time(times),m_conn(m_){}
  string get_rate::get_rates()
  {
     string id=get_exchange_rate_id(m_src,m_des);
    if(id.length()==0)
      return "0";
    return get_rate_from_myql(id,m_time);
  }
string get_rate::get_exchange_rate_id(const string& source,const string& target)
  {
    try
    {
      string source_currency_id=get_currency_id(source);
      string target_currency_id=get_currency_id(target);
      if(source_currency_id.length()==0||target_currency_id.length()==0)
      {
        return "";
      }
      string get_exchange_rate_ids = "select currency_exchange_rate_id from t_currency_exchange_rate where source_currency_id=\'"+source_currency_id+"\' and target_currency_id=\'"+target_currency_id+"\'";
      m_conn->query(get_exchange_rate_ids);
      while (m_conn->get_res()->next()) 
      {
        //ming_log->get_log_console()->info()<< res->getString(1)<<":" << res->getString("exchange_ratio");
        //ming_log->get_log_console()->info()<< m_res->getString(6)<<":" << m_res->getString("exchange_ratio"); 
        return m_conn->get_res()->getString(1);
      }
      return "";
    }
    catch (sql::SQLException &e) 
    {
      LOG_ERROR<<"# ERR: " << e.what();
      LOG_ERROR<<" (MySQL error code: " << e.getErrorCode();
      LOG_ERROR<<", SQLState: " << e.getSQLState();
      return "";
    }
    catch (exception &e) 
    {
      LOG_ERROR<<"# ERR: " << e.what();
      return "";
    }  
  }
  string get_rate::get_rate_from_myql(const string& exchange_rate_id,const string& which_day)
  {
    try
    {
      string query_sql = "select exchange_ratio from t_currency_daily_exchange_rate where exchange_rate_id=\'"+exchange_rate_id+"\' and (createBy='exchange_gw' or createBy='exchange_gw_rest') and exchange_date='"+which_day+"'";
      m_conn->query(query_sql);
      while (m_conn->get_res()->next()) 
      {
        //ming_log->get_log_console()->info()<< res->getString(1)<<":" << res->getString("exchange_ratio");
        //ming_log->get_log_console()->info()<< m_res->getString(6)<<":" << m_res->getString("exchange_ratio"); 
        return m_conn->get_res()->getString(1);
      }
      return "0";
    }
    catch (sql::SQLException &e) 
    {
      LOG_ERROR<<"# ERR: " << e.what();
      LOG_ERROR<<" (MySQL error code: " << e.getErrorCode();
      LOG_ERROR<<", SQLState: " << e.getSQLState();
      return "0";
    }
    catch (exception &e) 
    {
      LOG_ERROR<<"# ERR: " << e.what();
      return "0";
    }  
  
  }
  string get_rate::get_currency_id(const string& code)
  {
    string query_string="select currency_id from t_currency where code='"+code+"'";
    m_conn->query(query_string);
    while (m_conn->get_res()->next()) 
    {
      //ming_log->get_log_console()->info()<< res->getString(1)<<":" << res->getString("exchange_ratio");
      //ming_log->get_log_console()->info()<< m_res->getString(6)<<":" << m_res->getString("exchange_ratio"); 
      return m_conn->get_res()->getString(1);
    }
    return "0";
  }
  






string request_parser_get::get_result()
{
  try
  {
    //LOG_INFO << m_query_string["source"];
    string db=m_query_string["database"];
    boost::shared_ptr<mysql_info> info(new mysql_info());
    if(db=="os")
    {
      info->ip=get_config->m_mysql_ip;
      info->username=get_config->m_mysql_username;
      info->password=get_config->m_mysql_password;
      info->database=get_config->m_mysql_database;
      info->port=boost::lexical_cast<string>(get_config->m_mysql_port);
    }
    else if(db=="js")
    {
      info->ip=get_config->m_mysql_js_ip;
      info->username=get_config->m_mysql_js_username;
      info->password=get_config->m_mysql_js_password;
      info->database=get_config->m_mysql_js_database;
      info->port=boost::lexical_cast<string>(get_config->m_mysql_js_port);
    }
    else if(db=="eu")
    {
      info->ip=get_config->m_mysql_eu_ip;
      info->username=get_config->m_mysql_eu_username;
      info->password=get_config->m_mysql_eu_password;
      info->database=get_config->m_mysql_eu_database;
      info->port=boost::lexical_cast<string>(get_config->m_mysql_eu_port);
    }
    else
    {
      info->ip=get_config->m_mysql_as_ip;
      info->username=get_config->m_mysql_as_username;
      info->password=get_config->m_mysql_as_password;
      info->database=get_config->m_mysql_as_database;
      info->port=boost::lexical_cast<string>(get_config->m_mysql_as_port);
    }
      
    boost::shared_ptr<mysql_connect> m(new mysql_connect(info));
    get_rate gr(m_query_string["source"],m_query_string["target"],m_query_string["time"],m);
    return gr.get_rates();

  }
  catch (sql::SQLException &e) 
  {
    LOG_ERROR<<"# ERR: " << e.what();
    LOG_ERROR<<" (MySQL error code: " << e.getErrorCode();
    LOG_ERROR<<", SQLState: " << e.getSQLState();
    return "0";
  }
  catch (exception &e) 
  {
    LOG_ERROR<<"# ERR: " << e.what();
    return "0";
  }  
    
}







void parser_param(
  const string& uri,
  const string& request_method,
  const string& query_string,
  const string& content,
  const TcpConnectionPtr& conn)
{
    
    //str.compare(4, 4, "soft")
    //uri.compare(0,get_config->m_url.length(),get_config->m_url)
    size_t len=(get_config->m_url).length();
    string url=get_config->m_url;
    string result="0";
    if(request_method=="GET"&&uri.compare(0,len,url) == 0)
    {
        boost::shared_ptr<request_parser_base> pa(new request_parser_get(query_string,content));
        result=pa->get_result();
    } 
    Buffer response;
    response.append("Context-Type: text/plain\r\n\r\n");
    response.append(result);
    FastCgiCodec::respond(&response);
    conn->send(&response);
}

void onRequest(const TcpConnectionPtr& conn,
               FastCgiCodec::ParamMap& params,
               Buffer* in)
{
  string uri(params["REQUEST_URI"].c_str());
  LOG_INFO << conn->name() << ": " << uri;
  string query_string(params["QUERY_STRING"].c_str());
  LOG_INFO << "query_string: " << query_string;
  // for (FastCgiCodec::ParamMap::const_iterator it = params.begin();
  //      it != params.end(); ++it)
  // {
  //   //LOG_DEBUG << it->first << " = " << it->second;
  // }
  if (in->readableBytes() > 0)
   {
      LOG_DEBUG << "stdin " << in->retrieveAllAsString();
   } 
   string content(in->retrieveAllAsString().c_str());
   string method(params["REQUEST_METHOD"].c_str());
   parser_param(uri,method,query_string,content,conn);
  
}

void onConnection(const TcpConnectionPtr& conn)
{
  if (conn->connected())
  {
    typedef boost::shared_ptr<FastCgiCodec> CodecPtr;
    CodecPtr codec(new FastCgiCodec(onRequest));
    conn->setContext(codec);
    conn->setMessageCallback(
        boost::bind(&FastCgiCodec::onMessage, codec, _1, _2, _3));
    conn->setTcpNoDelay(true);
  }
}

void start_fastcgi()
{
  int port = get_config->m_port;
  int threads = get_config->m_threads;
  LOG_INFO <<port<<":"<<threads;
  InetAddress addr(static_cast<uint16_t>(port));
  LOG_INFO << "FastCGI listens on " << addr.toIpPort()
           << " threads " << threads;
#ifdef MING_DEBUG
  std::cout<< "FastCGI listens on " << addr.toIpPort()
           << " threads " << threads<<std::endl;
#endif
  muduo::net::EventLoop loop;
  boost::shared_ptr<TcpServer> server(new TcpServer(&loop, addr, "FastCGI"));
  server->setConnectionCallback(onConnection);
  server->setThreadNum(threads);
  server->start();
  loop.loop();
}
