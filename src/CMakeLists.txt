cmake_minimum_required(VERSION 3.0)
project(adi)

find_package(MPI)
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_COMPILER mpicxx)

add_executable(adi main.cpp)
