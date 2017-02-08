#ifndef CONFIG_HPP
#define	CONFIG_HPP
#include <string.h>
#include <cstdarg>
#include <iostream>
#include <map>
#include "include.hpp"
class iconfig:boost::noncopyable
{
	public:
		static boost::shared_ptr<iconfig> get_instance(const std::string& filename);
	private:
		iconfig(const std::string& filename);
		
	public:
		boost::property_tree::ptree m_pt;
		
		// unsigned short m_port;
		std::string m_port;
		unsigned short m_threads;
		std::string m_url;

		std::string m_redis_host;
		unsigned short m_redis_port;
		
		std::string m_log_name;
		std::string m_log_level;
		bool m_log_auto_flush;
		static boost::mutex m_mu;	
		static boost::shared_ptr<iconfig> m_ps;
		///////////////////////////////////////////
		std::string m_mysql_ip;
		unsigned short m_mysql_port;
		std::string m_mysql_username;
		std::string m_mysql_password;
		std::string m_mysql_database;
		///////////////////
		std::string m_mysql_eu_ip;
		unsigned short m_mysql_eu_port;
		std::string m_mysql_eu_username;
		std::string m_mysql_eu_password;
		std::string m_mysql_eu_database;
		//////////////////
		std::string m_mysql_js_ip;
		unsigned short m_mysql_js_port;
		std::string m_mysql_js_username;
		std::string m_mysql_js_password;
		std::string m_mysql_js_database;
		//////////////////
		std::string m_mysql_as_ip;
		unsigned short m_mysql_as_port;
		std::string m_mysql_as_username;
		std::string m_mysql_as_password;
		std::string m_mysql_as_database;

};
extern boost::shared_ptr<iconfig> get_config;
#endif

