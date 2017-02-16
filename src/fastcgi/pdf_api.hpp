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
#include "log.h"
#include "fcgi_config.h"
#include "fcgiapp.h"
#include "pdf.h"
class pdf_api
{
public:
    explicit pdf_api();
    void run(FCGX_Request& m_request);
    void do_convert();
    // std::string get_request_uri();
    ~pdf_api();
private:
    std::string m_test;
};
class pdf_impl
{
public:
	pdf_impl();
	/* Print out loading progress information */
	static void progress_changed(wkhtmltopdf_converter * c, int p) 
	{
		printf("%3d%%\r",p);
		fflush(stdout);
	}

	/* Print loading phase information */
	static void phase_changed(wkhtmltopdf_converter * c) 
	{
		int phase = wkhtmltopdf_current_phase(c);
		printf("%s\n", wkhtmltopdf_phase_description(c, phase));
	}

	/* Print a message to stderr when an error occurs */
	static void error(wkhtmltopdf_converter * c, const char * msg) 
	{
		fprintf(stderr, "Error: %s\n", msg);
	}

	/* Print a message to stderr when a warning is issued */
	static void warning(wkhtmltopdf_converter * c, const char * msg)
	{
		fprintf(stderr, "Warning: %s\n", msg);
	}
	static void convert();
	~pdf_impl();
	
};
#endif  /* SERVER_HTTP_HPP */