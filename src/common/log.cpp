
#include "log.hpp"
//int kRollSize = 50*1000*1000;
size_t kRollSize=boost::lexical_cast<int>(get_config->m_log_size)*1000*1000;
boost::shared_ptr<muduo::AsyncLogging> g_asyncLog = nullptr;

void asyncOutput(const char* msg, int len)
{
  g_asyncLog->append(msg, len);
}

void bench(bool longLog)
{
  // muduo::Logger::setOutput(asyncOutput);

  // int cnt = 0;
  // const int kBatch = 1000000;
  // muduo::string empty = " ";
  // muduo::string longStr(3000, 'X');
  // longStr += " ";

  // for (int t = 0; t < 1; ++t)
  // {
  //   muduo::Timestamp start = muduo::Timestamp::now();
  //   for (int i = 0; i < kBatch; ++i)
  //   {
  //     LOG_INFO << "Hello 0123456789" << " abcdefghijklmnopqrstuvwxyz "
  //              << (longLog ? longStr : empty)
  //              << cnt;
  //     ++cnt;
  //   }
  //   muduo::Timestamp end = muduo::Timestamp::now();
  //   printf("%f\n", timeDifference(end, start)*1000000/kBatch);
  //   struct timespec ts = { 0, 500*1000*1000 };
  //   nanosleep(&ts, NULL);
  // }

    muduo::ThreadPool pool("pool");
    pool.start(5);
    for(int i=0;i<5;++i)
      pool.run([](){LOG_INFO<<"info"<<" dsds";sleep(1);});
    LOG_DEBUG<<"DEBUG";
}

void init_log()
{
	try
	{
		size_t kOneGB = 1000*1024*1024;
	    rlimit rl = { 1*kOneGB, 1*kOneGB };
	    setrlimit(RLIMIT_AS, &rl);
	
	    muduo::Logger::setOutput(asyncOutput);
	    if(get_config->m_log_level=="info")
		{
			muduo::Logger::setLogLevel(muduo::Logger::LogLevel::INFO);
		}
		else if(get_config->m_log_level=="debug")
		{
			muduo::Logger::setLogLevel(muduo::Logger::LogLevel::DEBUG);
		}
		else if(get_config->m_log_level=="error")
		{
			muduo::Logger::setLogLevel(muduo::Logger::LogLevel::ERROR);
		}
		else
		{
			muduo::Logger::setLogLevel(muduo::Logger::LogLevel::WARN);
		}
		
		muduo::TimeZone beijing(8*3600, "CST");
	  	muduo::Logger::setTimeZone(beijing);
	  	muduo::string file_name(get_config->m_log_name.c_str());

	    //cout<<get_config->m_log_name<<endl;
	  
	  	boost::shared_ptr<muduo::AsyncLogging> log(new muduo::AsyncLogging(file_name, kRollSize));
	  	log->start();
	  	g_asyncLog = log;

	}
	  catch (std::exception& e)
	  {
	    //cout << diagnostic_information(e) << endl;
	    std::cout << e.what() << std::endl;
	  }
	  catch (...)
	  {
	    std::cout << "unknown error" << std::endl;
	  }	
}
