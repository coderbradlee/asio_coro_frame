#ifndef ATTENDANCE_HPP
#define	ATTENDANCE_HPP

#include "reader_writer.hpp"


class month_report
{
public:
	month_report(const std::string& from,const std::string& to);
	void start();
private:
	void read_from_excel();
	void data_cleaning();
	void write_to_excel();
	void print_get_data();
private:
	std::string m_from;
	std::string m_to;
	std::shared_ptr<XLSXIOReader> m_reader;
	std::shared_ptr<XLSXIOWriter> m_writer;
	std::shared_ptr<std::vector<std::shared_ptr<report_data>>> m_data;
};


void start_report();

#endif