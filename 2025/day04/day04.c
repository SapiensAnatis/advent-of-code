#include "day04/day04.h"

#include "lib/debug.h"
#include "lib/file.h"
#include "lib/grid.h"

#include <assert.h>
#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

void day04_part1(FILE* file) {
    struct Grid* grid = grid_create(read_all_text(file));

    if (grid == nullptr) {
        assert(false && "failed to parse grid");
        abort();
    }

    int accessible_squares_count = 0;

    for (ptrdiff_t x = 0; x < grid->size_x; x++) {
        for (ptrdiff_t y = 0; y < grid->size_y; y++) {
            const char square = grid_get_square(grid, x, y);
            if (square != '@') {
                continue;
            }

            DEBUG_PRINT("Evaluating square at %ld, %ld", x, y);

            const struct GridAdjacentSquares adjacent_squares =
                grid_get_adjacent_squares(grid, x, y);

            uint8_t paper_squares_count = 0;
            for (uint8_t i = 0; i < adjacent_squares.count; i++) {
                DEBUG_PRINT("Found adjacent square: %d", (int)adjacent_squares.squares[i]);
                if (adjacent_squares.squares[i] == '@') {
                    paper_squares_count += 1;
                }
            }

            DEBUG_PRINT("Paper square count: %d", paper_squares_count);

            if (paper_squares_count < 4) {
                accessible_squares_count += 1;
                DEBUG_PRINT("Marking square as accessible");
            }
        }
    }

    printf("Answer: %d\n", accessible_squares_count);

    grid_free(grid);
}