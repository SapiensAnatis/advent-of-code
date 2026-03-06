#ifndef AOC2025_LIB_STRING_SPLIT_H
#define AOC2025_LIB_STRING_SPLIT_H

#include <stddef.h>

#include "lib/string_view.h"

struct StringSplitIterator {
    struct StringView source_string;
    const char* separator;
    size_t current_position;
    struct StringView current_segment;
};

struct StringSplitIterator* string_split_create(const char* string, const char* separator);

bool string_split_move_next(struct StringSplitIterator* iter);

void string_split_free(struct StringSplitIterator* iter);

#endif // AOC2025_LIB_STRING_SPLIT_H
