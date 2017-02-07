#include "all_month_report.hpp"
month_report::month_report(boost::shared_ptr<mysql_info_> in)
{
	m_driver = get_driver_instance();
	m_con = boost::shared_ptr<sql::Connection>(m_driver->connect("tcp://"+in->ip+":"+in->port, in->username, in->password));
	
	m_con->setSchema(in->database);
}
void month_report::deal_with_pi()
{
	try
	{
	for(auto& i:m_report_datas)
	{
		std::string query_string="select pi_no,buyer_po,contract_no from t_proforma_invoice where quotation_id='"+i->quotation_id+"'";
		
		query(query_string);
		m_res->next();
		if(m_res->rowsCount()<1||m_res->isNull("pi_no")||m_res->getString(1)=="") {}
		else
		{	
			i->pi_no=m_res->getString("pi_no");
		}
		if(!m_res->isNull("buyer_po"))
		{
			i->buyers_po=m_res->getString("buyer_po");
		}
		if(!m_res->isNull("contract_no"))
		{
			i->contract_no=m_res->getString("contract_no");
		}
	};
	//std::for_each(m_report_datas.begin(),m_report_datas.end(),[](boost::shared_ptr<report_data>& x){x->print();});
	}
	catch (sql::SQLException &e) 
	{
	  BOOST_LOG_SEV(slg, error)<<"# ERR: " << e.what();
	  BOOST_LOG_SEV(slg, error)<<" (MySQL error code: " << e.getErrorCode();
	  BOOST_LOG_SEV(slg, error)<<", SQLState: " << e.getSQLState();

	}
	catch (std::exception& e)
  	{
    	BOOST_LOG_SEV(slg, error)<<"# ERR: " << e.what();
  	}
  	catch (...)
  	{
    	BOOST_LOG_SEV(slg, error)<<"unknown error ";
  	}
}
void month_report::deal_with_sales_country()
{
	try
	{
	for(auto& i:m_report_datas)
	{
		std::string query_string="select delivery_country_id from t_quotation where quotation_id='"+i->quotation_id+"'";
		
		query(query_string);
		m_res->next();
		if(m_res->rowsCount()<1||m_res->isNull("delivery_country_id")||m_res->getString(1)=="") {}
		else
		{
			query_string="select full_name from t_country where country_id='"+m_res->getString(1)+"'";
	
			query(query_string);
			m_res->next();
			if(m_res->rowsCount()<1||m_res->isNull("full_name")||m_res->getString(1)=="") 
			{
				std::cout<<query_string<<":"<<__FILE__<<":"<<__LINE__<<std::endl;
			}
		else
		{
			i->receiving_countries=m_res->getString("full_name");
		}
		}
		
		
		////////////////////////////////////////////////
		query_string="select sales_id from t_quotation where quotation_id='"+i->quotation_id+"'";
		query(query_string);
		m_res->next();
		if(m_res->rowsCount()<1||m_res->isNull("sales_id")||m_res->getString(1)=="") 
		{
			std::cout<<query_string<<":"<<__FILE__<<":"<<__LINE__<<std::endl;
		}
		else
		{
			i->owner_sales_sys_account_id=m_res->getString("sales_id");
		}
		query_string="select employee_no from t_system_account where system_account_id='"+i->owner_sales_sys_account_id+"'";
		//std::cout<<query_string<<":"<<__FILE__<<":"<<__LINE__<<std::endl;
		query(query_string);
		m_res->next();
		if(m_res->rowsCount()<1||m_res->isNull("employee_no")||m_res->getString(1)=="") 
		{
			std::cout<<query_string<<":"<<__FILE__<<":"<<__LINE__<<std::endl;
		}
		else
		{
			i->sales_employee_id=m_res->getString("employee_no");
		}
		
		query_string="\
		select master_file_obj_id \
		from t_wf_role_resolve \
		where master_file_type='COMPANY' \
		and employee_id='"+i->sales_employee_id+"'";
		//std::cout<<query_string<<":"<<__FILE__<<":"<<__LINE__<<std::endl;
		query(query_string);
		m_res->next();
		if(m_res->rowsCount()<1||m_res->isNull("master_file_obj_id")||m_res->getString(1)=="") 
		{
			std::cout<<query_string<<":"<<__FILE__<<":"<<__LINE__<<std::endl;
		}
		else
		{
			query_string="select short_name from t_company where company_id='"+m_res->getString(1)+"'";
			//std::cout<<query_string<<":"<<__FILE__<<":"<<__LINE__<<std::endl;
			query(query_string);
			m_res->next();
			if(m_res->rowsCount()<1||m_res->isNull("short_name")||m_res->getString(1)=="") 
			{
				std::cout<<query_string<<":"<<__FILE__<<":"<<__LINE__<<std::endl;
			}
			else
			{
				i->sales_company_name=m_res->getString("short_name");
			}	
		}
   
	};
	//std::for_each(m_report_datas.begin(),m_report_datas.end(),[](boost::shared_ptr<report_data>& x){x->print();});
	}
	catch (sql::SQLException &e) 
	{
	  BOOST_LOG_SEV(slg, error)<<"# ERR: " << e.what();
	  BOOST_LOG_SEV(slg, error)<<" (MySQL error code: " << e.getErrorCode();
	  BOOST_LOG_SEV(slg, error)<<", SQLState: " << e.getSQLState();

	}
	catch (std::exception& e)
  	{
    	BOOST_LOG_SEV(slg, error)<<"# ERR: " << e.what();
  	}
  	catch (...)
  	{
    	BOOST_LOG_SEV(slg, error)<<"unknown error ";
  	}
}
void month_report::deal_with_approved_status()
{
	try
	{
	for(auto& i:m_report_datas)
	{
		std::string query_string="select approval_status from t_quotation where quotation_id='"+i->quotation_id+"'";
		//std::cout<<query_string<<":"<<__FILE__<<":"<<__LINE__<<std::endl;
		query(query_string);
		m_res->next();
		if(m_res->rowsCount()<1||m_res->isNull("approval_status")||m_res->getString(1)=="") 
		{
			continue;
		}
		std::string status=m_res->getString(1);
		/*审批状态, 0:Pending; 1:Draft; 2:Submitted; 3:Approved; 4:Accepted; 5:Canceled; 6:Expired; 7:Rejected; 8:QU_Generated; 9:Refused; 10:PI_Generated; 11:Paying; 12:AR_Confirmed; 13:SO_Generated*/
		std::map<std::string,std::string> m={{"0","Pending"}, {"1","Draft"}, {"2","Submitted"}, {"3","Approved"}, {"4","Accepted"}, {"5","Canceled"}, {"6","Expired"}, {"7","Rejected"}, {"8","QU_Generated"}, {"9","Refused"}, {"10","PI_Generated"}, {"11","Paying"}, {"12","AR_Confirmed"}, {"13","SO_Generated"}};
		i->approval_status=m[status];
   		
	};
	std::for_each(m_report_datas.begin(),m_report_datas.end(),[](boost::shared_ptr<report_data>& x){x->print();});
	}
	catch (sql::SQLException &e) 
	{
	  BOOST_LOG_SEV(slg, error)<<"# ERR: " << e.what();
	  BOOST_LOG_SEV(slg, error)<<" (MySQL error code: " << e.getErrorCode();
	  BOOST_LOG_SEV(slg, error)<<", SQLState: " << e.getSQLState();

	}
	catch (std::exception& e)
  	{
    	BOOST_LOG_SEV(slg, error)<<"# ERR: " << e.what();
  	}
  	catch (...)
  	{
    	BOOST_LOG_SEV(slg, error)<<"unknown error ";
  	}
}
void month_report::deal_with_trade_term_info()
{
	try
	{
	for(auto& i:m_report_datas)
	{
		std::string query_string="select trade_term_id from t_quotation where quotation_id='"+i->quotation_id+"'";
		//std::cout<<query_string<<":"<<__FILE__<<":"<<__LINE__<<std::endl;
		query(query_string);
		m_res->next();
		if(m_res->rowsCount()<1||m_res->isNull("trade_term_id")||m_res->getString(1)=="") 
		{
			continue;
		}
		
		query_string="select short_name from t_trade_term where trade_term_id='"+m_res->getString(1)+"'";
		//std::cout<<query_string<<":"<<__FILE__<<":"<<__LINE__<<std::endl;
		query(query_string);
		m_res->next();
		if(m_res->rowsCount()<1||m_res->isNull("short_name")||m_res->getString(1)=="") continue;
		i->price_condition=m_res->getString("short_name");
   
	};
	std::for_each(m_report_datas.begin(),m_report_datas.end(),[](boost::shared_ptr<report_data>& x){x->print();});
	}
	catch (sql::SQLException &e) 
	{
	  BOOST_LOG_SEV(slg, error)<<"# ERR: " << e.what();
	  BOOST_LOG_SEV(slg, error)<<" (MySQL error code: " << e.getErrorCode();
	  BOOST_LOG_SEV(slg, error)<<", SQLState: " << e.getSQLState();

	}
	catch (std::exception& e)
  	{
    	BOOST_LOG_SEV(slg, error)<<"# ERR: " << e.what();
  	}
  	catch (...)
  	{
    	BOOST_LOG_SEV(slg, error)<<"unknown error ";
  	}
}
void month_report::deal_with_product_info()
{
	try
	{
	for(auto& i:m_report_datas)
	{
		std::string query_string="select item_master_id from t_quotation_detail where quotation_detail_id='"+i->quotation_detail_id+"'";
		//std::cout<<query_string<<":"<<__FILE__<<":"<<__LINE__<<std::endl;
		query(query_string);
		m_res->next();
		if(m_res->rowsCount()<1||m_res->isNull("item_master_id")||m_res->getString(1)=="") 
		{
			continue;
		}
		
		query_string="select item_basic_id,product_code,product_name from t_item_master where item_master_id='"+m_res->getString(1)+"'";
		//std::cout<<query_string<<":"<<__FILE__<<":"<<__LINE__<<std::endl;
		query(query_string);
		m_res->next();
		if(m_res->rowsCount()<1||m_res->isNull("item_basic_id")||m_res->getString(1)=="") continue;
		i->product_name_id=m_res->getString("product_code");
		i->product_name=m_res->getString("product_name");
   ///////////////////////////////////////////////////////////////////
		query_string="select item_category_id from t_item_basic where item_basic_id='"+m_res->getString(1)+"'";
		query(query_string);
		m_res->next();
		if(m_res->rowsCount()<1||m_res->isNull("item_category_id")||m_res->getString(1)=="") continue;

		query_string="select product_category_id from t_product_category_item_category_link where item_category_id='"+m_res->getString(1)+"'";
		query(query_string);
		m_res->next();
		if(m_res->rowsCount()<1||m_res->isNull("product_category_id")||m_res->getString(1)=="") continue;

		query_string="select parent_product_category_id from t_product_category where product_category_id='"+m_res->getString(1)+"'";
		query(query_string);
		m_res->next();
		if(m_res->rowsCount()<1||m_res->isNull("parent_product_category_id")||m_res->getString(1)=="") continue;
		
		// query_string="select name from t_product_category where product_category_id='"+m_res->getString(1)+"'";
		// query(query_string);
		// m_res->next();
		// if(m_res->rowsCount()<1||m_res->isNull("name")||m_res->getString(1)=="") continue;
		// i->product_classification=m_res->getString(1);
	};
	//std::for_each(m_report_datas.begin(),m_report_datas.end(),[](boost::shared_ptr<report_data>& x){x->print();});
	}
	catch (sql::SQLException &e) 
	{
	  BOOST_LOG_SEV(slg, error)<<"# ERR: " << e.what();
	  BOOST_LOG_SEV(slg, error)<<" (MySQL error code: " << e.getErrorCode();
	  BOOST_LOG_SEV(slg, error)<<", SQLState: " << e.getSQLState();

	}
	catch (std::exception& e)
  	{
    	BOOST_LOG_SEV(slg, error)<<"# ERR: " << e.what();
  	}
  	catch (...)
  	{
    	BOOST_LOG_SEV(slg, error)<<"unknown error ";
  	}
}
void month_report::deal_with_payment_method_info()
{
	try
	{
	for(auto& i:m_report_datas)
	{
		std::string query_string="select payment_method_id from t_payment_term where quotation_id='"+i->quotation_id+"'";
		//std::cout<<query_string<<":"<<__FILE__<<":"<<__LINE__<<std::endl;
		query(query_string);
		m_res->next();
		if(m_res->rowsCount()<1||m_res->isNull("payment_method_id")||m_res->getString(1)=="") 
		{
			//std::cout<<__FILE__<<":"<<__LINE__<<std::endl;
			continue;
		}
		
		query_string="select name from t_payment_method where payment_method_id='"+m_res->getString(1)+"'";
		//std::cout<<query_string<<":"<<__FILE__<<":"<<__LINE__<<std::endl;
		query(query_string);
		m_res->next();
		if(m_res->isNull("name")||m_res->getString(1)=="") continue;
		i->payment_term_desc=m_res->getString("name");
		
	};
	//std::for_each(m_report_datas.begin(),m_report_datas.end(),[](boost::shared_ptr<report_data>& x){x->print();});
	}
	catch (sql::SQLException &e) 
	{
	  BOOST_LOG_SEV(slg, error)<<"# ERR: " << e.what();
	  BOOST_LOG_SEV(slg, error)<<" (MySQL error code: " << e.getErrorCode();
	  BOOST_LOG_SEV(slg, error)<<", SQLState: " << e.getSQLState();

	}
	catch (std::exception& e)
  	{
    	BOOST_LOG_SEV(slg, error)<<"# ERR: " << e.what();
  	}
  	catch (...)
  	{
    	BOOST_LOG_SEV(slg, error)<<"unknown error ";
  	}
}
void month_report::deal_with_currency_info()
{
	try
	{
	for(auto& i:m_report_datas)
	{
		std::string query_string="select currency_id from t_quotation where quotation_id='"+i->quotation_id+"'";
		//std::cout<<query_string<<":"<<__FILE__<<":"<<__LINE__<<std::endl;
		query(query_string);
		m_res->next();
		if(m_res->isNull("currency_id")||m_res->getString(1)=="") continue;
		query_string="select code from t_currency where currency_id='"+m_res->getString(1)+"'";
		//std::cout<<query_string<<":"<<__FILE__<<":"<<__LINE__<<std::endl;
		query(query_string);
		m_res->next();
		if(m_res->isNull("code")||m_res->getString(1)=="") continue;
		i->currency=m_res->getString("code");
		i->price_total_currency=m_res->getString("code");
		i->guided_currency=m_res->getString("code");
	};
	//std::for_each(m_report_datas.begin(),m_report_datas.end(),[](boost::shared_ptr<report_data>& x){x->print();});
	}
	catch (sql::SQLException &e) 
	{
	  BOOST_LOG_SEV(slg, error)<<"# ERR: " << e.what();
	  BOOST_LOG_SEV(slg, error)<<" (MySQL error code: " << e.getErrorCode();
	  BOOST_LOG_SEV(slg, error)<<", SQLState: " << e.getSQLState();

	}
	catch (std::exception& e)
  	{
    	BOOST_LOG_SEV(slg, error)<<"# ERR: " << e.what();
  	}
  	catch (...)
  	{
    	BOOST_LOG_SEV(slg, error)<<"unknown error ";
  	}
}
void month_report::deal_with_customer_info()
{
	try
	{
	//std::for_each(m_report_datas.begin(),m_report_datas.end(),[&](boost::shared_ptr<report_data>& i)
	for(auto& i:m_report_datas)
	{
		std::string query_string="select delivery_country_id from t_quotation where quotation_id='"+i->quotation_id+"'";
		
		query(query_string);
		m_res->next();
		query_string="select full_name,area_id from t_country where country_id='"+m_res->getString(1)+"'";
	
		query(query_string);
		m_res->next();
		if(!(m_res->isNull("full_name")))
			i->receiving_countries=m_res->getString("full_name");
		if(!(m_res->isNull("area_id")))
		{	
			query_string="select short_name from t_area where area_id='"+m_res->getString("area_id")+"'";
			//std::cout<<query_string<<":"<<__FILE__<<":"<<__LINE__<<std::endl;
			query(query_string);
			m_res->next();
			if(!(m_res->isNull("short_name"))) 
				i->import_area=m_res->getString("short_name");
		}
		////////////////////////////////////////////////
		query_string="select customer_basic_id from t_quotation where quotation_id='"+i->quotation_id+"'";
		//std::cout<<query_string<<":"<<__FILE__<<":"<<__LINE__<<std::endl;
		query(query_string);
		m_res->next();
		if(m_res->isNull("customer_basic_id")) continue;
		query_string="select country_id,trade_name from t_customer_basic where customer_basic_id='"+m_res->getString(1)+"'";
		//std::cout<<query_string<<":"<<__FILE__<<":"<<__LINE__<<std::endl;
		query(query_string);
		m_res->next();
		if(m_res->isNull("country_id")||m_res->getString(1)=="") continue;
		i->country_id=m_res->getString(1);
		if(m_res->isNull("trade_name")||m_res->getString(2)=="") 
		{

		}
		else
		{
			i->account_name=m_res->getString(2);
		}
		query_string="select full_name,area_id from t_country where country_id='"+m_res->getString(1)+"'";
		//std::cout<<query_string<<":"<<__FILE__<<":"<<__LINE__<<std::endl;
		query(query_string);
		m_res->next();
		if(m_res->isNull("full_name")) continue;
		i->customer_countries=m_res->getString(1);
		if(m_res->isNull("area_id")) continue;
		query_string="select full_name from t_area where area_id='"+m_res->getString(2)+"'";
		//std::cout<<query_string<<":"<<__FILE__<<":"<<__LINE__<<std::endl;
		query(query_string);
		m_res->next();
		if(m_res->isNull("full_name")) continue;
		i->country_region=m_res->getString(1);

	};//);
	//std::for_each(m_report_datas.begin(),m_report_datas.end(),[](boost::shared_ptr<report_data>& x){x->print();});
}
	catch (sql::SQLException &e) 
	{
	  BOOST_LOG_SEV(slg, error)<<"# ERR: " << e.what();
	  BOOST_LOG_SEV(slg, error)<<" (MySQL error code: " << e.getErrorCode();
	  BOOST_LOG_SEV(slg, error)<<", SQLState: " << e.getSQLState();

	}
	catch (std::exception& e)
  	{
    	BOOST_LOG_SEV(slg, error)<<"# ERR: " << e.what();
  	}
  	catch (...)
  	{
    	BOOST_LOG_SEV(slg, error)<<"unknown error ";
  	}
}
void month_report::deal_with_sales_info()
{
	try
	{
	//for(auto i=m_report_datas.begin();i!=m_report_datas.end();++i)
	std::for_each(m_report_datas.begin(),m_report_datas.end(),[&](boost::shared_ptr<report_data>& i)
	{
		std::string query_string="select sales_id from t_quotation where quotation_id='"+(i)->quotation_id+"'";
		//std::cout<<query_string<<":"<<__FILE__<<":"<<__LINE__<<std::endl;

		query(query_string);
		m_res->next();
		query_string="select employee_no from t_system_account where system_account_id='"+m_res->getString(1)+"'";
	
		query(query_string);
		m_res->next();
		query_string="select full_name,position_id  from t_employee where employee_id='"+m_res->getString("employee_no")+"'";
		
		query(query_string);
		m_res->next();
		
		i->sales_full_name=m_res->getString("full_name");
		// query_string="select full_name,company_id from t_position where position_id='"+m_res->getString("position_id")+"'";

		// query(query_string);
		// m_res->next();

		// i->sales_type=m_res->getString("full_name");
		// query_string="select full_name from t_company where company_id='"+m_res->getString("company_id")+"'";

		// query(query_string);
		// m_res->next();
		// i->account_name=m_res->getString("full_name");
		
	});
	//std::for_each(m_report_datas.begin(),m_report_datas.end(),[](boost::shared_ptr<report_data>& x){x->print();});
}
	catch (sql::SQLException &e) 
	{
	  
	  BOOST_LOG_SEV(slg, error)<<"# ERR: " << e.what();
	  BOOST_LOG_SEV(slg, error)<<" (MySQL error code: " << e.getErrorCode();
	  BOOST_LOG_SEV(slg, error)<<", SQLState: " << e.getSQLState();

	}
	catch (std::exception& e)
  	{
    	BOOST_LOG_SEV(slg, error)<<"# ERR: " << e.what();
  	}
  	catch (...)
  	{
    	BOOST_LOG_SEV(slg, error)<<"unknown error ";
  	}
}
void month_report::insert_data()
{
	try
	{
		std::cout<<__FILE__<<":"<<__LINE__<<":"<<m_res->rowsCount()<<std::endl;
	while(m_res->next())
	{
		//std::cout<< m_res->getString(1)<<":" << m_res->getString("product_qty_pc")<<std::endl;
	 	boost::shared_ptr<report_data> temp(new report_data());
	 	temp->quotation_id=m_res->getString("quotation_id");
	 	temp->quotation_detail_id=m_res->getString("quotation_detail_id");

	 	temp->quotation_no=m_res->getString("quotation_no");
	 	temp->product_qty_pc=m_res->getDouble("product_qty_pc");
	 	
	 	if(!m_res->isNull("module_extend_prop"))
	 	{
	 		temp->product_qty_w=m_res->getDouble("product_qty_pc")*m_res->getDouble("module_extend_prop");
	 	}
	 	else
	 	{
	 		temp->product_qty_w=0;
	 	}
	 	if(!m_res->isNull("guidance_price"))
	 	{
	 		temp->guidance_price=m_res->getDouble("guidance_price");
	 		temp->total_guidance_price=temp->product_qty_w*m_res->getDouble("guidance_price");
	 	}
	 	if(!m_res->isNull("unit_price"))
	 	{
	 		temp->unit_price=m_res->getDouble("unit_price");
	 		temp->price_total=temp->unit_price*temp->product_qty_w;
	 	}
	 	
	 	temp->creat_at=m_res->getString("creat_at");
	 	m_report_datas.push_back(temp);
	 	//std::for_each(m_report_datas.begin(),m_report_datas.end(),[](boost::shared_ptr<report_data>& x){x->print();});
	}
	}
	catch (sql::SQLException &e) 
	{
	  //ming_log->get_log_console()->info()<< "# ERR: " << e.what();
	  //ming_log->get_log_console()->info()<< " (MySQL error code: " << e.getErrorCode();
	  //ming_log->get_log_console()->info()<< ", SQLState: " << e.getSQLState();
	  BOOST_LOG_SEV(slg, error)<<"# ERR: " << e.what();
	  BOOST_LOG_SEV(slg, error)<<" (MySQL error code: " << e.getErrorCode();
	  BOOST_LOG_SEV(slg, error)<<", SQLState: " << e.getSQLState();

	}
	catch (std::exception& e)
  	{
    	BOOST_LOG_SEV(slg, error)<<"# ERR: " << e.what();
  	}
  	catch (...)
  	{
    	BOOST_LOG_SEV(slg, error)<<"unknown error ";
  	}
}
void month_report::write_to_csv()
{
	write_csv w("all_report.csv");
	std::for_each(m_report_datas.begin(),m_report_datas.end(),[&](boost::shared_ptr<report_data>& x)
		{
			//if(x->sales_company_name=="ReneSola Japan")
			{
				w.addData(x->csv_line());
			}
		});
}
void month_report::write_to_excel()
{
	// write_csv w("all_report.xlsx");
	// std::for_each(m_report_datas.begin(),m_report_datas.end(),[&](boost::shared_ptr<report_data>& x)
	// 	{
	// 		//if(x->sales_company_name=="ReneSola Japan")
	// 		{
	// 			w.addData(x->csv_line());
	// 		}
	// 	});

	boost::shared_ptr<XLSXIOWriter> xlsxfile(new XLSXIOWriter("all_report.xlsx"));
    xlsxfile->SetRowHeight(1);
  	std::vector<std::string> v
  	{	
		"Quotation_NO.","Sales","Sales_Company","Customer","Country","Destination_Country","Import_Area","Approval_Status","Trade_Term","Product_Classification","Product_Code","Qty(PCS)","Power(Wp)","Unit_price_Currency","Unit_price","Guidance_Price_Currency","Guidance_Price","Total_Price","Guided_Total_Price","Payment_Terms","Quotation_Date"
  	};
  
  	for(const auto& i:v)
  	{
    	xlsxfile->AddColumn(i.c_str());
  	}
  	xlsxfile->NextRow();
  
  	std::for_each(m_report_datas.begin(),m_report_datas.end(),[&](boost::shared_ptr<report_data>& x)
	{
		*xlsxfile<<x->quotation_no;
	 	*xlsxfile<<x->sales_full_name;
	 	*xlsxfile<<x->sales_company_name;
	 	*xlsxfile<<x->account_name;
	 	*xlsxfile<<x->customer_countries;
	 	*xlsxfile<<x->receiving_countries;
	 	*xlsxfile<<x->import_area;
	 	*xlsxfile<<x->approval_status;
	 	*xlsxfile<<x->price_condition;
	 	*xlsxfile<<x->product_name;
		*xlsxfile<<x->product_name_id;//export as product_name_id
	 	*xlsxfile<<x->product_qty_pc;
		*xlsxfile<<x->product_qty_w;
		*xlsxfile<<x->currency;
		*xlsxfile<<x->unit_price;
		*xlsxfile<<x->guided_currency;
		*xlsxfile<<x->guidance_price;
		*xlsxfile<<x->price_total;
		*xlsxfile<<x->total_guidance_price;
		*xlsxfile<<x->payment_term_desc;
	 	*xlsxfile<<x->creat_at;
		xlsxfile->NextRow();
	}); 
  }
void month_report::start()
{
	try
	{
	std::string query_string=
	"SELECT\
	t_quotation.quotation_id as quotation_id,\
	t_quotation_detail.quotation_detail_id as quotation_detail_id,\
	t_quotation.quotation_no as quotation_no,\
	t_quotation_detail.quantity as product_qty_pc,\
	t_quotation_detail.ss_guidance_price as guidance_price,\
	t_quotation_detail.module_extend_prop as module_extend_prop,\
	t_quotation_detail.applied_unit_price as unit_price,\
	t_quotation.createAt as creat_at\
	FROM\
	t_quotation INNER JOIN\
	t_quotation_detail\
	on\
	t_quotation_detail.quotation_id = t_quotation.quotation_id\
	and t_quotation_detail.dr=0 and t_quotation.createAt >'2016-01-01 00:00:01' and t_quotation.createAt <'2016-12-31 23:59:59'\
	ORDER BY t_quotation.createAt";
	
	query(query_string);

	insert_data();
	deal_with_sales_info();
	deal_with_customer_info();
	deal_with_currency_info();
	deal_with_payment_method_info();
	deal_with_product_info();
	deal_with_trade_term_info();
	deal_with_sales_country();
	deal_with_approved_status();
	deal_with_pi();
	// write_to_csv();
	write_to_excel();
   } 
	catch (sql::SQLException &e) 
	{
	  //ming_log->get_log_console()->info()<< "# ERR: " << e.what();
	  //ming_log->get_log_console()->info()<< " (MySQL error code: " << e.getErrorCode();
	  //ming_log->get_log_console()->info()<< ", SQLState: " << e.getSQLState();
	  BOOST_LOG_SEV(slg, error)<<"# ERR: " << e.what();
	  BOOST_LOG_SEV(slg, error)<<" (MySQL error code: " << e.getErrorCode();
	  BOOST_LOG_SEV(slg, error)<<", SQLState: " << e.getSQLState();

	}
	catch (std::exception& e)
  	{
    	BOOST_LOG_SEV(slg, error)<<"# ERR: " << e.what();
  	}
  	catch (...)
  	{
    	BOOST_LOG_SEV(slg, error)<<"unknown error ";
  	}
}
void month_report::query(const std::string& query_sql)
{
	try 
	{
	  //std::cout<<__FILE__<<":"<<__LINE__<<std::endl;
	  m_pstmt = boost::shared_ptr<sql::PreparedStatement>(m_con->prepareStatement(query_sql));
	  m_res = boost::shared_ptr<sql::ResultSet>(m_pstmt->executeQuery());

	} 
	catch (sql::SQLException &e) 
	{
	  //ming_log->get_log_console()->info()<< "# ERR: " << e.what();
	  //ming_log->get_log_console()->info()<< " (MySQL error code: " << e.getErrorCode();
	  //ming_log->get_log_console()->info()<< ", SQLState: " << e.getSQLState();
	  BOOST_LOG_SEV(slg, error)<<"# ERR: " << e.what();
	  BOOST_LOG_SEV(slg, error)<<" (MySQL error code: " << e.getErrorCode();
	  BOOST_LOG_SEV(slg, error)<<", SQLState: " << e.getSQLState();

	}
}
boost::shared_ptr<sql::ResultSet> month_report::get_res()const
{
	return m_res;
}
void start_report()
{
	boost::shared_ptr<mysql_info_> info(new mysql_info_());
	info->ip=get_config->m_mysql_js_ip;
	info->username=get_config->m_mysql_js_username;
	info->password=get_config->m_mysql_js_password;
	info->database=get_config->m_mysql_js_database;

	info->port=boost::lexical_cast<std::string>(get_config->m_mysql_js_port);
	boost::shared_ptr<month_report> report(new month_report(info));
	report->start();
}