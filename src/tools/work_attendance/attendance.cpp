#include "attendance.hpp"

void month_report::start(cosnt std::string& from,cosnt std::string& to)
{
	read_from_excel();
	data_cleaning();
	write_to_excel();
}

void month_report::read_from_excel()
{
	XLSXIOReader reads(from);
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
	std::string from="before.xsl";
	std::string to="after.xsl";
	report->start(from,to);
}