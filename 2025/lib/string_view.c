#include "lib/string_view.h"

#include "debug.h"

#include <ctype.h>
#include <limits.h>
#include <stdlib.h>
#include <string.h>

char* string_view_to_string(const struct StringView* view) {
    // String views are not guaranteed to be null terminated, so conversion to a C string requires
    // making a copy
    char* buffer = malloc(view->length + 1);
    memcpy(buffer, view->data, view->length);
    buffer[view->length] = '\0';

    return buffer;
}

bool try_parse_int(const struct StringView* view, int* out_result) {
    long tmp_result = 0;
    const bool result = try_parse_long(view, &tmp_result);
    if (result && tmp_result <= INT_MAX && tmp_result >= INT_MIN) {
        *out_result = (int)tmp_result;
        return true;
    }

    return false;
}

bool try_parse_long(const struct StringView* view, long* out_result) {
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