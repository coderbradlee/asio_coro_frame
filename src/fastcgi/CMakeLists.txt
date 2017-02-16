include_directories(../common)
include_directories(wkhtmltox/include)
set(SRC_LIST ../common/config.cpp ../common/log.cpp redis_api.cpp pdf_api.cpp router.cpp main.cpp)

set(MAIN_LIB_LIST boost_program_options boost_filesystem boost_coroutine boost_system boost_thread pthread boost_context boost_date_time boost_log_setup boost_log redispp hiredis ssl crypto boost_regex boost_timer boost_signals boost_locale fcgi++ fcgi wkhtmltox)

add_executable(all.fcgi ${SRC_LIST})
set_target_properties(all.fcgi PROPERTIES COMPILE_FLAGS "-std=c++14")
target_link_libraries(all.fcgi ${MAIN_LIB_LIST})

