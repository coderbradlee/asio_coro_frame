#ifndef FASTCGI_HPP
#define FASTCGI_HPP
#ifndef lint
static const char rcsid[] = "$Id: threaded.c,v 1.9 2001/11/20 03:23:21 robs Exp $";
#endif /* not lint */

#include "fcgi_config.h"

#include <pthread.h>
#include <sys/types.h>

#ifdef HAVE_UNISTD_H
#include <unistd.h>
#endif

#include "fcgiapp.h"

#include <string>
#include <set>
#include <vector>
#include <exception>
#include <fstream>
#include <iostream>

#include <cstdlib>

#define THREAD_COUNT 30

using namespace std;

string served_contents="served_contents";

void doit();


#endif  /* SERVER_HTTP_HPP */