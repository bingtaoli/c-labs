cmake_minimum_required (VERSION 2.6)
project (epoll)

set(CMAKE_CXX_FLAGS ${CMAKE_CXX_FLAGS} "-g -O2 -Wall")
add_executable(epoll "*.cc")
