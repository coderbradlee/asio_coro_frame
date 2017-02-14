#include "pdf.hpp"

pdf_api::pdf_api()
{
    FCGX_Init();
    FCGX_InitRequest( &m_request, 0, 0 );
}
void pdf_api::run()
{
    for(;;)
    {
        int rc = FCGX_Accept_r( &m_request );
        
        if (rc < 0)
            continue;
        std::cout<<get_request_uri()<<std::endl;
        FCGX_FPrintF( m_request.out,
            "Content-Type: application/xml; charset=UTF-8\r\n"
            "Content-Encoding: gzip\r\n"
            "\r\n"
        );
        do_convert();
        int num_bytes_written = FCGX_PutStr( m_test.c_str(), m_test.length(), m_request.out );
        if( num_bytes_written != m_test.length() || num_bytes_written == -1 )
        {
            break;
        }

        FCGX_Finish_r( &m_request );
    }
}
void pdf_api::do_convert()
{
    m_test="ming";
}
std::string pdf_api::get_request_uri()
{
    return FCGX_GetParam("REQUEST_URI", m_request.envp);
}
pdf_api::~pdf_api()
{
}
