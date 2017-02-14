
#include "config.hpp"
boost::shared_ptr<iconfig> iconfig::get_instance(const std::string& filename)
{
	
	if (m_ps == nullptr)
	{
		boost::mutex::scoped_lock t(m_mu);
		if(m_ps == nullptr)
			m_ps = boost::shared_ptr<iconfig>(new iconfig(filename));
	}
	
	return m_ps;
}

iconfig::iconfig(const std::string& filename)
{			
	boost::property_tree::ini_parser::read_ini(filename, m_pt);
	m_port= m_pt.get<std::string>("webserver.port");
	m_threads= boost::lexical_cast<unsigned short>(m_pt.get<std::string>("webserver.threads"));
	m_url= m_pt.get<std::string>("webserver.url");
	m_redis_host= m_pt.get<std::string>("redis.host");
	m_redis_port= boost::lexical_cast<unsigned short>(m_pt.get<std::string>("redis.port"));
	m_log_name= m_pt.get<std::string>("log.name");
	m_log_level= m_pt.get<std::string>("log.level");
	std::string flush="false";
	if(flush==m_pt.get<std::string>("log.auto_flush"))
		m_log_auto_flush= false;
	else
		m_log_auto_flush= true;
	//////////////////////////////////////////////////////////////
	m_mysql_ip = m_pt.get<std::string>("mysql.ip");
	m_mysql_port = boost::lexical_cast<unsigned short>(m_pt.get<std::string>("mysql.port"));
	m_mysql_username = m_pt.get<std::string>("mysql.username");
	m_mysql_password = m_pt.get<std::string>("mysql.password");
	m_mysql_database = m_pt.get<std::string>("mysql.database");
	m_mysql_eu_ip = m_pt.get<std::string>("mysql_eu.ip");
	m_mysql_eu_port = boost::lexical_cast<unsigned short>(m_pt.get<std::string>("mysql_eu.port"));
	m_mysql_eu_username = m_pt.get<std::string>("mysql_eu.username");
	m_mysql_eu_password = m_pt.get<std::string>("mysql_eu.password");
	m_mysql_eu_database = m_pt.get<std::string>("mysql_eu.database");

	m_mysql_js_ip = m_pt.get<std::string>("mysql_js.ip");
	m_mysql_js_port = boost::lexical_cast<unsigned short>(m_pt.get<std::string>("mysql_js.port"));
	m_mysql_js_username = m_pt.get<std::string>("mysql_js.username");
	m_mysql_js_password = m_pt.get<std::string>("mysql_js.password");
	m_mysql_js_database = m_pt.get<std::string>("mysql_js.database");

	m_mysql_as_ip = m_pt.get<std::string>("mysql_as.ip");
	m_mysql_as_port = boost::lexical_cast<unsigned short>(m_pt.get<std::string>("mysql_as.port"));
	m_mysql_as_username = m_pt.get<std::string>("mysql_as.username");
	m_mysql_as_password = m_pt.get<std::string>("mysql_as.password");
	m_mysql_as_database = m_pt.get<std::string>("mysql_as.database");
	////////////////////////////////////////////////////////////////
	m_attendance_from=m_pt.get<std::string>("attendance.from");
	m_attendance_to=m_pt.get<std::string>("attendance.to");
}
	
boost::shared_ptr<iconfig> iconfig::m_ps = nullptr;
boost::mutex iconfig::m_mu;
boost::shared_ptr<iconfig> get_config= iconfig::get_instance("config.ini");
