#include "attendance.hpp"

void month_report::start(const std::string& from,const std::string& to)
{
	m_from=from;
	m_to=to;
	read_from_excel();
	data_cleaning();
	write_to_excel();
}

void month_report::read_from_excel()
{
	XLSXIOReader reads(m_from.c_str());
	reads.list_sheets();
	reads.process_data();
}
void month_report::data_cleaning()
{
	
}
void month_report::write_to_excel()
{

}
void start_report()
{
	boost::shared_ptr<month_report> report(new month_report());
	std::string from="before.xlsx";
	std::string to="after.xlsx";
	report->start(from,to);
}