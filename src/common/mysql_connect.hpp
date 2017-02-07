#ifndef MYSQL_CONNECT_HPP
#define	MYSQL_CONNECT_HPP
#include "mysql_connection.h"
#include "config.hpp"
#include <cppconn/driver.h>
#include <cppconn/exception.h>
#include <cppconn/resultset.h>
#include <cppconn/statement.h>
#include <cppconn/prepared_statement.h>
#include "log.hpp"
struct mysql_info
 {
 	std::string ip;
 	std::string port;
 	std::string username;
 	std::string password;
 	std::string database;
 }; 

class mysql_connect
{
public:
	mysql_connect(boost::shared_ptr<mysql_info> in);
	void query(const std::string& sql);
	boost::shared_ptr<sql::ResultSet> get_res()const;
	void test();
private:
	boost::shared_ptr<mysql_info> m_mysql_info;
	boost::shared_ptr<sql::ResultSet> m_res;
	boost::shared_ptr<sql::Statement> m_stmt;
	boost::shared_ptr<sql::PreparedStatement> m_pstmt;
	boost::shared_ptr<sql::Driver> m_drivers;
	boost::shared_ptr<sql::Connection> m_con;
	
	
	
	sql::Driver* m_driver;
	
};
void test();

#endif