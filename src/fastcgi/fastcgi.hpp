#ifndef FASTCGI_HPP
#define FASTCGI_HPP
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
    explicit redis_api()
    {
        FCGX_Init();
        FCGX_InitRequest( &m_request, 0, 0 );
        //m_cluster_p = HiredisCommand<ThreadPoolCluster>::createCluster(get_config->m_redis_host.c_str(),get_config->m_redis_port);
    }
    void run()
    {
        for(;;)
        {
            int rc = FCGX_Accept_r( &m_request );
            
            if (rc < 0)
                continue;

            FCGX_FPrintF( m_request.out,
                "Content-Type: application/xml; charset=UTF-8\r\n"
                "Content-Encoding: gzip\r\n"
                "\r\n"
            );
            do_redis_read();
            int num_bytes_written = FCGX_PutStr( m_test.c_str(), m_test.length(), m_request.out );
            if( num_bytes_written != m_test.length() || num_bytes_written == -1 )
            {
                break;
            }

            FCGX_Finish_r( &m_request );
        }
    }
    void do_redis_read()
    {
        m_test="ming";
    }
    ~redis_api()
    {
        delete m_cluster_p;
    }
private:
    ThreadPoolCluster::ptr_t m_cluster_p;
    FCGX_Request m_request;
    std::vector<char> m_data;
    std::string m_test;
};
void doit();


#endif  /* SERVER_HTTP_HPP */