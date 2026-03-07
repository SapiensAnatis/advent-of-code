extern "C" {
#include "day03/day03.h"
#include "lib/file.h"
}

#include <cinttypes>
#include <cstdint>
#include <cstdio>

#include "gtest/gtest.h"

TEST(day03part1, example) {
    FILE* example = get_example_input(3);

    const int64_t answer = day03_part1(example);
    printf("answer = %" PRId64 "\n", answer);

    ASSERT_EQ(357, answer);
}

TEST(day03part1, real) {
    FILE* real = get_real_input(3);

    const int64_t answer = day03_part1(real);
    printf("answer = %" PRId64 "\n", answer);
}

TEST(day03part2, example) {
    FILE* example = get_example_input(3);

    const int64_t answer = day03_part2(example);
    printf("answer = %" PRId64 "\n", answer);

    ASSERT_EQ(3121910778619L, answer);
}

TEST(day03part2, real) {
    FILE* real = get_real_input(3);

    const int64_t answer = day03_part2(real);
    printf("answer = %" PRId64 "\n", answer);
}
