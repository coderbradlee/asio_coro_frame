#include "pdf_api.hpp"
#include "json_map.hpp"
#include <stdio.h>
pdf_api::pdf_api()
{
}
void pdf_api::get_request_content(const FCGX_Request & request) 
{
    // char * content_length_str = FCGX_GetParam("CONTENT_LENGTH",
    //                                            request.envp);
    std::string content_length_str= FCGX_GetParam("CONTENT_LENGTH", request.envp);
    int content_length = 0;
    
    if (content_length_str!="") 
    {
        content_length=boost::lexical_cast<int>(content_length_str);
    } 
    else 
    {
        // Do not read from stdin if CONTENT_LENGTH is missing
        content_length = 0;
    }
    BOOST_LOG_SEV(slg, notification)<<__LINE__<<":content_length:"<<content_length;
    std::shared_ptr<char> content_buffer(new char[content_length]);
    // request.in.read(content_buffer.get(), content_length);
    int read_len= FCGX_GetStr(content_buffer.get(), content_length, request.in);
    if(read_len!=content_length)
    {
        BOOST_LOG_SEV(slg, error)<<__LINE__<<":read_length:"<<read_len;
    }
    m_content=content_buffer.get();
}
void pdf_api::run(FCGX_Request& request)
{
    // FCGX_FPrintF( request.out,
    //     "Content-Type: application/xml; charset=UTF-8\r\n"
    //     "Content-Encoding: gzip\r\n"
    //     "\r\n"
    // );
    try
    {
        get_request_content(request);
        BOOST_LOG_SEV(slg, notification)<<__LINE__<<":content:"<<m_content;
        const auto& j = nlohmann_map::json::parse(m_content);
     
        const auto& src = j["src"];
        const auto& dst = j["dst"];

        do_convert(src,dst);
    }
    catch(std::exception& e)
    {
        BOOST_LOG_SEV(slg, error)<<__LINE__<<"json:"<<e.what();
        m_response="failed";
    }

    int num_bytes_written = FCGX_PutStr( m_response.c_str(), m_response.length(), request.out );
    if( num_bytes_written != (int)m_response.length() || num_bytes_written == -1 )
    {
        BOOST_LOG_SEV(slg, error)<<__LINE__<<":write content:"<<num_bytes_written;
    }

    FCGX_Finish_r( &request );
}
void pdf_api::do_convert(std::string src,std::string dst)
{
    bool success=pdf_impl::convert(src,dst);
    if(success)
    {
        m_response="ok";
    }
    else
    {
        m_response="failed";
    }
}
// std::std::string pdf_api::get_request_uri()
// {
//     return FCGX_GetParam("REQUEST_URI", m_request.envp);
// }
pdf_api::~pdf_api()
{
}
bool pdf_impl::convert(std::string src,std::string dst)
{
    wkhtmltopdf_global_settings * gs;
    wkhtmltopdf_object_settings * os;
    wkhtmltopdf_converter * c;

    /* Init wkhtmltopdf in graphics less mode */
    wkhtmltopdf_init(false);

    /*
     * Create a global settings object used to store options that are not
     * related to input objects, note that control of this object is parsed to
     * the converter later, which is then responsible for freeing it
     */
    gs = wkhtmltopdf_create_global_settings();
    /* We want the result to be storred in the file called test.pdf */
    wkhtmltopdf_set_global_setting(gs, "out", dst.c_str());

    // wkhtmltopdf_set_global_setting(gs, "load.cookieJar", "myjar.jar");
    /*
     * Create a input object settings object that is used to store settings
     * related to a input object, note again that control of this object is parsed to
     * the converter later, which is then responsible for freeing it
     */
    os = wkhtmltopdf_create_object_settings();
    /* We want to convert to convert the qstring documentation page */
    wkhtmltopdf_set_object_setting(os, "page", src.c_str());
    wkhtmltopdf_set_object_setting(os, "load.blockLocalFileAccess","false");
    /* Create the actual converter object used to convert the pages */
    c = wkhtmltopdf_create_converter(gs);

    /* Call the progress_changed function when progress changes */
    wkhtmltopdf_set_progress_changed_callback(c, pdf_impl::progress_changed);

    /* Call the phase _changed function when the phase changes */
    wkhtmltopdf_set_phase_changed_callback(c, pdf_impl::phase_changed);

    /* Call the error function when an error occurs */
    wkhtmltopdf_set_error_callback(c, pdf_impl::error);

    /* Call the warning function when a warning is issued */
    wkhtmltopdf_set_warning_callback(c, pdf_impl::warning);

    /*
     * Add the the settings object describing the qstring documentation page
     * to the list of pages to convert. Objects are converted in the order in which
     * they are added
     */
    wkhtmltopdf_add_object(c, os, NULL);

    /* Perform the actual conversion */
    bool ret=true;
    int temp=wkhtmltopdf_convert(c);
    BOOST_LOG_SEV(slg, severity_level::error)<<__LINE__<<":status:"<<temp;
    if (!temp)
    {   
        fprintf(stderr, "Conversion failed!");
        ret=false;
    }

    /* Output possible http error code encountered */
    printf("httpErrorCode: %d\n", wkhtmltopdf_http_error_code(c));
    BOOST_LOG_SEV(slg, severity_level::error)<<__LINE__<<":Got error code:"<<wkhtmltopdf_http_error_code(c);
    /* Destroy the converter object since we are done with it */
    wkhtmltopdf_destroy_converter(c);

    /* We will no longer be needing wkhtmltopdf funcionality */
    wkhtmltopdf_deinit();
    return ret;
}