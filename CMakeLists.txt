cmake_minimum_required(VERSION 3.8)
project(httpd)

set(CMAKE_CXX_STANDARD 11)

set(SOURCE_FILES main.cpp)
add_executable(httpd ${SOURCE_FILES})