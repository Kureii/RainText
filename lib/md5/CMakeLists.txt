set(md5_SOURCE md5.cpp md5.h)

add_library(md5_lib STATIC ${md5_SOURCE})
target_include_directories(md5_lib PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
