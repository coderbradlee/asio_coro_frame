
add_subdirectory(common)

add_subdirectory(test)
include_directories(common)
add_executable(redis_cluster main.cpp)
set(MAIN_LIB_LIST boost_program_options boost_filesystem boost_coroutine boost_system boost_thread pthread boost_context boost_date_time boost_log_setup boost_log redispp hiredis)

set_target_properties(redis_cluster PROPERTIES COMPILE_FLAGS "-std=c++14")

target_link_libraries(redis_cluster ${MAIN_LIB_LIST})
