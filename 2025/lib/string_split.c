#include "lib/string_split.h"

#include "lib/debug.h"

#include <assert.h>
#include <stdlib.h>
#include <string.h>

struct StringSplitIterator string_split_create(const char* string, const char* separator) {
    // NOLINTNEXTLINE(modernize-use-nullptr): clang-tidy bug
    struct StringSplitIterator iter = {0};

    iter.source_string.data = string;
    iter.source_string.length = strlen(string);
    iter.separator = separator;
    iter.current_position = 0;

    const char* next_ptr = strstr(string, iter.separator);
    if (next_ptr == nullptr) {
        // String does not contain separator. Yield a single segment which is just the entire
        // string.
        iter.current_segment = iter.source_string;

        DEBUG_PRINT("Created single-segment StringSplitIterator: '%.*s'",
                    (int)iter.current_segment.length, iter.current_segment.data);
    } else {
        iter.current_segment.data = string;
        iter.current_segment.length = next_ptr - string;

        DEBUG_PRINT("Created StringSplitIterator with initial segment: '%.*s'",
                    (int)iter.current_segment.length, iter.current_segment.data);
    }

    return iter;
}

bool string_split_move_next(struct StringSplitIterator* iter) {
    size_t search_start_position =
        iter->current_position + iter->current_segment.length + strlen(iter->separator);

    if (search_start_position > iter->source_string.length) {
        DEBUG_PRINT("No further segments in StringSplitIterator");
        return false;
    }

    const char* search_start = iter->source_string.data + search_start_position;
    const char* next_ptr = strstr(search_start, iter->separator);

    if (next_ptr == nullptr) {
        // No further segment. Return the remainder of the string.
        const size_t bytes_left = iter->source_string.length - search_start_position;
        iter->current_segment.data = search_start;
        iter->current_segment.length = bytes_left;
        iter->current_position = search_start_position;

        DEBUG_PRINT("Reached last split segment in string: '%.*s'",
                    (int)iter->current_segment.length, iter->current_segment.data);
    } else {
        const size_t segment_size = next_ptr - search_start;
        iter->current_segment.data = search_start;
        iter->current_segment.length = segment_size;
        iter->current_position = search_start_position;

        DEBUG_PRINT("Reached next split segment in string: '%.*s'",
                    (int)iter->current_segment.length, iter->current_segment.data);
    }

    return true;
}
