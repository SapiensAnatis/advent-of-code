#include "day03/day03.h"

#include "lib/debug.h"
#include "lib/file.h"
#include "lib/string.h"
#include "lib/string_split.h"
#include "lib/string_view.h"

#include <assert.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>

int64_t find_max_jolts_part1(const struct StringView* line) {
    DEBUG_PRINT("Analysing line: '%.*s'", (int)line->length, line->data);

    int max_digit = 0;
    size_t max_digit_position = 0;
    size_t line_length = line->length;

    for (size_t i = 0; i < line_length; i++) {
        char ch = line->data[i];
        int digit = ch - '0';
        if (digit > max_digit) {
            max_digit = digit;
            max_digit_position = i;
        }
    }

    int second_max_digit = 0;
    size_t start_search_pos = (max_digit_position + 1) % line_length;
    size_t second_max_digit_position = 0;

    for (size_t i = start_search_pos; i < line_length && i != max_digit_position; i++) {
        char ch = line->data[i];
        int digit = ch - '0';
        if (digit > second_max_digit) {
            second_max_digit = digit;
            second_max_digit_position = i;
        }
    }

    assert(second_max_digit_position != max_digit_position);

    int answer = max_digit_position < second_max_digit_position
                     ? (max_digit * 10) + second_max_digit
                     : (second_max_digit * 10) + max_digit;

    DEBUG_PRINT("Max jolts: %d", answer);

    return answer;
}

int64_t find_max_jolts_part2(const struct StringView* line) {
    DEBUG_PRINT("Analysing line: '%.*s'", (int)line->length, line->data);

    constexpr size_t JOLT_LENGTH = 12;

    size_t line_length = line->length;
    const size_t max_skips_possible = line_length - JOLT_LENGTH;
    size_t remaining_skips = max_skips_possible;
    size_t line_position = 0;

    int jolt_digits[JOLT_LENGTH] = {0};

    for (size_t i = 0; i < JOLT_LENGTH; i++) {
        char ch = line->data[line_position];
        char max_skip_ch = ch;
        size_t skip_usage = 0;

        for (size_t skip_count = 1; skip_count <= remaining_skips; skip_count++) {
            size_t pos = line_position + skip_count;
            if (pos >= line_length) {
                break;
            }

            char candidate_skip_ch = line->data[pos];

            DEBUG_PRINT(
                "Line position %zu. Evaluating skip to use %c instead of %c. Skip count: %zu",
                line_position, candidate_skip_ch, max_skip_ch, skip_count);

            if (candidate_skip_ch > max_skip_ch) {
                max_skip_ch = candidate_skip_ch;
                skip_usage = skip_count;
            }
        }

        // clang-tidy sees these as identical empty branches in release mode
        // NOLINTNEXTLINE(bugprone-branch-clone)
        if (skip_usage >= 1) {
            DEBUG_PRINT("Skipping %zu characters to use %c instead of %c", skip_usage, max_skip_ch,
                        ch);
        } else {
            DEBUG_PRINT("Using character %c. Skips remaining: %zu. Skip usage: %zu", ch,
                        remaining_skips, skip_usage);
        }

        remaining_skips -= skip_usage;
        ch = max_skip_ch;
        int digit = ch - '0';

        jolt_digits[i] = digit;
        line_position += 1;
        line_position += skip_usage;
    }

    long long jolt_acc = 0;
    for (size_t jolt_index = 0; jolt_index < JOLT_LENGTH; jolt_index++) {
        jolt_acc += jolt_digits[jolt_index];

        bool last_iteration = jolt_index == JOLT_LENGTH - 1;
        if (!last_iteration) {
            jolt_acc *= 10;
        }
    }

    DEBUG_PRINT("Max jolts: %lld", jolt_acc);

    return jolt_acc;
}

int64_t parse_input_get_answer(FILE* file, int64_t (*find_jolts_func)(const struct StringView*)) {
    struct String* string = read_all_text(file);
    string_trim_end(string, '\n');

    const char* string_buffer = string_data(string);

    int64_t answer = 0;

    struct StringSplitIterator iter = string_split_create(string_buffer, "\n");
    do {
        answer += find_jolts_func(&iter.current_segment);
    } while (string_split_move_next(&iter));

    string_free(string);

    return answer;
}

int64_t day03_part1(FILE* file) { return parse_input_get_answer(file, find_max_jolts_part1); }

int64_t day03_part2(FILE* file) { return parse_input_get_answer(file, find_max_jolts_part2); }
