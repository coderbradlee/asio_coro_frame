#include "excel_to_mysql.hpp"

void start_report()
{
	// boost::shared_ptr<mysql_info_> info(new mysql_info_());
	// info->ip=get_config->m_mysql_js_ip;
	// info->username=get_config->m_mysql_js_username;
	// info->password=get_config->m_mysql_js_password;
	// info->database=get_config->m_mysql_js_database;

	// info->port=boost::lexical_cast<std::string>(get_config->m_mysql_js_port);
	// boost::shared_ptr<month_report> report(new month_report(info));
	// report->start();
	XLSXIOReader test("mexic_price.xlsx");
	test.list_sheets();
	test.process_data();
}