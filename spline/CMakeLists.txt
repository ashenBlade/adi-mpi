cmake_minimum_required(VERSION 3.23)
project(spline)

find_package(MPI)
set(CMAKE_C_STANDARD 11)
set(CMAKE_C_COMPILER mpicc)

add_executable(spline main.c matrix_multiply.c)
