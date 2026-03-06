#include "day02/day02.h"
#include "lib/debug.h"
#include "lib/file.h"
#include "lib/string.h"
#include "lib/string_split.h"

#include <assert.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>

bool is_invalid_id_part1(long id) {
    int digit_count = (int)ceil(log10((double)id));
    if (digit_count % 2 != 0) {
        // Numbers with an odd digit count cannot be invalid
        return false;
    }

    assert(digit_count > 1);

    // For a number like 1010, divide it by 10^2 (100)
    int half_digit_count = digit_count / 2;
    long divisor = (int)trunc(pow(10.0, half_digit_count));
    // An invalid ID, when divided by the right number, will have an equal quotient and modulus
    ldiv_t div_result = ldiv(id, divisor);

    DEBUG_PRINT("Division of %ld by %ld: quot: %ld, rem: %ld", id, divisor, div_result.quot,
                div_result.rem);

    return div_result.quot == div_result.rem;
}

bool is_invalid_id_part2(long id) {
    int digit_count = (int)ceil(log10((double)id));

    if (digit_count <= 1) {
        return false;
    }

    // For a number like 1010, if it is not an invalid number by 10^2 (100), then it cannot be one
    // by 1000 as 1000 encompasses 3 digits which is more than half.
    int half_digit_count = digit_count / 2;
    long max_divisor = (int)trunc(pow(10.0, half_digit_count));

    for (long current_divisor = 10; current_divisor <= max_divisor; current_divisor *= 10) {
        ldiv_t initial_div_result = ldiv(id, current_divisor);
        long expected_rem = initial_div_result.rem;
        long current_id = initial_div_result.quot;
        bool all_rems_match_expected = true;

        while (current_id > 0) {
            ldiv_t div_result = ldiv(current_id, current_divisor);
            all_rems_match_expected &= (div_result.rem == expected_rem);
            current_id = div_result.quot;
        }

        if (all_rems_match_expected) {
            // This method is not bulletproof, for instance 30303 will pass the test of being
            // invalid according to the divisor 100. We can eliminate these false positives, because
            // being invalid by divisor 100 means that it is proposed the number is made up of a
            // repeating n-digit sequence, where n = log10(100) = 2. In the case of 30303 this
            // cannot be true, because the number of digits doesn't divide by 2.

            // Could just keep a counter in the loop but I'm lazy
            int proposed_sequence_len = (int)trunc(log10((double)current_divisor));
            if (digit_count % proposed_sequence_len == 0) {
                DEBUG_PRINT("ID %ld is invalid according to divisor %ld", id, current_divisor);
                return true;
            }
        }
    }

    return false;
}

void parse_and_scan(FILE* file, bool (*id_checker)(long)) {
    struct String* file_contents = read_all_text(file);
    string_trim_end(file_contents, '\n');

    char* file_contents_ptr = string_data(file_contents);

    struct StringSplitIterator* iter = string_split_create(file_contents_ptr, ",");

    long invalid_id_sum = 0;

    do {
        const char* current = string_view_to_string(&iter->current_segment);
        DEBUG_PRINT("Checking segment: '%s'", current);

        long range_start = 0;
        long range_end = 0;

        struct StringSplitIterator* iter_inner = string_split_create(current, "-");

        const struct StringView* segment_inner = &iter_inner->current_segment;
        bool result = try_parse_long(segment_inner, &range_start);
        if (!result) {
            assert(result && "failed to parse segment");
            exit(1);
        }

        bool has_next = string_split_move_next(iter_inner);
        if (!has_next) {
            assert(false && "no next segment");
            exit(1);
        }

        segment_inner = &iter_inner->current_segment;;
        result = try_parse_long(segment_inner, &range_start);
        if (!result) {
            assert(result && "failed to parse segment");
            exit(1);
        }

        for (long i = range_start; i <= range_end; i++) {
            if (id_checker(i)) {
                DEBUG_PRINT("Found invalid ID: %ld", i);
                printf("Found invalid ID: %ld\n", i);
                invalid_id_sum += i;
            }
        }

        string_split_free(iter_inner);
    } while (string_split_move_next(iter));

    printf("Invalid ID sum: %ld\n", invalid_id_sum);

    string_split_free(iter);
    string_free(file_contents);
}

void day02_part1(FILE* file) { parse_and_scan(file, is_invalid_id_part1); }

void day02_part2(FILE* file) { parse_and_scan(file, is_invalid_id_part2); }
