
set(SRC_LIST common/config.cpp common/log.cpp main.cpp)

set(MAIN_LIB_LIST boost_program_options boost_filesystem boost_coroutine boost_system boost_thread pthread boost_context boost_date_time boost_log_setup boost_log redispp hiredis ssl crypto boost_regex boost_timer boost_signals boost_locale)

add_executable(redis.fcgi ${SRC_LIST})
set_target_properties(redis.fcgi PROPERTIES COMPILE_FLAGS "-std=c++14")
target_link_libraries(redis.fcgi ${MAIN_LIB_LIST})

