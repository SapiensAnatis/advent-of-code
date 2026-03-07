extern "C" {
#include "day04/day04.h"
#include "lib/file.h"
}

#include <cstdint>
#include <cstdio>

#include "gtest/gtest.h"

TEST(day04part1, example) {
    FILE* example = get_example_input(4);

    const int32_t answer = day04_part1(example);
    printf("answer = %d\n", answer);

    ASSERT_EQ(13, answer);
}

TEST(day04part1, real) {
    FILE* real = get_real_input(4);

    const int32_t answer = day04_part1(real);
    printf("answer = %d\n", answer);
}

TEST(day04part2, example) {}

TEST(day04part2, real) {}
