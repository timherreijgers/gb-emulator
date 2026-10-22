cmake_minimum_required(VERSION 4.3)

if (CMAKE_VERSION VERSION_GREATER_EQUAL "4.4.0")
    set(CMAKE_EXPERIMENTAL_CXX_IMPORT_STD "f35a9ac6-8463-4d38-8eec-5d6008153e7d")
elseif (CMAKE_VERSION VERSION_GREATER_EQUAL "4.3.0")
    set(CMAKE_EXPERIMENTAL_CXX_IMPORT_STD "451f2fe2-a8a2-47c3-bc32-94786d8fc91b")
endif ()

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wno-reserved-module-identifier")

message(STATUS "CMake version: ${CMAKE_VERSION}")
message(STATUS "import std gate: ${CMAKE_EXPERIMENTAL_CXX_IMPORT_STD}")

project(gb_emulator VERSION 0.0.1)

add_compile_options(-Wall -Wextra -Wpedantic -Werror -Wsuggest-override -Wshadow -Wswitch-enum -Wswitch-default -Wconversion -Wno-include-angled-in-module-purview)
enable_testing()

find_package(GTest REQUIRED)
find_package(benchmark REQUIRED)

include(cmake/ccache.cmake)

add_subdirectory(emulatorlib)
add_subdirectory(utilitylib)

add_executable(gb_emulator main.cpp)

target_link_libraries(gb_emulator PRIVATE emulatorlib)