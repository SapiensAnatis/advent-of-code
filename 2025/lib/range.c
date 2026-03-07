#include "lib/range.h"

#include "lib/debug.h"

#include <inttypes.h>
#include <stdint.h>

/**
 * Test if a value is in a range.
 * @param range The range to test against.
 * @param value The value to test.
 * @return true if the value is in the range, otherwise false.
 */
bool range_test(const struct Range* range, const int64_t value) {
    return value >= range->start && value <= range->end;
}

/**
 * Try to merge two ranges into a single range.
 * @param first The first range.
 * @param second The second range.
 * @param out_merged Destination to write the output range to, if successful.
 * @return A bool indicating whether the ranges could be merged into a single range.
 */
bool range_try_merge(const struct Range* first, const struct Range* second,
                     struct Range* out_merged) {
    const bool adjacent = first->end + 1 == second->start || second->end + 1 == first->start;
    const bool overlap = range_test(first, second->start) || range_test(second, first->start) ||
                         range_test(first, second->end) || range_test(second, first->end);

    if (overlap || adjacent) {
        struct Range result = {0};

        result.start = first->start < second->start ? first->start : second->start;
        result.end = first->end > second->end ? first->end : second->end;
        *out_merged = result;
        return true;
    }

    return false;
}

void range_debug_print([[maybe_unused]] const struct Range* range) {
    DEBUG_PRINT("%" PRId64 "-%" PRId64 "\n", range->start, range->end);
}