#ifndef LOG_HPP
#define LOG_HPP

#include <muduo/base/AsyncLogging.h>
#include <muduo/base/Logging.h>
#include <muduo/base/Timestamp.h>
#include <muduo/base/Logging.h>
#include <muduo/base/LogFile.h>
#include <muduo/base/ThreadPool.h>
#include <muduo/base/TimeZone.h>

#include <stdio.h>
#include <sys/resource.h>
#include "config.hpp"

void asyncOutput(const char* msg, int len);

void bench(bool longLog);

void init_log();

#endif