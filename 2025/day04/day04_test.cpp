extern "C" {
#include "day04/day04.h"
#include "lib/file.h"
}

#include <cstdio>

#include "gtest/gtest.h"

TEST(day04part1, example) {
    FILE* example = get_example_input(4);

    day04_part1(example);
}

TEST(day04part1, real) {
    FILE* example = get_real_input(4);

    day04_part1(example);
}

TEST(day04part2, example) {}

TEST(day04part2, real) {}
