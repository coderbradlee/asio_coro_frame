#include "router.hpp"


router::router()
{
    FCGX_Init();
    FCGX_InitRequest( &m_request, 0, 0 );
}
void router::run()
{
    for(;;)
    {
        int rc = FCGX_Accept_r( &m_request );
        
        if (rc < 0)
            continue;

        // FCGX_FPrintF( m_request.out,
        //     "Content-Type: application/xml; charset=UTF-8\r\n"
        //     "Content-Encoding: gzip\r\n"
        //     "\r\n");
        std::string uri; 
        uri = FCGX_GetParam("REQUEST_URI", m_request.envp);
        std::cout<<uri<<std::endl;
        // int num_bytes_written = FCGX_PutStr( m_test.c_str(), m_test.length(), m_request.out );
        // if( num_bytes_written != m_test.length() || num_bytes_written == -1 )
        // {
        //     break;
        // }
        // boost::thread_group m_thread_group;
        // for (size_t i = 0; i < 1; ++i)
        // {
        //     m_thread_group.create_thread(boost::bind(&redis_api::run,&r));
        //     m_thread_group.create_thread(boost::bind(&pdf_api::run,&p));
        // }

        // m_thread_group.join_all();
        // 
        FCGX_PutStr( uri.c_str(), uri.length(), m_request.out );
        // str1.compare(str2) != 0
        if(uri.compare("/redis/")==0)
        {
            redis_api r;
            r.run(m_request);
        }
        else if(uri.compare("/pdf/")==0)
        {
            pdf_api p;
            p.run(m_request);
        }
        FCGX_Finish_r( &m_request );
    }
}