cmake_minimum_required(VERSION 3.6)
project(winserver)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=gnu++11 -lws2_32")

set(SOURCE_FILES winserver.cpp)
add_executable(winserver ${SOURCE_FILES})

target_link_libraries(winserver ws2_32)