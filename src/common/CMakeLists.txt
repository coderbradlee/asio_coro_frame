
set(redis_SRCS redispp.cpp)
add_library(redispp ${redis_SRCS})

install(TARGETS redispp DESTINATION lib)