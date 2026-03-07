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
set(CMAKE_C_FLAGS "-Wall -Wextra -Wpedantic -Werror")
set(CMAKE_C_FLAGS_DEBUG "-Og -g")

# C++ flags for GTest
set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_SCAN_FOR_MODULES OFF)
set(CMAKE_CXX_CLANG_TIDY "clang-tidy;-config-file=${CMAKE_CURRENT_SOURCE_DIR}/.clang-tidy;-warnings-as-errors=*")
set(CMAKE_CXX_FLAGS "-Wall -Wextra -Wpedantic -Werror")
set(CMAKE_CXX_FLAGS_DEBUG "-Og -g")

include_directories("${CMAKE_CURRENT_LIST_DIR}")

add_executable(aoc2025 asan_options.c)

target_link_libraries(aoc2025 gtest_main)
target_link_libraries(aoc2025 hardening)

add_subdirectory("lib")

target_link_libraries(aoc2025 aoc2025_lib)

include(GoogleTest)
gtest_discover_tests(aoc2025)

add_subdirectory("day01")
add_subdirectory("day02")
add_subdirectory("day03")
add_subdirectory("day04")
