cmake_minimum_required(VERSION 3.21)
project(PROJETO C)

set(CMAKE_C_STANDARD 99)
set(SOURCE_FILES main.c utils.h utils.c matdin.h matdin.c matdin3d.h matdin3d.c jogofunc.h jogofunc.c list.h list.c)

add_executable(PROJETO ${SOURCE_FILES})
