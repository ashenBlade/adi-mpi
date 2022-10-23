cmake_minimum_required(VERSION 3.23)
project(spline)

find_package(MPI)
set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_COMPILER mpicxx)

add_executable(spline main.cpp)
