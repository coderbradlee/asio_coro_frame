//
// connection_manager.cpp
// ~~~~~~~~~~~~~~~~~~~~~~
//
// Copyright (c) 2003-2016 Christopher M. Kohlhoff (chris at kohlhoff dot com)
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//

#include "connection_manager.hpp"

namespace http {
namespace server {

connection_manager::connection_manager(boost::asio::io_service& io):m_io_service(io),m_strand(io)
{
}

void connection_manager::start(connection_ptr c)
{
	m_io_service.post(m_strand.wrap([&]()
		{
		  connections_.insert(c);
		  c->start();
		}
	));
}

void connection_manager::stop(connection_ptr c)
{
	m_io_service.post(m_strand.wrap([&]()
		{
		  connections_.erase(c);
  		  c->stop();
		}
	));
  
}

void connection_manager::stop_all()
{
  
  m_io_service.post(m_strand.wrap([&]()
		{
		  	for (auto c: connections_)
    		c->stop();
  			connections_.clear();
		}
	));
}

} // namespace server
} // namespace http
