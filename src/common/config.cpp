
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
	m_port= boost::lexical_cast<unsigned short>(m_pt.get<std::string>("webserver.port"));
	m_threads= boost::lexical_cast<unsigned short>(m_pt.get<std::string>("webserver.threads"));
	m_url= m_pt.get<std::string>("webserver.url");
	m_redis_host= m_pt.get<std::string>("redis.host");
	m_redis_port= boost::lexical_cast<unsigned short>(m_pt.get<std::string>("redis.port"));
	m_log_name= m_pt.get<std::string>("log.name");
	m_log_level= m_pt.get<std::string>("log.level");
}
	
boost::shared_ptr<iconfig> iconfig::m_ps = nullptr;
boost::mutex iconfig::m_mu;
boost::shared_ptr<iconfig> get_config= iconfig::get_instance("config.ini");
