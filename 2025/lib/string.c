#include "lib/string.h"

#include "lib/debug.h"
#include "lib/vector.h"

#include <assert.h>
#include <stdlib.h>
#include <string.h>



static constexpr char NULL_TERMINATOR = '\0';

static bool string_is_null_terminated(const struct String* string) {
    char* last_char = vector_at(string->char_buffer, vector_size(string->char_buffer) - 1);
    return *last_char == '\0';
}

struct String* string_create(const char* value) {
    struct String* result = malloc(sizeof(struct String));
    assert(result != nullptr && "failed to allocate string");

    result->char_buffer = vector_create(sizeof(char));

    vector_append_range(result->char_buffer, value, strlen(value));
    vector_append(result->char_buffer, &NULL_TERMINATOR);

    return result;
}

void string_append(struct String* string, const char* value) {
    assert(string_is_null_terminated(string));
    vector_pop(string->char_buffer); // Remove existing null terminator
    vector_append_range(string->char_buffer, value, strlen(value));
    vector_append(string->char_buffer, &NULL_TERMINATOR);
}

void string_trim_end(struct String* string, char target) {
    // -2: last index (size - 1) is \0, last char is at (size - 2)
    assert(string_is_null_terminated(string));
    char* current_ending = vector_at(string->char_buffer, vector_size(string->char_buffer) - 2);

    DEBUG_PRINT("Trimming string. Current ending: %d, target for removal: %d", (int)*current_ending,
                (int)target);

    while (*current_ending == target) {
        DEBUG_PRINT("Removing one character");
        vector_pop(string->char_buffer); // pop current null terminator
        vector_pop(string->char_buffer); // pop character to trim
        vector_append(string->char_buffer,
                      &NULL_TERMINATOR); // re-insert null terminator where trimmed char was
    }
}

char* string_data(struct String* string) { return vector_data(string->char_buffer); }

size_t string_length(const struct String* string) { return vector_size(string->char_buffer); }

void string_free(struct String* string) {
    vector_free(string->char_buffer);
    free(string);
}
