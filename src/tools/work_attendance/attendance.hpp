#pragma once
#include "config.hpp"
#include "log.h"
#include <iomanip>
#include "xlsxio_write.h"
#include "xlsxio_read.h"
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
	void start(const std::string& from,const std::string& to);
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
class XLSXIOWriter
{
private:
 	xlsxiowriter handle;
public:

	XLSXIOWriter (const char* filename, const char* sheetname = NULL, size_t detectionrows = 5);
    ~XLSXIOWriter ();
    void SetRowHeight (size_t height = 0);

    void AddColumn (const char* name, int width = 0);

    void AddCellString (const char* value);

    void AddCellInt (long long value);

    void AddCellFloat (double value);

    void AddCellDateTime (time_t value);

    XLSXIOWriter& operator << (const char* value);
    XLSXIOWriter& operator << (const std::string& value);
    XLSXIOWriter& operator << (int64_t value);
    XLSXIOWriter& operator << (double value);
  
    void NextRow ();
};

class XLSXIOReader
{
private:
	struct xlsx_list_sheets_data 
	{
  		char* firstsheet;
	};
	xlsxioreader xlsxioread;
	xlsx_list_sheets_data sheetdata;
	std::shared_ptr<report_data> m_row_data;
	std::shared_ptr<std::vector<std::shared_ptr<report_data>>> m_data;

public:
	XLSXIOReader(const char* filename):m_row_data(std::make_shared<report_data>)
	{
		if ((xlsxioread = xlsxioread_open(filename)) == NULL) 
		{
		    fprintf(stderr, "Error opening .xlsx file\n");
		}
		sheetdata.firstsheet = NULL;
	}
	std::shared_ptr<std::vector<std::shared_ptr<report_data>>> get_data()
	{
		return m_data;
	}
    void list_sheets()
    {
    	xlsxioread_list_sheets(xlsxioread, xlsx_list_sheets_callback, &sheetdata);
    }
	
    void process_data()
    {
    	xlsxioread_process(xlsxioread, sheetdata.firstsheet, XLSXIOREAD_SKIP_EMPTY_ROWS, sheet_cell_callback, sheet_row_callback, NULL);
    }
	//calback function for listing sheets
	static int xlsx_list_sheets_callback (const char* name, void* callbackdata)
	{
	  xlsx_list_sheets_data* data = (xlsx_list_sheets_data*)callbackdata;
	  if (!data->firstsheet)
	    data->firstsheet = strdup(name);
	  printf(" - %s\n", name);
	  return 0;
	}

	//calback function for end of row
	static int sheet_row_callback (size_t row, size_t maxcol, void* callbackdata)
	{
		if(m_row_data!=nullptr)
		{
			m_data->push_back(m_row_data);
			m_row_data=std::make_shared<report_data>();
		}
		
	  // printf("\n");
	  return 0;
	}

	//calback function for cell data
	static int sheet_cell_callback (size_t row, size_t col, const char* value, void* callbackdata)
	{
	  // if (col > 1)
	  //   printf("\t");
	  // if (value)
	  //   printf("%d:%s",col, value);
	  if(m_row_data!=nullptr)
	  {
	  	switch(col)
	  	{
	  		case 1:
	  			m_row_data->group=value;
	  			break;
	  		case 2:
	  			m_row_data->number=value;
	  			break;
	  		case 3:
	  			m_row_data->id=value;
	  			break;
	  		case 4:
	  			m_row_data->name=value;
	  			break;
	  		case 5:
	  			m_row_data->dates=value;
	  			break;
	  		case 6:
	  			m_row_data->on_duty=value;
	  			break;
	  		case 7:
	  			m_row_data->on_duty_desc=value;
	  			break;
	  		case 8:
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
	~XLSXIOReader()
	{
		free(sheetdata.firstsheet);
	  	xlsxioread_close(xlsxioread);
	}
	  
};


inline XLSXIOWriter::XLSXIOWriter (const char* filename, const char* sheetname, size_t detectionrows)
{
  unlink(filename);
  handle = xlsxiowrite_open(filename, sheetname);
  xlsxiowrite_set_detection_rows(handle, detectionrows);
}

inline XLSXIOWriter::~XLSXIOWriter ()
{
  xlsxiowrite_close(handle);
}

inline void XLSXIOWriter::SetRowHeight (size_t height)
{
  xlsxiowrite_set_row_height(handle, height);
}

inline void XLSXIOWriter::AddColumn (const char* name, int width)
{
  xlsxiowrite_add_column(handle, name, width);
}

inline void XLSXIOWriter::AddCellString (const char* value)
{
  xlsxiowrite_add_cell_string(handle, value);
}

inline void XLSXIOWriter::AddCellInt (long long value)
{
  xlsxiowrite_add_cell_int(handle, value);
}

inline void XLSXIOWriter::AddCellFloat (double value)
{
  xlsxiowrite_add_cell_float(handle, value);
}

inline void XLSXIOWriter::AddCellDateTime (time_t value)
{
  xlsxiowrite_add_cell_datetime(handle, value);
}

inline XLSXIOWriter& XLSXIOWriter::operator << (const char* value)
{
  AddCellString(value); return *this;
}

inline XLSXIOWriter& XLSXIOWriter::operator << (const std::string& value)
{
  AddCellString(value.c_str());
  return *this;
}

inline XLSXIOWriter& XLSXIOWriter::operator << (int64_t value)
{
  AddCellInt(value);
  return *this;
}

inline XLSXIOWriter& XLSXIOWriter::operator << (double value)
{
  AddCellFloat(value);
  return *this;
}

inline void XLSXIOWriter::NextRow ()
{
  xlsxiowrite_next_row(handle);
}

void start_report();