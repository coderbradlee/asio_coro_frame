#include <boost/program_options.hpp>
#include "log.hpp"
#include "fastcgi/fastcgi_resource.h"
using std::string;
using std::cout;
using std::endl;
void start_inspector_thread();
void parseCommandLine(int argc, char* argv[])
{
    using namespace boost::program_options;
    //声明需要的选项
    options_description desc("Allowed options");
    desc.add_options()
        ("help,h", "produce help message")
        ("config,c", value<string>()->default_value("config.ini"), "set config")
        ;

    variables_map vm;        
    store(parse_command_line(argc, argv, desc), vm);
    notify(vm);    

    if (vm.count("help")) 
    {
        cout << desc;
    }
   
    if (vm.count("config"))  
    {  
        string conf_name = vm["config"].as<string>();  
        //LOG_INFO<<conf_name;
        //ifstream ifs_config(conf_name.c_str());  
        //boost::shared_ptr<iconfig> get_config= iconfig::get_instance(conf_name);
        //get_config= iconfig::get_instance(conf_name);
    }  
}
int main(int argc, char* argv[])
{
  try
  {
    parseCommandLine(argc, argv);

    init_log();
    muduo::Timestamp start = muduo::Timestamp::now();

    start_fastcgi();

    //test1::test1();
    //short_url::test();
    //start_inspector_thread();
    
    muduo::Timestamp end = muduo::Timestamp::now();
    printf("%f\n", timeDifference(end, start));
    sleep(10);
  }
  catch (std::exception& e)
  {
    //cout << diagnostic_information(e) << endl;
    cout << e.what() << endl;
  }
  catch (...)
  {
    cout << "unknown error" << endl;
  }
  return 0;
}