cmake_minimum_required(VERSION 3.15)
project(DHBW_Kurs C)

set(CMAKE_C_STANDARD 99)
set(GCC_COVERAGE_COMPILE_FLAGS "-Wall -pedantic-errors")
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} ${GCC_COVERAGE_COMPILE_FLAGS}")

add_executable(DHBW_Kurs main.c utils.h utils.c load.c)