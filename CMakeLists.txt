cmake_minimum_required(VERSION 3.8)
project(control_unit)

set(CMAKE_C_STANDARD 11)

set(SOURCE_FILES main.c src/memory.c include/memory.h src/control_unit.c include/control_unit.h src/arithmetic_logic_unit.c include/arithmetic_logic_unit.h include/cache.h include/cache_associative.h src/cache_associative.c src/cache_direct.c include/cache_direct.h)
add_executable(control_unit ${SOURCE_FILES})