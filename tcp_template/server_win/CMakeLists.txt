cmake_minimum_required(VERSION 2.8)
project(server_win)
if(CMAKE_COMPILER_IS_GNUCXX)
    message(STATUS "GCC detected, adding compile flags")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++98 -pedantic -Wall -Wextra")
endif(CMAKE_COMPILER_IS_GNUCXX)

set(SOURCE_FILES main.c)
add_executable(server_win ${SOURCE_FILES})