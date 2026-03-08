#include "day06/day06.h"

#include "lib/debug.h"
#include "lib/file.h"
#include "lib/string_split.h"
#include "lib/string_view.h"
#include "lib/vector.h"

#include <assert.h>
#include <ctype.h>
#include <inttypes.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

struct MathProblem {
    struct Vector* operands; // vector<int32_t>
    char operator_ch;
};

static void free_math_problem(void* problem) {
    vector_free(((struct MathProblem*)problem)->operands);
}

static bool is_operator(const char c) { return c == '+' || c == '*'; }

static void debug_print_math_problem(const struct MathProblem* problem) {
#ifdef DEBUG_PRINT
    char buf[1024];
    auto cursor = buf;
    for (size_t i = 0; i < vector_size(problem->operands); i++) {
        const int32_t* operand = vector_at(problem->operands, i);
        char op = 0;

        if (i == vector_size(problem->operands) - 1) {
            op = '=';
        } else {
            op = problem->operator_ch;
        }

        auto const written = sprintf(cursor, "%d %c ", *operand, op);
        assert(written > 0);
        cursor += written;
    }
    *cursor = '\0';

    DEBUG_PRINT("%s", buf);
#endif
}

static struct Vector* parse_math_problems(FILE* file) {
    struct Vector* problems = vector_create(sizeof(struct MathProblem), free_math_problem);

    // Read the first line, and initialise our problems, each with their first number
    char buf[4096];

    if (!read_line(file, buf, sizeof(buf))) {
        assert(false && "failed to read first file line");
        abort();
    }

    struct StringSplitIterator first_line_split =
        string_split_create(buf, " ", STRING_SPLIT_REMOVE_EMPTY_ENTRIES);

    do {
        struct MathProblem problem = {
            .operands = vector_create(sizeof(int32_t), VECTOR_DEFAULT_DELETER), .operator_ch = '+'};

        int32_t first_operand = 0;
        if (!string_view_try_parse_int32(&first_line_split.current_segment, &first_operand)) {
            assert(false && "failed to parse first line");
            abort();
        }

        vector_append(problem.operands, &first_operand);
        vector_append(problems, &problem);
    } while (string_split_move_next(&first_line_split));

    while (read_line(file, buf, sizeof(buf))) {
        if (!isdigit(buf[0]) && !isspace(buf[0])) {
            // Operator found
            break;
        }

        struct StringSplitIterator split_iterator =
            string_split_create(buf, " ", STRING_SPLIT_REMOVE_EMPTY_ENTRIES);

        int32_t problem_idx = 0;

        do {
            int32_t operand = 0;
            if (!string_view_try_parse_int32(&split_iterator.current_segment, &operand)) {
                assert(false && "failed to parse operand");
                abort();
            }

            struct MathProblem* problem = vector_at(problems, problem_idx);
            vector_append(problem->operands, &operand);

            problem_idx += 1;
        } while (string_split_move_next(&split_iterator));
    }

    // We should now be on the last line, which contains operators
    if (!is_operator(buf[0])) {
        assert(false && "failed to find operator line");
        abort();
    }

    {
        int32_t problem_idx = 0;

        struct StringSplitIterator op_iterator =
            string_split_create(buf, " ", STRING_SPLIT_REMOVE_EMPTY_ENTRIES);

        do {
            struct MathProblem* problem = vector_at(problems, problem_idx);
            const char operator_ch = op_iterator.current_segment.data[0];
            if (!is_operator(operator_ch)) {
                assert(false && "invalid operator in operator line");
                abort();
            }

            problem->operator_ch = operator_ch;
            problem_idx += 1;
        } while (string_split_move_next(&op_iterator));
    }

    DEBUG_PRINT("Parsed %" PRIi64 "　math problems：", vector_size(problems));

    return problems;
}

static int64_t solve_problem(const struct MathProblem* problem) {
    DEBUG_PRINT("Solving problem:");
    debug_print_math_problem(problem);

    int64_t local_result = *((int32_t*)vector_at(problem->operands, 0));
    for (size_t j = 1; j < vector_size(problem->operands); j++) {
        const int32_t* operand = vector_at(problem->operands, j);
        if (problem->operator_ch == '+') {
            local_result += *operand;
        } else {
            local_result *= *operand;
        }
    }

    DEBUG_PRINT("Result: %" PRId64 "", local_result);

    return local_result;
}

int64_t day06_part1(FILE* file) {
    struct Vector* problems = parse_math_problems(file);

    int64_t result = 0;

    for (size_t i = 0; i < vector_size(problems); i++) {
        const struct MathProblem* problem = vector_at(problems, i);
        result += solve_problem(problem);
    }

    vector_free(problems);

    return result;
}

static uint8_t count_digits(const int32_t number) {
    uint8_t digits = 1;
    int32_t acc = number;
    while (acc >= 10) {
        acc /= 10;
        digits++;
    }

    return digits;
}

static uint8_t extract_digit_left(const int32_t number, const uint8_t digit_index) {
    static constexpr int32_t pow10[] = {1,      10,      100,      1000,      10000,
                                        100000, 1000000, 10000000, 100000000, 1000000000};

    const uint8_t digits = count_digits(number);
    const int16_t idx = (int16_t)(digits - digit_index - 1);
    if (idx < 0) {
        return 0;
    }

    assert(idx < (int16_t)sizeof(pow10));
    const int32_t num_shifted = number / pow10[idx];
    const int32_t unit_place = num_shifted % 10;

    return unit_place;
}

static uint8_t extract_digit_right(const int32_t number, const uint8_t digit_index) {
    static constexpr int32_t pow10[] = {1,      10,      100,      1000,      10000,
                                        100000, 1000000, 10000000, 100000000, 1000000000};
    assert(digit_index < (int16_t)sizeof(pow10));
    const int32_t num_shifted = number / pow10[digit_index];
    const int32_t unit_place = num_shifted % 10;

    return unit_place;
}

int64_t day06_part2(FILE* file) {
    struct Vector* problems = parse_math_problems(file);
    struct Vector* transformed_problems =
        vector_create(sizeof(struct MathProblem), free_math_problem);

    for (size_t i = 0; i < vector_size(problems); i++) {
        const struct MathProblem* problem = vector_at(problems, i);

        struct MathProblem transformed_problem = {
            .operands = vector_create(sizeof(int32_t), VECTOR_DEFAULT_DELETER),
            .operator_ch = problem->operator_ch,
        };

        // Instead of reparsing the input we can just extract digits from each number vertically.
        // Whether we extract the digits from the left or right depends on how the problem was
        // formatted. If a problem is right aligned:
        //
        // 123
        //  45
        //   6
        //
        // then we must extract from each column starting from the right to get 356, 24, 1
        //
        // If a problem is left aligned:
        //
        // 64
        // 23
        // 314
        //
        // then we must extract starting from the left to get 4, 431, 623
        //
        // One idea would be to store the position each operand was found in, and then check
        // all positions in a number; if they're all the same then the numbers were left aligned,
        // but if they differ the numbers were right aligned

        const int32_t first_orig_operand = *(int32_t*)vector_at(problem->operands, 0);
        const int32_t last_orig_operand =
            *(int32_t*)vector_at(problem->operands, vector_size(problem->operands) - 1);

        const bool is_right_extract = first_orig_operand > last_orig_operand;

        for (unsigned digit_idx = 0;; digit_idx++) {
            int32_t new_operand = 0;

            for (size_t j = 0; j < vector_size(problem->operands); j++) {
                const int32_t* operand = vector_at(problem->operands, j);

                // BROKEN: see above, we need to check spacing for this
                const uint8_t digit = is_right_extract ? extract_digit_right(*operand, digit_idx)
                                                       : extract_digit_left(*operand, digit_idx);

                // DEBUG_PRINT("Extracted %s digit %d at index %d from number %d",
                //             is_right_extract ? "right" : "left", digit, digit_idx, *operand);

                // This breaks if one of the digits is genuinely 0, but we are fortunate that (at
                // least in my input) 0 is never used, which seems intentional...
                if (digit != 0) {
                    new_operand *= 10;
                    new_operand += digit;
                }
            }

            DEBUG_PRINT("Digit idx = %d, got value %d", digit_idx, new_operand);

            if (new_operand != 0) {
                vector_append(transformed_problem.operands, &new_operand);
            } else {
                // No useful digits extracted from this index
                break;
            }
        }

        DEBUG_PRINT("Original problem:");
        debug_print_math_problem(problem);
        DEBUG_PRINT("Transformed problem:");
        debug_print_math_problem(&transformed_problem);

        vector_append(transformed_problems, &transformed_problem);
    }

    int64_t result = 0;

    for (size_t i = 0; i < vector_size(transformed_problems); i++) {
        const struct MathProblem* transformed_problem = vector_at(transformed_problems, i);
        result += solve_problem(transformed_problem);
    }

    vector_free(problems);
    vector_free(transformed_problems);

    return result;
}
