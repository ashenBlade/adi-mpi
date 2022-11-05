cmake_minimum_required(VERSION 3.23)
project(hw4 C)

find_package(MPI)
set(CMAKE_C_COMPILER mpicc)
set(CMAKE_C_STANDARD 17)

add_executable(hw4 main.c)
