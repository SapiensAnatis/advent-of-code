//
// Created by jay on 07/03/2026.
//

#ifndef AOC2025_LIB_RANGE_H
#define AOC2025_LIB_RANGE_H

#include <stdint.h>

/**
 * Represents a numeric range. Start and end are inclusive.
 */
struct Range {
    int64_t start;
    int64_t end;
};

bool range_test(const struct Range* range, int64_t value);

bool range_try_merge(const struct Range* first, const struct Range* second, struct Range* out_merged);

#endif // AOC2025_LIB_RANGE_H
