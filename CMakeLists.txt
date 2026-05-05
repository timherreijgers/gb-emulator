cmake_minimum_required(VERSION 4.2)

project(gb_emulator VERSION 0.0.1)

add_compile_options(-Wall -Wextra -Wpedantic -Werror)
enable_testing()

find_package(GTest REQUIRED)
find_package(benchmark REQUIRED)

include(cmake/ccache.cmake)

add_executable(gb_emulator main.cpp)
