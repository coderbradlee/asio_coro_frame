#ifndef READER_WRITER_HPP
#define	READER_WRITER_HPP
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
	static std::shared_ptr<report_data> m_row_data;
	static std::shared_ptr<std::vector<std::shared_ptr<report_data>>> m_data;

public:
	XLSXIOReader(const char* filename);
	std::shared_ptr<std::vector<std::shared_ptr<report_data>>> get_data();
    void list_sheets();
	static int cell_callback(long unsigned int row, long unsigned int col, const char* value, void* callbackdata);
	static int row_callback(long unsigned int row, long unsigned int maxcol, void* callbackdata);
    void process_data();
	~XLSXIOReader();
	  
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
    XLSXIOWriter& operator << (time_t value);
    void NextRow ();
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
inline XLSXIOWriter& XLSXIOWriter::operator << (time_t value)
{
  AddCellDateTime(value);
  return *this;
}
inline void XLSXIOWriter::NextRow ()
{
  xlsxiowrite_next_row(handle);
}


#endif