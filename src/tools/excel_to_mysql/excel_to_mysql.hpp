#ifndef ALL_MONTH_REPORT_HPP
#define	ALL_MONTH_REPORT_HPP
#include "mysql_connection.h"
#include "config.hpp"
#include <cppconn/driver.h>
#include <cppconn/exception.h>
#include <cppconn/resultset.h>
#include <cppconn/statement.h>
#include <cppconn/prepared_statement.h>
#include "log.h"
#include <iomanip>
#include "xlsxio_write.h"
struct report_data
 {
 	std::string quotation_id;
 	std::string quotation_detail_id;
 	std::string quotation_no;
 	std::string country_id;
 	std::string owner_sales_sys_account_id;
 	std::string sales_employee_id;
 	std::string sales_company_name;
 	std::string account_name;
 	std::string approval_status;
 	std::string sales_full_name;
 	std::string receiving_countries;
 	std::string customer_countries;
 	std::string country_region;
	std::string product_name_id;//export as product_name_id
	std::string product_name;
	double product_qty_pc;
	double product_qty_w;
	std::string price_condition;
	std::string currency;
	double unit_price;
	std::string guided_currency;
	std::string price_total_currency;
	double price_total;
	std::string pi_no;
	std::string buyers_po;
	std::string contract_no;
 	std::string payment_term_desc;
 	std::string creat_at;
 	double guidance_price;
 	double total_guidance_price;
 	std::string import_area;
 	void print()
 	{
 		std::cout<<std::setprecision(8)<<quotation_id
 			<<","<<quotation_detail_id
 			<<","<<quotation_no
		 	<<","<<account_name
		 	<<","<<approval_status
		 	<<","<<sales_full_name
		 	<<","<<receiving_countries
		 	<<","<<customer_countries
		 	<<","<<country_region
			<<","<<product_name_id//export as product_name_id
			<<","<<product_name
			<<","<<product_qty_pc
			<<","<<product_qty_w
			<<","<<price_condition
			<<","<<currency
			<<","<<unit_price
			<<","<<guided_currency
			<<","<<price_total_currency
			<<","<<price_total
			<<","<<pi_no
			<<","<<buyers_po
			<<","<<contract_no
		 	<<","<<payment_term_desc
		 	<<","<<creat_at
 			<<std::endl;
 	}
 	std::string csv_line()
 	{
 		std::ostringstream stream;
			stream<<std::setprecision(8)<<quotation_no
		 	<<","<<account_name
		 	<<","<<sales_company_name
		 	<<","<<"customer_name"
		 	<<","<<customer_countries
		 	<<","<<receiving_countries
		 	<<","<<"import area"
		 	<<","<<approval_status
		 	<<","<<"trade term"
			<<","<<product_name_id//export as product_name_id
			<<","<<product_name
		 	<<","<<product_qty_pc
			<<","<<product_qty_w
			<<","<<currency
			<<","<<unit_price
			<<","<<"zhidaojia"
			<<","<<price_total
			<<","<<"zhidaozhenggejiage"
			<<","<<payment_term_desc
		 	<<","<<creat_at
		 	

		 	<<","<<sales_full_name
			<<","<<price_condition
			<<","<<currency
			<<","<<price_total_currency
			<<","<<pi_no
			<<","<<buyers_po
			<<","<<contract_no
			<<"\r\n";
 		return stream.str();
 	}
 }; 
struct mysql_info_
 {
 	std::string ip;
 	std::string port;
 	std::string username;
 	std::string password;
 	std::string database;
 }; 
class month_report
{
public:
	month_report(boost::shared_ptr<mysql_info_> in);
	void start();
private:
	void query(const std::string& sql);
	boost::shared_ptr<sql::ResultSet> get_res()const;
	void insert_data();
	void deal_with_sales_info();
	void deal_with_customer_info();
	void deal_with_currency_info();
	void deal_with_payment_method_info();
	void deal_with_product_info();
	void deal_with_trade_term_info();
	void deal_with_sales_country();
	void deal_with_approved_status();
	void deal_with_pi();
	void write_to_csv();
	void write_to_excel();
private:
	std::vector<boost::shared_ptr<report_data>> m_report_datas;
	boost::shared_ptr<sql::ResultSet> m_res;
	boost::shared_ptr<sql::Statement> m_stmt;
	boost::shared_ptr<sql::PreparedStatement> m_pstmt;
	boost::shared_ptr<sql::Driver> m_drivers;
	boost::shared_ptr<sql::Connection> m_con;

	sql::Driver* m_driver;
};
class XLSXIOWriter
{
 private:
  xlsxiowriter handle;
 public:

  /*! \brief XLSXIOWriter constructor, creates and opens .xlsx file
   * \param  filename      path of .xlsx file to open
   * \param  sheetname     name of worksheet
   * \param  detectionrows number of rows to buffer in memory, zero for none, defaults to 5
   * \sa     xlsxiowrite_open()
   */
  XLSXIOWriter (const char* filename, const char* sheetname = NULL, size_t detectionrows = 5);

  /*! \brief XLSXIOWriter destructor, closes .xlsx file
   * \sa     xlsxiowrite_close()
   */
  ~XLSXIOWriter ();

  /*! \brief specify the row height to use for the current and next rows
   * \param  height        row height (in text lines), zero for unspecified
   * Must be called before the first call to any Add method of the current row
   * \sa     xlsxiowrite_set_row_height()
   */
  void SetRowHeight (size_t height = 0);

  /*! \brief add a column cell
   * \param  name          column name
   * \param  width         column width (in characters)
   * Only one row of column names is supported or none.
   * Call for each column, and finish column row by calling NextRow().
   * Must be called before any NextRow() or the AddCell methods.
   * \sa     NextRow()
   */
  void AddColumn (const char* name, int width = 0);

  /*! \brief add a cell with string data
   * \param  value         string value
   * \sa     NextRow()
   */
  void AddCellString (const char* value);

  /*! \brief add a cell with integer data
   * \param  value         integer value
   * \sa     NextRow()
   */
  void AddCellInt (long long value);

  /*! \brief add a cell with floating point data
   * \param  value         floating point value
   * \sa     NextRow()
   */
  void AddCellFloat (double value);

  /*! \brief add a cell with date and time data
   * \param  value         date and time value
   * \sa     NextRow()
   */
  void AddCellDateTime (time_t value);

  /*! \brief insertion operators
   * \sa     AddCellString()
   * \name   operator<<
   * \{
   */
  XLSXIOWriter& operator << (const char* value);
  XLSXIOWriter& operator << (const std::string& value);
  XLSXIOWriter& operator << (int64_t value);
  XLSXIOWriter& operator << (double value);
  //XLSXIOWriter& operator << (time_t value);
  /*! @} */

  /*! \brief mark the end of a row (next cell will start on a new row)
   * \sa     xlsxiowrite_next_row()
   * \sa     AddCellString()
   */
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

/*
inline XLSXIOWriter& XLSXIOWriter::operator << (time_t value)
{
  AddCellDateTime(value);
  return *this;
}
*/

inline void XLSXIOWriter::NextRow ()
{
  xlsxiowrite_next_row(handle);
}
class write_csv
{
public:
	write_csv(std::string filename)
	{
		setFilename(filename);
		m_fp = std::ofstream(filename);
	}
	void close()
	{
		m_fp.close();
	}
	~write_csv()
	{
		m_fp.close();
	}
	void addData(std::string data)
	{
		m_fp.write(data.c_str(), data.length());
	}
private:
	void setFilename(std::string filename)
	{
		m_filename = filename;
	}
	
private:
		std::string m_filename;
		std::ofstream m_fp;
};
void start_report();

#endif