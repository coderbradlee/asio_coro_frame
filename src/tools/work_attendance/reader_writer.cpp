
#include "reader_writer.hpp"

XLSXIOReader::XLSXIOReader(const char* filename)
{
	if ((xlsxioread = xlsxioread_open(filename)) == NULL) 
	{
	    fprintf(stderr, "Error opening .xlsx file\n");
	}
	sheetdata.firstsheet = NULL;
}
std::shared_ptr<std::vector<std::shared_ptr<report_data>>> XLSXIOReader::get_data()
{
	return m_data;
}
void XLSXIOReader::list_sheets()
{
	// xlsxioread_list_sheets(xlsxioread, xlsx_list_sheets_callback, &sheetdata);
	// xlsxioread_list_sheets(xlsxioread, boost::bind(&XLSXIOReader::xlsx_list_sheets_callback,this,_1,_2), &sheetdata);
	xlsxioread_list_sheets(xlsxioread, [&](const char* name, void* callbackdata)->int
		{
			xlsx_list_sheets_data* data = (xlsx_list_sheets_data*)callbackdata;
		  	if (!data->firstsheet)
		    	data->firstsheet = strdup(name);
		  	printf(" - %s\n", name);
		  	return 0;
		}, &sheetdata);
}
int XLSXIOReader::cell_callback(long unsigned int row, long unsigned int col, const char* value, void* callbackdata)
{
	if(m_row_data!=nullptr)
  	{
	  	switch(col)
	  	{
	  		case 1:
	  			std::cout<<value<<":"<<__FILE__<<":"<<__LINE__<<std::endl;
	  			m_row_data->group=value;
	  			break;
	  		case 2:
	  			std::cout<<value<<":"<<__FILE__<<":"<<__LINE__<<std::endl;
	  			m_row_data->number=value;
	  			break;
	  		case 3:
	  			std::cout<<value<<":"<<__FILE__<<":"<<__LINE__<<std::endl;
	  			m_row_data->id=value;
	  			break;
	  		case 4:
	  			std::cout<<value<<":"<<__FILE__<<":"<<__LINE__<<std::endl;
	  			m_row_data->name=value;
	  			break;
	  		case 5:
	  			std::cout<<value<<":"<<__FILE__<<":"<<__LINE__<<std::endl;
	  			m_row_data->dates=value;
	  			break;
	  		case 6:
	  			std::cout<<value<<":"<<__FILE__<<":"<<__LINE__<<std::endl;
	  			m_row_data->on_duty=value;
	  			break;
	  		case 7:
	  			std::cout<<value<<":"<<__FILE__<<":"<<__LINE__<<std::endl;
	  			m_row_data->on_duty_desc=value;
	  			break;
	  		case 8:
	  			std::cout<<value<<":"<<__FILE__<<":"<<__LINE__<<std::endl;
	  			m_row_data->off_duty=value;
	  			break;
	  		case 9:
	  			m_row_data->off_duty_desc=value;
	  			break;
	  		case 10:
	  			m_row_data->come_late=value;
	  			break;
	  		case 11:
	  			m_row_data->leave_early=value;
	  			break;
	  		case 12:
	  			m_row_data->extra_work=value;
	  			break;
	  		case 13:
	  			m_row_data->away_from_work=value;
	  			break;
	  		case 14:
	  			m_row_data->without_clock_in=value;
	  			break;
	  		default:
	  			break;
	  	}
  }
  return 0;
}
int XLSXIOReader::row_callback(long unsigned int row, long unsigned int maxcol, void* callbackdata)
{
	if(m_row_data!=nullptr)
	{
		m_data->push_back(m_row_data);
		m_row_data->print();
		m_row_data=std::shared_ptr<report_data>(new report_data());
	}
	else
	{
		m_row_data=std::shared_ptr<report_data>(new report_data());
		std::cout<<m_row_data<<":"<<__FILE__<<":"<<__LINE__<<std::endl;
	}
	return 0;
}
void XLSXIOReader::process_data()
{
	//read data
	xlsxioread_process(xlsxioread, sheetdata.firstsheet, XLSXIOREAD_SKIP_EMPTY_ROWS, cell_callback,row_callback, NULL);
	
}

XLSXIOReader::~XLSXIOReader()
{
	free(sheetdata.firstsheet);
  	xlsxioread_close(xlsxioread);
}
  

std::shared_ptr<report_data> XLSXIOReader::m_row_data=std::shared_ptr<report_data>(new report_data());
std::shared_ptr<std::vector<std::shared_ptr<report_data>>> XLSXIOReader::m_data=std::shared_ptr<std::vector<std::shared_ptr<report_data>>>(new std::vector<std::shared_ptr<report_data>>());
