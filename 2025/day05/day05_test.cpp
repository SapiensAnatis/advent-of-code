extern "C" {
#include "day05/day05.h"
#include "lib/file.h"
}

#include <cinttypes>
#include <cstdint>
#include <cstdio>

#include "gtest/gtest.h"

TEST(day05part1, example) {
    FILE* example = get_example_input(5);

    const int32_t answer = day05_part1(example);
    printf("answer = %d\n", answer);

    ASSERT_EQ(3, answer);
}

TEST(day05part1, real) {
    FILE* real = get_real_input(5);

    const int32_t answer = day05_part1(real);
    printf("answer = %d\n", answer);
}

TEST(day05part2, example) {
    FILE* example = get_example_input(5);

    const int64_t answer = day05_part2(example);
    printf("answer = %" PRId64 "\n", answer);

    ASSERT_EQ(14, answer);
}

TEST(day05part2, real) {
    FILE* real = get_real_input(5);

    const int64_t answer = day05_part2(real);
    printf("answer = %" PRId64 "\n", answer);
}