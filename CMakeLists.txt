cmake_minimum_required(VERSION 3.23)
project(myproject)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -fpermissive -g")

add_executable(myproject main.cpp process.h)
