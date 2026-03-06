extern "C" {
#include "day03/day03.h"
#include "lib/file.h"
}

#include <cstdio>

#include "gtest/gtest.h"

TEST(day03part1, example) {
    FILE* example = get_example_input(3);

    day03_part1(example);
}

TEST(day03part1, real) {
    FILE* real = get_real_input(3);

    day03_part1(real);
}

TEST(day03part2, example) {
    FILE* example = get_example_input(3);

    day03_part2(example);
}

TEST(day03part2, real) {
    FILE* real = get_real_input(3);

    day03_part2(real);
}
