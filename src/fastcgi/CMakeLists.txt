string(TIMESTAMP BUILD_TIME UTC)

option(BUILD_STATIC_LIBS "Set to on to build and install static library" OFF)

# Set up our log level for fastcgi++/log.hpp
if(NOT LOG_LEVEL)
    if(CMAKE_BUILD_TYPE STREQUAL "RELEASE")
        set(LOG_LEVEL 1)
    elseif(CMAKE_BUILD_TYPE STREQUAL "DEBUG")
        set(LOG_LEVEL 3)
    else()
        set(LOG_LEVEL 2)
    endif()
endif()

set(SYSTEM "LINUX")

include_directories(include)
# Our configuration 
#configure_file("include/config.hpp.in"  "include/fastcgi++/config.hpp" COPYONLY)

# The shared library itself
#add_library(fastcgipp SHARED
 #   src/log.cpp
  #  src/http.cpp
   # src/protocol.cpp
   # src/sockets.cpp
   # src/transceiver.cpp
   # src/fcgistreambuf.cpp
   # src/webstreambuf.cpp
   # src/request.cpp
   # src/manager.cpp)
add_library(fastcgipp STATIC
	src/log.cpp
	src/http.cpp
	src/protocol.cpp
	src/sockets.cpp
	src/transceiver.cpp
	src/fcgistreambuf.cpp
	src/webstreambuf.cpp
	src/request.cpp
	src/manager.cpp)

set(VERSION      "3.0beta")
set(VERSION_MAJOR 3)
string(TIMESTAMP BUILD_TIME UTC)

set_target_properties(fastcgipp PROPERTIES VERSION ${VERSION}
                                           SOVERSION ${VERSION_MAJOR})

configure_file(${PROJECT_SOURCE_DIR}/src/fastcgi/include/config.hpp.in ${PROJECT_SOURCE_DIR}/src/fastcgi/include/fastcgi++/config.hpp @ONLY)


add_executable(helloworld.fcgi helloworld.cpp)
add_dependencies(helloworld.fcgi fastcgipp)
set(MAIN_LIB_LIST boost_program_options boost_filesystem boost_coroutine boost_system boost_thread pthread boost_context boost_date_time boost_log_setup boost_log redispp hiredis ssl crypto boost_regex boost_timer boost_signals boost_locale libfastcgipp.a)
target_link_libraries(helloworld.fcgi ${MAIN_LIB_LIST} )









