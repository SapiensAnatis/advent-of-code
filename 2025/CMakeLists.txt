cmake_minimum_required(VERSION 3.30)
project(aoc2025)

enable_testing()

include(FetchContent)

FetchContent_Declare(
        googletest
        URL https://github.com/google/googletest/archive/f3c355f9dd382bc2c323be2713e351a578b68c61.zip
)

FetchContent_MakeAvailable(googletest)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

set(CMAKE_C_STANDARD 23)
set(CMAKE_C_STANDARD_REQUIRED ON)
set(CMAKE_C_CLANG_TIDY "clang-tidy;-config-file=${CMAKE_CURRENT_SOURCE_DIR}/.clang-tidy;-warnings-as-errors=*")
set(CMAKE_C_FLAGS "-Wall -Wextra -Wpedantic -Werror -Wno-gnu-zero-variadic-macro-arguments -fsanitize=address")
set(CMAKE_C_FLAGS_DEBUG "-Og -g")
add_link_options("-fsanitize=address")

# C++ flags for GTest
set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_CLANG_TIDY "clang-tidy;-config-file=${CMAKE_CURRENT_SOURCE_DIR}/.clang-tidy;-warnings-as-errors=*")
set(CMAKE_CXX_FLAGS "-Wall -Wextra -Wpedantic -Werror")
set(CMAKE_CXX_FLAGS_DEBUG "-Og -g")

include_directories("${CMAKE_CURRENT_LIST_DIR}")

add_executable(aoc)

target_link_libraries(aoc gtest_main)

add_subdirectory("lib")

target_link_libraries(aoc aoc_lib)

add_subdirectory("day01")

include(GoogleTest)
gtest_discover_tests(aoc)

add_subdirectory("day02")
add_subdirectory("day03")
