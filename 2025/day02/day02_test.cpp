extern "C" {
#include "day02/day02.h"
#include "lib/file.h"
}

#include <cinttypes>
#include <cstdint>
#include <cstdio>

#include "gtest/gtest.h"

TEST(day02part1, example) {
    FILE* example = get_example_input(2);

    const int64_t result = day02_part1(example);
    printf("answer = %" PRId64 "\n", result);

    ASSERT_EQ(1227775554L, result);
}

TEST(day02part1, real) {
    FILE* real = get_real_input(2);

    const int64_t result = day02_part1(real);
    printf("answer = %" PRId64 "\n", result);
}

TEST(day02part2, example) {
    FILE* example = get_example_input(2);

    const int64_t result = day02_part2(example);
    printf("answer = %" PRId64 "\n", result);

    ASSERT_EQ(4174379265, result);
}

TEST(day02part2, real) {
    FILE* real = get_real_input(2);

    const int64_t result = day02_part2(real);
    printf("answer = %" PRId64 "\n", result);
}
