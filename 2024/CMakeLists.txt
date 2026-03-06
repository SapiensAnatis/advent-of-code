cmake_minimum_required(VERSION 3.30)
project(aoc2024)

set(CMAKE_CXX_STANDARD 20)

include(FetchContent)

FetchContent_Declare(
        googletest
        URL https://github.com/google/googletest/archive/f3c355f9dd382bc2c323be2713e351a578b68c61.zip
)

FetchContent_MakeAvailable(googletest)

find_package(Boost 1.83.0)
include_directories(${Boost_INCLUDE_DIR})

# https://gitlab.kitware.com/cmake/cmake/-/issues/26283
set(CMAKE_CXX_SCAN_FOR_MODULES OFF)
set(CMAKE_CXX_CLANG_TIDY "clang-tidy;-config-file=${CMAKE_CURRENT_SOURCE_DIR}/.clang-tidy;-warnings-as-errors=*")
add_compile_options(-Wall -Wextra -Wpedantic -Werror)

add_executable(aoc2024)
target_link_libraries(aoc2024 ${Boost_LIBRARIES} gtest_main)

add_subdirectory("lib")
add_subdirectory("day01")
add_subdirectory("day02")
add_subdirectory("day03")
add_subdirectory("day04")
add_subdirectory("day05")
add_subdirectory("day06")
add_subdirectory("day07")
add_subdirectory("day08")
add_subdirectory("day09")
add_subdirectory("day10")
add_subdirectory("day11")
add_subdirectory("day12")
add_subdirectory("day13")
add_subdirectory("day14")
add_subdirectory("day15")
add_subdirectory("day16")
add_subdirectory("day17")
add_subdirectory("day18")
add_subdirectory("day19")
add_subdirectory("day20")
add_subdirectory("day21")
add_subdirectory("day22")
add_subdirectory("day23")
add_subdirectory("day24")
add_subdirectory("day25")
add_subdirectory("perf")
