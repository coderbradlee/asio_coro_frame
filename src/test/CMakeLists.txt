add_executable(test main.cpp)
set(MAIN_LIB_LIST boost_system  boost_program_options boost_filesystem boost_coroutine boost_thread pthread boost_context boost_date_time boost_regex boost_timer boost_signals boost_locale)

set_target_properties(test PROPERTIES COMPILE_FLAGS "-std=c++14")

target_link_libraries(test ${MAIN_LIB_LIST})
