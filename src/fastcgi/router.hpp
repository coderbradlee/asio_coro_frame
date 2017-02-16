#ifndef ROUTER_HPP
#define ROUTER_HPP
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
#include "redis_api.hpp"
#include "pdf_api.hpp"
class router
{
public:
    explicit router();
    void run();
private:
    FCGX_Request m_request;
};

#endif  /* SERVER_HTTP_HPP */