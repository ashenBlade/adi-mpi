cmake_minimum_required(VERSION 3.23)
project(parallel_programming)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++14 -fopenmp")
set(CMAKE_CXX_COMPILER "/usr/bin/mpic++")


add_executable(parallel_programming
        main.cpp)

target_include_directories(parallel_programming
        PUBLIC /usr/include/mpi)
