# And they say C# has boilerplate...

set -euo pipefail

if [ $# != 1 ]; then
    echo "Usage: ./create_day.sh <day_number>"
    exit 1
fi

day=$1

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

target_sources(aoc2025 PRIVATE
    ${day_name}.c
    ${day_name}.h
    ${day_name}_test.cpp
)
EOF

c_file_name="./${day_name}/${day_name}.c"
if [ ! -f "$c_file_name" ]; then
  tee "$c_file_name" <<EOF > /dev/null
#include "${day_name}/${day_name}.h"

EOF
fi

h_file_name="./${day_name}/${day_name}.h"
if [ ! -f "$h_file_name" ]; then
  include_guard="AOC2025_${day_name^^}_H"
  tee "$h_file_name" <<EOF > /dev/null
#ifndef ${include_guard}
#define ${include_guard}

#endif // ${include_guard}
EOF
fi

test_file_name="./${day_name}/${day_name}_test.cpp"
if [ ! -f "$test_file_name" ]; then
  tee "$test_file_name" <<EOF > /dev/null
extern "C" {
#include "${day_name}/${day_name}.h"
#include "lib/file.h"
}

#include <cstdio>

#include "gtest/gtest.h"

TEST(${day_name}part1, example) {
}

TEST(${day_name}part1, real) {
}

TEST(${day_name}part2, example) {
}

TEST(${day_name}part2, real) {
}
EOF
fi

if ! grep -q "${day_name}" "CMakeLists.txt" ; then
    echo "add_subdirectory(\"${day_name}\")" >> CMakeLists.txt
fi
