#include <cinttypes>
extern "C" {
#include "day07/day07.h"
#include "lib/file.h"
}

#include <cstdint>
#include <cstdio>

#include "gtest/gtest.h"

TEST(day07part1, example) {
    FILE* example = get_example_input(7);

    const int64_t answer = day07_part1(example);
    printf("answer = %" PRIi64 "\n", answer);

    ASSERT_EQ(21, answer);
}

TEST(day07part1, real) {
    FILE* real = get_real_input(7);

    const int64_t answer = day07_part1(real);
    printf("answer = %" PRIi64 "\n", answer);
}

TEST(day07part2, example) {
    FILE* example = get_example_input(7);

    const uint64_t answer = day07_part2(example);
    printf("answer = %" PRIu64 "\n", answer);

    ASSERT_EQ(40, answer);
}

TEST(day07part2, real) {
    FILE* real = get_real_input(7);

    const uint64_t answer = day07_part2(real);
    printf("answer = %" PRIu64 "\n", answer);
}
