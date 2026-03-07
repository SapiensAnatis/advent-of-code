extern "C" {
#include "day01/day01.h"
#include "lib/file.h"
}

#include "gtest/gtest.h"

#include <cstdint>
#include <cstdio>

TEST(day01part1, example) {
    FILE* example = get_example_input(1);

    const int32_t dial_zero_counter = day01_part1(example);
    printf("answer = %d", dial_zero_counter);

    ASSERT_EQ(3, dial_zero_counter);
}

TEST(day01part1, real) {
    FILE* real = get_real_input(1);

    const int32_t dial_zero_counter = day01_part1(real);
    printf("answer = %d", dial_zero_counter);
}

TEST(day01part2, example) {
    FILE* example = get_example_input(1);

    const int32_t dial_zero_counter = day01_part2(example);
    printf("answer = %d", dial_zero_counter);

    ASSERT_EQ(6, dial_zero_counter);
}

TEST(day01part2, real) {
    FILE* real = get_real_input(1);

    const int32_t dial_zero_counter = day01_part2(real);
    printf("answer = %d", dial_zero_counter);
}
