//
// Created by jay on 06/03/2026.
//

#ifndef AOC2025_STRING_VIEW_H
#define AOC2025_STRING_VIEW_H

#include <stddef.h>

struct StringView {
    const char* data;
    size_t length;
};

char* string_view_to_string(const struct StringView* view);

bool try_parse_int(const struct StringView* view, int* out_result);

bool try_parse_long(const struct StringView* view, long* out_result) ;

#endif // AOC2025_STRING_VIEW_H
