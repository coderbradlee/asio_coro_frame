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
configure_file(
    "include/config.hpp.in"
    "fastcgi++/config.hpp" @ONLY)

# The shared library itself
add_library(fastcgipp SHARED
    src/log.cpp
    src/http.cpp
    src/protocol.cpp
    src/sockets.cpp
    src/transceiver.cpp
    src/fcgistreambuf.cpp
    src/webstreambuf.cpp
    src/request.cpp
    src/manager.cpp)
set_target_properties(fastcgipp PROPERTIES VERSION ${VERSION}
                                           SOVERSION ${VERSION_MAJOR})
