cmake_minimum_required(VERSION 3.8)
project(control_unit)

set(CMAKE_C_STANDARD 11)

set(SOURCE_FILES main.c src/memory.c include/memory.h src/control_unit.c include/control_unit.h src/arithmetic_logic_unit.c include/arithmetic_logic_unit.h)
add_executable(control_unit ${SOURCE_FILES})