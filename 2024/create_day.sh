#!/usr/bin/bash

# And they say C# has boilerplate...

day=$1 

if [ -z "$day" ]; then
    echo "Usage: ./create_day.sh <day_number>"
    exit 1
fi

day_name=$(printf "day%02d" "$day")
mkdir -p "$day_name"

touch "./${day_name}/input.txt"
touch "./${day_name}/example.txt"

# We don't care as much about overwriting this file if it already exists as it's auto-generated boilerplate
tee "./${day_name}/CMakeLists.txt" <<EOF > /dev/null
configure_file("\${CMAKE_CURRENT_SOURCE_DIR}/example.txt" "\${CMAKE_CURRENT_BINARY_DIR}/example.txt" COPYONLY)

if (EXISTS "\${CMAKE_CURRENT_SOURCE_DIR}/input.txt")
    configure_file("\${CMAKE_CURRENT_SOURCE_DIR}/input.txt" "\${CMAKE_CURRENT_BINARY_DIR}/input.txt" COPYONLY)
endif ()

target_sources(aoc2024 PRIVATE
    ${day_name}.cpp
    ${day_name}.h
    ${day_name}_test.cpp
)
EOF

cpp_file_name="./${day_name}/${day_name}.cpp"
if [ ! -f "$cpp_file_name" ]; then
  tee "$cpp_file_name" <<EOF > /dev/null
#include "${day_name}.h"

namespace ${day_name} {
} // namespace ${day_name}
EOF
fi

h_file_name="./${day_name}/${day_name}.h"
if [ ! -f "$h_file_name" ]; then
  include_guard="AOC2024_${day_name^^}_H"
  tee "$h_file_name" <<EOF > /dev/null
#ifndef ${include_guard}
#define ${include_guard}

namespace ${day_name} {
} // namespace ${day_name}

#endif // ${include_guard}
EOF
fi

test_file_name="./${day_name}/${day_name}_test.cpp"
if [ ! -f "$test_file_name" ]; then
  tee "$test_file_name" <<EOF > /dev/null
#include "../lib/aoc.h"
#include "${day_name}.h"
#include "gtest/gtest.h"

TEST(${day_name}_part1, example) {
}

TEST(${day_name}_part1, real) {
}

TEST(${day_name}_part2, example) {
}

TEST(${day_name}_part2, real) {
}
EOF
fi

if ! grep -q "${day_name}" "CMakeLists.txt" ; then
    echo "add_subdirectory(\"${day_name}\")" >> CMakeLists.txt
fi