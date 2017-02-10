#include "fastcgi.h"
void doit()
{
    int rc;
    FCGX_Request request;
    
    FCGX_InitRequest( &request, 0, 0 );

    int num_bytes_written;
	string served_contents="ming served_contents";

    int len = served_contents.length();

    for(;;)
    {
		rc = FCGX_Accept_r( &request );
        
        if (rc < 0)
            continue;

        FCGX_FPrintF( request.out,
            "Content-Type: application/xml; charset=UTF-8\r\n"
            "Content-Encoding: gzip\r\n"
            "\r\n"
        );

        num_bytes_written = FCGX_PutStr( served_contents.c_str(), len, request.out );
        if( num_bytes_written != len || num_bytes_written == -1 )
        {
            break;
        }

        FCGX_Finish_r( &request );
    }
}
