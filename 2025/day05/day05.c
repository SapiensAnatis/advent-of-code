#include "day05/day05.h"

#include "lib/debug.h"
#include "lib/file.h"
#include "lib/range.h"
#include "lib/string_split.h"
#include "lib/string_view.h"
#include "lib/vector.h"

#include <assert.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int32_t day05_part1(FILE* file) {
    char line_buf[256];

    // Read ranges
    struct Vector* range_vector = vector_create(sizeof(struct Range));

    while (read_line(file, line_buf, sizeof(line_buf))) {
        if (line_buf[0] == '\0') {
            // Empty line: signals end of ranges and start of values
            break;
        }

        struct StringSplitIterator* split = string_split_create(line_buf, "-");

        struct Range range = {0};

        if (!string_view_try_parse_int64(&split->current_segment, &range.start)) {
            assert(false && "failed to parse range start");
            abort();
        }

        if (!string_split_move_next(split)) {
            assert(false && "failed to get split end");
            abort();
        }

        if (!string_view_try_parse_int64(&split->current_segment, &range.end)) {
            assert(false && "failed to parse range end");
            abort();
        }

        vector_append(range_vector, &range);

        string_split_free(split);
    }

    DEBUG_PRINT("Created %ld ranges", vector_size(range_vector));

    int32_t fresh_ids = 0;

    while (read_line(file, line_buf, sizeof(line_buf))) {
        struct StringView line_view = {.data = line_buf, .length = strlen(line_buf)};

        int64_t id_value = 0;
        if (!string_view_try_parse_int64(&line_view, &id_value)) {
            assert(false && "failed to parse id");
            abort();
        }

        for (size_t i = 0; i < vector_size(range_vector); i++) {
            const struct Range* range = vector_at(range_vector, i);
            if (range_test(range, id_value)) {
                fresh_ids += 1;
                break; // Don't check against other ranges
            }
        }
    }

    vector_free(range_vector);

    return fresh_ids;
}