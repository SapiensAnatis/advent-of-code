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

int64_t day06_part1(FILE* file) {
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

    int64_t result = 0;

    for (size_t i = 0; i < vector_size(problems); i++) {
        const struct MathProblem* problem = vector_at(problems, i);
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

        result += local_result;
    }

    vector_free(problems);

    return result;
}