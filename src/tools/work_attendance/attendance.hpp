#pragma once
#include "config.hpp"
#include "log.h"
#include <iomanip>
#include "xlsxio_write.h"
#include "xlsxio_read.h"
struct report_data
 {
 	std::string quotation_id;
 	std::string quotation_detail_id;
 	std::string quotation_no;
 	std::string country_id;
 	std::string owner_sales_sys_account_id;
 	std::string sales_employee_id;
 	
 	void print()
 	{
 		std::cout
 			<<std::endl;
 	}
 }; 

class month_report
{
public:
	month_report();
	void start(const std::string& from,const std::string& to);
private:
	void read_from_excel();
	void data_cleaning();
	void write_to_excel();
private:
	std::vector<boost::shared_ptr<report_data>> m_report_datas;
	std::string from;
	std::string to;
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
public:
	XLSXIOReader(const char* filename)
	{
		if ((xlsxioread = xlsxioread_open(filename)) == NULL) 
		{
		    fprintf(stderr, "Error opening .xlsx file\n");
		}
		sheetdata.firstsheet = NULL;
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
	  printf("\n");
	  return 0;
	}

	//calback function for cell data
	static int sheet_cell_callback (size_t row, size_t col, const char* value, void* callbackdata)
	{
	  if (col > 1)
	    printf("\t");
	  if (value)
	    printf("%s", value);
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