#include "pdf_api.hpp"

pdf_api::pdf_api()
{
    FCGX_Init();
    FCGX_InitRequest( &m_request, 0, 0 );
}
void pdf_api::run(FCGX_Request request)
{
    FCGX_FPrintF( request.out,
        "Content-Type: application/xml; charset=UTF-8\r\n"
        "Content-Encoding: gzip\r\n"
        "\r\n"
    );
    do_convert();
    int num_bytes_written = FCGX_PutStr( m_test.c_str(), m_test.length(), request.out );
    if( num_bytes_written != (int)m_test.length() || num_bytes_written == -1 )
    {
    }

    FCGX_Finish_r( &request );

}
void pdf_api::do_convert()
{
    pdf_impl::convert();
    m_test="ok";
}
std::string pdf_api::get_request_uri()
{
    return FCGX_GetParam("REQUEST_URI", m_request.envp);
}
pdf_api::~pdf_api()
{
}
void pdf_impl::convert()
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
    wkhtmltopdf_set_global_setting(gs, "out", "test.pdf");

    wkhtmltopdf_set_global_setting(gs, "load.cookieJar", "myjar.jar");
    /*
     * Create a input object settings object that is used to store settings
     * related to a input object, note again that control of this object is parsed to
     * the converter later, which is then responsible for freeing it
     */
    os = wkhtmltopdf_create_object_settings();
    /* We want to convert to convert the qstring documentation page */
    wkhtmltopdf_set_object_setting(os, "page", "test.html");

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
    if (!wkhtmltopdf_convert(c))
        fprintf(stderr, "Conversion failed!");

    /* Output possible http error code encountered */
    printf("httpErrorCode: %d\n", wkhtmltopdf_http_error_code(c));

    /* Destroy the converter object since we are done with it */
    wkhtmltopdf_destroy_converter(c);

    /* We will no longer be needing wkhtmltopdf funcionality */
    wkhtmltopdf_deinit();
}