cmake_minimum_required(VERSION 3.8)
project(ControlUnit)

set(CMAKE_C_STANDARD 11)
#set (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra -Werror")

set(SOURCE_FILES main.c src/memory.c include/memory.h)
add_executable(ControlUnit ${SOURCE_FILES})