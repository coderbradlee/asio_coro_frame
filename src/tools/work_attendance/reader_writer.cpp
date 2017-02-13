
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
  		std::string temp;
  		if(value!=NULL)
  		{
  			temp=value;
  			// std::cout<<temp<<":"<<__FILE__<<":"<<__LINE__<<std::endl;
  		}
  		else
  		{
  			temp="";
			// std::cout<<temp<<":"<<__FILE__<<":"<<__LINE__<<std::endl;
  		}
  		
	  	switch(col)
	  	{
	  		case 1:
	  			m_row_data->group=temp;
	  			break;
	  		case 2:
	  			m_row_data->number=temp;
	  			break;
	  		case 3:
	  			m_row_data->id=temp;
	  			break;
	  		case 4:
	  			m_row_data->name=temp;
	  			break;
	  		case 5:
	  			m_row_data->dates=temp;
	  			break;
	  		case 6:
	  			m_row_data->on_duty=temp;
	  			break;
	  		case 7:
	  			m_row_data->on_duty_desc=temp;
	  			break;
	  		case 8:
	  			m_row_data->off_duty=temp;
	  			break;
	  		case 9:
	  			m_row_data->off_duty_desc=temp;
	  			break;
	  		case 10:
	  			m_row_data->come_late=temp;
	  			break;
	  		case 11:
	  			m_row_data->leave_early=temp;
	  			break;
	  		case 12:
	  			m_row_data->extra_work=temp;
	  			break;
	  		case 13:
	  			m_row_data->away_from_work=temp;
	  			break;
	  		case 14:
	  			m_row_data->without_clock_in=temp;
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
		// m_row_data->print();
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
  
std::shared_ptr<std::vector<std::shared_ptr<report_data>>> XLSXIOReader::get_data()
{
	return m_data;
}
std::shared_ptr<report_data> XLSXIOReader::m_row_data=std::shared_ptr<report_data>(new report_data());
std::shared_ptr<std::vector<std::shared_ptr<report_data>>> XLSXIOReader::m_data=std::shared_ptr<std::vector<std::shared_ptr<report_data>>>(new std::vector<std::shared_ptr<report_data>>());
