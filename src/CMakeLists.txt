
add_subdirectory(common)

add_subdirectory(test)
add_subdirectory(tools/all_report)
add_subdirectory(tools/excel_to_mysql)
include_directories(common)
add_executable(redis_cluster 
	common/config.cpp 
	common/log.cpp 
	common/connection.cpp
    common/connection_manager.cpp
    common/mime_types.cpp
    common/reply.cpp
    common/request_handler.cpp
    common/request_parser.cpp
    common/server.cpp
    main.cpp)
set(MAIN_LIB_LIST boost_program_options boost_filesystem boost_coroutine boost_system boost_thread pthread boost_context boost_date_time boost_log_setup boost_log redispp hiredis ssl crypto boost_regex boost_timer boost_signals boost_locale)

 

set_target_properties(redis_cluster PROPERTIES COMPILE_FLAGS "-std=c++14")

target_link_libraries(redis_cluster ${MAIN_LIB_LIST})
