extern "C" {
#include "day08/day08.h"
#include "lib/file.h"
}

#include <cstdint>
#include <cstdio>

#include "gtest/gtest.h"

TEST(day08part1, example) {
    FILE* example = get_example_input(8);

    const uint32_t answer = day08_part1(example, true);
    printf("answer = %d", answer);

    ASSERT_EQ(40, answer);
}

TEST(day08part1, real) {
    FILE* real = get_real_input(8);

    const uint32_t answer = day08_part1(real, false);
    printf("answer = %d", answer);
}

TEST(day08part2, example) {}

TEST(day08part2, real) {}
