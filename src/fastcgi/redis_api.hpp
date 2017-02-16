#ifndef REDIS_API_HPP
#define REDIS_API_HPP
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
#include "redis_cluster.hpp"

class redis_api
{
public:
    explicit redis_api();
    void run(FCGX_Request& m_request);
    void do_redis_read();
    ~redis_api();
private:
    ThreadPoolCluster::ptr_t m_cluster_p;
    std::string m_test;
};

#endif  /* SERVER_HTTP_HPP */