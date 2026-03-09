#include "lib/string.h"

#include "lib/debug.h"
#include "lib/deleter.h"
#include "lib/fatal_error.h"
#include "lib/string_view.h"
#include "lib/vector.h"

#include <assert.h>
#include <stdlib.h>
#include <string.h>

static constexpr char NULL_TERMINATOR = '\0';

[[maybe_unused]] static bool string_is_null_terminated(const struct String* string) {
    const char* last_char = vector_at(string->char_buffer, vector_size(string->char_buffer) - 1);
    return *last_char == '\0';
}

struct String* string_create(const char* value) {
    struct String* result = malloc(sizeof(struct String));
    if (result == nullptr) {
        FATAL_ERROR("failed to allocate string");
    }

    result->char_buffer = vector_create(sizeof(char), DEFAULT_DELETER);

    vector_append_range(result->char_buffer, value, strlen(value));
    vector_append(result->char_buffer, &NULL_TERMINATOR);

    return result;
}

struct String* string_copy(const struct String* value) {
    // Bypass const; we aren't going to mutate and will just immediately copy
    const char* data = string_data((struct String*)value);
    return string_create(data);
}

void string_append(struct String* string, const char* value) {
    assert(string_is_null_terminated(string));
    vector_pop(string->char_buffer, nullptr); // Remove existing null terminator
    vector_append_range(string->char_buffer, value, strlen(value));
    vector_append(string->char_buffer, &NULL_TERMINATOR);
}

void string_trim_end(struct String* string, const char target) {
    // -2: last index (size - 1) is \0, last char is at (size - 2)
    assert(string_is_null_terminated(string));
    char* current_ending = vector_at(string->char_buffer, vector_size(string->char_buffer) - 2);

    DEBUG_PRINT("Trimming string. Current ending: %d, target for removal: %d", (int)*current_ending,
                (int)target);

    while (*current_ending == target) {
        DEBUG_PRINT("Removing one character");
        vector_pop(string->char_buffer, nullptr); // pop current null terminator
        vector_pop(string->char_buffer, nullptr); // pop character to trim
        vector_append(string->char_buffer,
                      &NULL_TERMINATOR); // re-insert null terminator where trimmed char was
    }
}

struct StringView string_to_string_view(struct String* string) {
    const struct StringView result = {.data = string_data(string), .length = string_length(string)};
    return result;
}

char* string_data(struct String* string) { return vector_data(string->char_buffer); }

size_t string_length(const struct String* string) { return vector_size(string->char_buffer); }

void string_free(struct String* string) {
    vector_free(string->char_buffer);
    free(string);
}
