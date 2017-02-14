#ifndef PDF_HPP
#define PDF_HPP
#include <pthread.h>
#include <sys/types.h>
#include <string>
#include <set>
#include <vector>
#include <exception>
#include <fstream>
#include <iostream>
#include <cstdlib>
#include "config.hpp"
#include "fcgi_config.h"
#include "fcgiapp.h"

class pdf_api
{
public:
    explicit pdf_api();
    void run();
    void do_convert();
    std::string get_request_uri();
    ~pdf_api();
private:
    FCGX_Request m_request;
    std::vector<char> m_data;
    std::string m_test;
};

#endif  /* SERVER_HTTP_HPP */