#ifndef FASTCGI_RESOURCE_H
#define FASTCGI_RESOURCE_H

#include "fastcgi.h"
#include "../sudoku/sudoku.h"

#include <muduo/base/Logging.h>
#include <muduo/net/EventLoop.h>
#include <muduo/net/TcpServer.h>

#include <boost/bind.hpp>
#include "../log.hpp"
#include "../mysql_connect.hpp"
using namespace muduo::net;

const muduo::string kPath = "/sudoku/";

void onRequest(const TcpConnectionPtr& conn,
               FastCgiCodec::ParamMap& params,
               Buffer* in);

void onConnection(const TcpConnectionPtr& conn);
void parser_param(
	const muduo::string& uri,
  const muduo::string& query_string,
  const muduo::string& content,
  const TcpConnectionPtr& conn);
void start_fastcgi();

class request_parser_base:boost::noncopyable
{
public:
	request_parser_base(const muduo::string& query_string,const muduo::string& content);
	virtual muduo::string get_result()=0;
	virtual ~request_parser_base(){}
protected:
	std::map<muduo::string,muduo::string> m_query_string;
	muduo::string m_content;
};
class request_parser_get:public request_parser_base
{
public:
	request_parser_get(const muduo::string& query_string,const muduo::string& content);
	muduo::string get_result();
};
class get_rate
{
public:
	get_rate(const muduo::string& src,const muduo::string& des,const muduo::string& time,boost::shared_ptr<mysql_connect> m_);
	muduo::string get_rates();
private:
	muduo::string get_currency_id(const muduo::string& code);
	muduo::string get_rate_from_myql(const muduo::string& exchange_rate_id,const muduo::string& which_day);
	muduo::string get_exchange_rate_id(const muduo::string& source,const muduo::string& target);
	
	muduo::string m_src;
	muduo::string m_des;
	muduo::string m_time;
	boost::shared_ptr<mysql_connect> m_conn;
};
#endif  