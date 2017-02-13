#ifndef ATTENDANCE_HPP
#define	ATTENDANCE_HPP

#include "reader_writer.hpp"
struct report_data
 {
 	std::string group;
 	std::string number;
 	std::string id;
 	std::string name;
 	std::string dates;
 	std::string on_duty;
 	std::string on_duty_desc;
 	std::string off_duty;
 	std::string off_duty_desc;
 	std::string come_late;
 	std::string leave_early;
 	std::string extra_work;
 	std::string away_from_work;
 	std::string without_clock_in;
  	
 	void print()
 	{
 		std::cout<<
 		group<<","<<
	 	number<<","<<
	 	id<<","<<
	 	name<<","<<
	 	dates<<","<<
	 	on_duty<<","<<
	 	on_duty_desc<<","<<
	 	off_duty<<","<<
	 	off_duty_desc<<","<<
	 	come_late<<","<<
	 	leave_early<<","<<
	 	extra_work<<","<<
	 	away_from_work<<","<<
	 	without_clock_in
 		<<std::endl;
 	}
 }; 

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