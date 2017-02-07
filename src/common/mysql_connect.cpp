#include "mysql_connect.hpp"


mysql_connect::mysql_connect(boost::shared_ptr<mysql_info> in):m_mysql_info(in),m_res(nullptr),m_stmt(nullptr),m_pstmt(nullptr)
{
	m_driver = get_driver_instance();
	m_con = boost::shared_ptr<sql::Connection>(m_driver->connect("tcp://"+m_mysql_info->ip+":"+m_mysql_info->port, m_mysql_info->username, m_mysql_info->password));
	
	m_con->setSchema(m_mysql_info->database);

}
void mysql_connect::query(const string& query)
{
	try 
	{
	  m_pstmt = boost::shared_ptr<sql::PreparedStatement>(m_con->prepareStatement(query));
	  m_res = boost::shared_ptr<sql::ResultSet>(m_pstmt->executeQuery());

	} 
	catch (sql::SQLException &e) 
	{
	  //ming_log->get_log_console()->info()<< "# ERR: " << e.what();
	  //ming_log->get_log_console()->info()<< " (MySQL error code: " << e.getErrorCode();
	  //ming_log->get_log_console()->info()<< ", SQLState: " << e.getSQLState();
	  LOG_ERROR<<"# ERR: " << e.what();
	  LOG_ERROR<<" (MySQL error code: " << e.getErrorCode();
	  LOG_ERROR<<", SQLState: " << e.getSQLState();

	}
}
boost::shared_ptr<sql::ResultSet> mysql_connect::get_res()const
{
	return m_res;
}
void mysql_connect::test()
{
	try 
	{
	  
	  /* '?' is the supported placeholder syntax */
	  m_pstmt = boost::shared_ptr<sql::PreparedStatement>(m_con->prepareStatement("insert into t_currency_daily_exchange_rate values(rand_string(20),'TFTBLZNSNBNAZAZGC2RW','2016','03','11',?,'2016-03-11','2016-03-11 14:51:05','','','','','',0,1)"));
	  for (int i = 1; i <= 1; i++) 
	  {
	    //pstmt->setInt(1, i);
	    m_pstmt->setDouble(1, 0.772558);
	    m_pstmt->executeUpdate();
	  }

	  string query="select * from t_currency_daily_exchange_rate where exchange_rate_id='TFTBLZNSNBNAZAZGC2RW' and exchange_date='2016-03-11'";
	  m_pstmt = boost::shared_ptr<sql::PreparedStatement>(m_con->prepareStatement(query));
	  m_res = boost::shared_ptr<sql::ResultSet>(m_pstmt->executeQuery());

	  while (m_res->next()) 
	  {
			//ming_log->get_log_console()->info()<< res->getString(1)<<":" << res->getString("exchange_ratio");
		  //ming_log->get_log_console()->info()<< m_res->getString(6)<<":" << m_res->getString("exchange_ratio");	
	  }
	} 
	catch (sql::SQLException &e) 
	{
	  //ming_log->get_log_console()->info()<< "# ERR: " << e.what();
	  //ming_log->get_log_console()->info()<< " (MySQL error code: " << e.getErrorCode();
	  //ming_log->get_log_console()->info()<< ", SQLState: " << e.getSQLState();
	}

}
// void test()
// {
// 	boost::shared_ptr<mysql_info> info(new mysql_info());
// 	info->ip=get_config->m_mysql_ip;
// 	info->username=get_config->m_mysql_username;
// 	info->password=get_config->m_mysql_password;
// 	info->database=get_config->m_mysql_database;

// 	info->port=boost::lexical_cast<string>(get_config->m_mysql_port);
// 	mysql_connect m(info);
// 	m.test();
// }