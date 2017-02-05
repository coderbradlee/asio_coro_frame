#include "log.h"
#include "config.hpp"
std::map<std::string,severity_level> severitymap=
{
  { "normal", severity_level::normal },
  { "notification", severity_level::notification },
  { "warning", severity_level::warning},
  { "error", severity_level::error },
  { "critical", severity_level::critical}
};

//boost::shared_ptr< sink_t > initlog()
boost::shared_ptr< file_sink > initlog()
{
   try
    {
		  boost::shared_ptr< file_sink > sink(new file_sink(
      	keywords::file_name = get_config->m_log_name+"_%Y%m%d_%5N.log",
            keywords::rotation_size = 16*1024 * 1024 // rotation size, in characters
            keywords::auto_flush=get_config->m_log_auto_flush;
            ));
      // sink->auto_flush(true);
        // Set up where the rotated files will be stored
        sink->locked_backend()->set_file_collector(sinks::file::make_collector(
            keywords::target = "logs",                          // where to store rotated files
            keywords::max_size = 16*1024 * 1024,              // maximum total size of the stored files, in bytes
            keywords::min_free_space = 100 * 1024 * 1024        // minimum free space on the drive, in bytes
            // keywords::auto_flush=true
            ));
        sink->locked_backend()->scan_for_files();
        sink->set_formatter
        (
            expr::format("%1%: [%2%] [%3%] [%4%] - %5%")
                % expr::attr< unsigned int >("RecordID")
				        % expr::attr< severity_level >("Severity")
                % expr::attr< boost::posix_time::ptime >("TimeStamp")
               // % expr::attr< boost::thread::id >("ThreadID")
				        % expr::attr< attrs::current_thread_id::value_type >("ThreadID")
                % expr::smessage
        );
		    sink->set_filter(expr::attr< severity_level >("Severity").or_default(normal) >= severitymap[get_config->m_log_level]);
        // Add it to the core
        logging::core::get()->add_sink(sink);
        logging::core::get()->add_global_attribute("TimeStamp", attrs::local_clock());
        logging::core::get()->add_global_attribute("RecordID", attrs::counter< unsigned int >());
		    logging::core::get()->add_global_attribute("ThreadID", attrs::current_thread_id());
        return sink;
    }
    catch (std::exception& e)
    {
        std::cout << "FAILURE: " << e.what() << std::endl;
        return nullptr;
    }
}

boost::shared_ptr< file_sink > initsink=initlog();//init log;
src::severity_logger< severity_level > slg;
