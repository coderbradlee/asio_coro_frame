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
	// print_get_data(); 
}
void month_report::data_cleaning()
{
		
}
void month_report::write_to_excel()
{
    m_writer->SetRowHeight(1);
  	std::vector<std::string> v
  	{	
		"部门班组",	"工号",	"姓名",	"日期",	"出勤时间"	
  	};
  
  	for(const auto& i:v)
  	{
    	m_writer->AddColumn(i.c_str());
  	}
  	m_writer->NextRow();
  
  	for(size_t i=1;i<(*m_data).size();++i)
	{
		// std::cout<<x->group<<std::endl;//
		auto x=(*m_data)[i];
		*m_writer<<x->group;
	 	*m_writer<<x->id;
	 	*m_writer<<x->name;
	 	std::string temp_date=(x->dates).substr(0,(x->dates).size()-1);
	 	temp_date+=" 00:00:00";
	 	std::cout<<temp_date<<std::endl;
	 	time_t tt=StringToDatetime(temp_date.c_str());
	 	xlsxiowrite_add_cell_datetime(handle, tt);//
	 	// *m_writer<<tt;
	 	// *m_writer<<temp_date;
		m_writer->NextRow();
	} 
	m_writer->NextRow();
}
void start_report()
{
	std::string from="before.xlsx";
	std::string to="after.xlsx";
	boost::shared_ptr<month_report> report(new month_report(from,to));
	
	report->start();
}
time_t StringToDatetime(const char *str)  
{  
    tm tm_;  
    int year, month, day, hour, minute,second;  
    sscanf(str,"%d-%d-%d %d:%d:%d", &year, &month, &day, &hour, &minute, &second);  
    tm_.tm_year  = year-1900;  
    tm_.tm_mon   = month-1;  
    tm_.tm_mday  = day;  
    tm_.tm_hour  = hour;  
    tm_.tm_min   = minute;  
    tm_.tm_sec   = second;  
    tm_.tm_isdst = 0;  
  
    time_t t_ = mktime(&tm_); //已经减了8个时区  
    return t_; //秒时间  
} 