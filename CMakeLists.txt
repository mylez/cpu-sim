cmake_minimum_required(VERSION 3.10)
project(computer C)

set(CMAKE_C_STANDARD 99)

add_executable(computer main.c control.c control.h isa.h cpu.c cpu.h register_file.c register_file.h util.c util.h memory.c memory.h)