cmake_minimum_required(VERSION 3.23)
project(parallel_programming)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++14 -fopenmp")

find_package(MPI)

set(CMAKE_CXX_COMPILER mpicxx)

add_executable(${PROJECT_NAME}
        main.cpp)

