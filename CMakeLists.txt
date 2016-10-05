cmake_minimum_required(VERSION 3.6)
project(project2430)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

set(SOURCE_FILES main.c parse.h raycaster.h)
add_executable(project2430 ${SOURCE_FILES})