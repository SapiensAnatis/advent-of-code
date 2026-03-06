extern "C" {
#include "day01/day01.h"
#include "lib/file.h"
}

#include "gtest/gtest.h"

#include <cstdio>

TEST(day01part1, example) {
    FILE* example = get_example_input(1);

    day01_part1(example);
}

TEST(day01part1, real) {
    FILE* real = get_real_input(1);

    day01_part1(real);
}

TEST(day01part2, example) {
    FILE* example = get_example_input(1);

    day01_part2(example);
}

TEST(day01part2, real) {
    FILE* real = get_real_input(1);

    day01_part2(real);
}

