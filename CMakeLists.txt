cmake_minimum_required(VERSION 2.8)

project(np)

include_directories(./)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} --std=c++11")

add_library(np STATIC np.cpp np.h)

add_executable(libtest libtest.cpp  buffered_reader.cpp buffered_reader.h)
target_link_libraries(libtest np)

add_subdirectory(echo)
add_subdirectory(udp)
