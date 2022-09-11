cmake_minimum_required(VERSION 3.23)
project(c C)

set(CMAKE_C_STANDARD 11)

add_executable(cpp
        main.c
        matrix.c
        matrix.h
        measurements.c
        measurements.h)
