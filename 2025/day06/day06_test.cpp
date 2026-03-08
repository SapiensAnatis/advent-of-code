extern "C" {
#include "day06/day06.h"
#include "lib/file.h"
}

#include <cinttypes>
#include <cstdint>
#include <cstdio>

#include "gtest/gtest.h"

TEST(day06part1, example) {
    FILE* example = get_example_input(6);

    const int64_t answer = day06_part1(example);
    printf("answer = %" PRIi64 "\n", answer);

    ASSERT_EQ(4277556, answer);
}

TEST(day06part1, real) {
    FILE* real = get_real_input(6);

    const int64_t answer = day06_part1(real);
    printf("answer = %" PRIi64 "\n", answer);
}

TEST(day06part2, example) {}

TEST(day06part2, real) {}
