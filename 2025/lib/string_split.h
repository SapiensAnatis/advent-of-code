#ifndef AOC2025_LIB_STRING_SPLIT_H
#define AOC2025_LIB_STRING_SPLIT_H

#include <stddef.h>
#include <stdint.h>

#include "lib/string_view.h"

typedef uint8_t ss_option_bitmask;

enum StringSplitOptions : ss_option_bitmask {
    STRING_SPLIT_DEFAULT = 0,
    STRING_SPLIT_REMOVE_EMPTY_ENTRIES = 1
};

struct StringSplitIterator {
    struct StringView source_string;
    const char* separator;
    size_t current_position;
    struct StringView current_segment;
    ss_option_bitmask options;
};

struct StringSplitIterator string_split_create(const char* string, const char* separator,
                                               ss_option_bitmask options);

bool string_split_move_next(struct StringSplitIterator* iter);

#endif // AOC2025_LIB_STRING_SPLIT_H
