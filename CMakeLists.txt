cmake_minimum_required(VERSION 3.15)
project(DHBW_Kurs C)

set(CMAKE_C_STANDARD 99)
set(CMAKE_CXX_FLAGS -Wall -pedantic-errors)

add_executable(DHBW_Kurs main.c utils.h utils.c)