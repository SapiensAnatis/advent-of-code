extern "C" {
#include "day02/day02.h"
#include "lib/file.h"
}

#include <cstdio>

#include "gtest/gtest.h"

TEST(day02part1, example) {
    FILE* example = get_example_input(2);

    day02_part1(example);
}

TEST(day02part1, real) {
    FILE* real = get_real_input(2);

    day02_part1(real);
}

TEST(day02part2, example) {
    FILE* example = get_example_input(2);

    day02_part2(example);
}

TEST(day02part2, real) {
    FILE* real = get_real_input(2);

    day02_part2(real);
}
