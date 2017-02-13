#include "attendance.hpp"
month_report::month_report(const std::string& from,const std::string& to):
m_from(from),
m_to(to),
m_reader(std::make_shared<XLSXIOReader>(m_from.c_str())),
m_writer(std::make_shared<XLSXIOWriter>(m_to.c_str()))
{

}
void month_report::start()
{
	read_from_excel();
	data_cleaning();
	write_to_excel();
}
void month_report::print_get_data()
{
	for(const auto& i:*m_data)
	{
		i->print();
	}
}
void month_report::read_from_excel()
{
	m_reader->list_sheets();
	m_reader->process_data();
	m_data=m_reader->get_data();
	print_get_data(); 
}
void month_report::data_cleaning()
{
	
}
void month_report::write_to_excel()
{

}
void start_report()
{
	std::string from="before.xlsx";
	std::string to="after.xlsx";
	boost::shared_ptr<month_report> report(new month_report(from,to));
	
	report->start();
}