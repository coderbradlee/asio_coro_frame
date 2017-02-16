#include "redis_api.hpp"

redis_api::redis_api()
{
    FCGX_Init();
    FCGX_InitRequest( &m_request, 0, 0 );
    //m_cluster_p = HiredisCommand<ThreadPoolCluster>::createCluster(get_config->m_redis_host.c_str(),get_config->m_redis_port);
}
void redis_api::run(FCGX_Request& request)
{
    FCGX_FPrintF( request.out,
        "Content-Type: application/xml; charset=UTF-8\r\n"
        "Content-Encoding: gzip\r\n"
        "\r\n"
    );
    do_redis_read();
    int num_bytes_written = FCGX_PutStr( m_test.c_str(), m_test.length(), request.out );
    if( num_bytes_written != m_test.length() || num_bytes_written == -1 )
    {
        
    }

    FCGX_Finish_r( &request );

}
void redis_api::do_redis_read()
{
    m_test="redis_api";
}
redis_api::~redis_api()
{
    delete m_cluster_p;
}
