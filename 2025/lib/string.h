#ifndef AOC2025_LIB_STRING_H
#define AOC2025_LIB_STRING_H

#include "lib/string_view.h"

#include <stddef.h>

struct String {
    struct Vector* char_buffer;
};

struct String* string_create(const char* value);

void string_append(struct String* string, const char* value);

char* string_data(struct String* string);

size_t string_length(const struct String* string);

void string_trim_end(struct String* string, char target);

struct StringView string_to_string_view(const struct String* string);

void string_free(struct String* string);

#endif // AOC2025_LIB_STRING_H
