cmake_minimum_required(VERSION 3.8)
project(ControlUnit)

set(CMAKE_C_STANDARD 11)
#set (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra -Werror")

set(SOURCE_FILES main.c src/memory.c include/memory.h src/control_unit.c include/control_unit.h)
add_executable(ControlUnit ${SOURCE_FILES})