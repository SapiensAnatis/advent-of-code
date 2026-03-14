//
// Created by jay on 06/03/2026.
//

#ifndef AOC2025_STRING_VIEW_H
#define AOC2025_STRING_VIEW_H

#include <stddef.h>
#include <stdint.h>

struct StringView {
    const char* data;
    size_t length;
};

char* string_view_to_string(const struct StringView* view);

ptrdiff_t string_view_find_char(const struct StringView* haystack, char needle, ptrdiff_t start);

bool string_view_try_parse_int32(const struct StringView* view, int32_t* out_result);

bool string_view_try_parse_int64(const struct StringView* view, int64_t* out_result);

bool string_view_try_parse_uint32(const struct StringView* view, uint32_t* out_result);

#endif // AOC2025_STRING_VIEW_H
