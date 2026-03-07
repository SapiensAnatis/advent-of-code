#include "lib/string_view.h"

#include "debug.h"

#include <assert.h>
#include <ctype.h>
#include <limits.h>
#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

char* string_view_to_string(const struct StringView* view) {
    // String views are not guaranteed to be null terminated, so conversion to a C string requires
    // making a copy
    char* buffer = malloc(view->length + 1);
    if (buffer == nullptr) {
        assert(false && "failed to allocate string");
        abort();
    }
    memcpy(buffer, view->data, view->length);
    buffer[view->length] = '\0';

    return buffer;
}

/**
 * Find a character in a string view.
 * @param haystack The input view.
 * @param needle The character to search for.
 * @param start Offset index at which to start the search.
 * @return The index at which the character was found, or -1 if it was not found.
 */
ptrdiff_t string_view_find_char(const struct StringView* haystack, const char needle,
                                const ptrdiff_t start) {
    if (haystack->length >= PTRDIFF_MAX) {
        assert(false && "string view too long");
    }

    if (start >= (ptrdiff_t)haystack->length) {
        assert(false && "start >= haystack.length");
        return -1;
    }

    for (size_t i = start; i < haystack->length; i++) {
        if (haystack->data[i] == needle) {
            return (ptrdiff_t)i;
        }
    }

    return -1;
}

/**
 * Tries to parse a StringView to an int32_t.
 * @param view The input view.
 * @param out_result Pointer to result. Will not be written to if this returns false.
 * @return A boolean indicating whether the parsing succeeded or not.
 */
bool try_parse_int(const struct StringView* view, int32_t* out_result) {
    long tmp_result = 0;
    const bool result = try_parse_long(view, &tmp_result);
    if (result && tmp_result <= INT_MAX && tmp_result >= INT_MIN) {
        *out_result = (int)tmp_result;
        return true;
    }

    return false;
}

/**
 * Tries to parse a StringView to an int64_t.
 * @param view The input view.
 * @param out_result Pointer to result. Will not be written to if this returns false.
 * @return A boolean indicating whether the parsing succeeded or not.
 */
bool try_parse_long(const struct StringView* view, int64_t* out_result) {
    long result = 0;

    for (size_t i = 0; i < view->length; i++) {
        result *= 10;

        const char ch = view->data[i];
        if (isdigit(ch)) {
            const int digit = ch - '0';
            result += digit;
        } else {
            DEBUG_PRINT("Found non-digit character: %c", ch);
            return false;
        }
    }

    *out_result = result;
    return true;
}