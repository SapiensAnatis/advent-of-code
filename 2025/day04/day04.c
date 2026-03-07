#include "day04/day04.h"

#include "lib/debug.h"
#include "lib/file.h"
#include "lib/grid.h"
#include "lib/vector.h"

#include <assert.h>
#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

static uint8_t get_adjacent_paper_count(const struct Grid* grid, const grid_coord x,
                                        const grid_coord y) {
    const struct GridAdjacentSquares adjacent_squares = grid_get_adjacent_squares(grid, x, y);

    uint8_t paper_squares_count = 0;
    for (uint8_t i = 0; i < adjacent_squares.count; i++) {
        DEBUG_PRINT("Found adjacent square: %d", (int)adjacent_squares.squares[i]);
        if (adjacent_squares.squares[i] == '@') {
            paper_squares_count += 1;
        }
    }

    DEBUG_PRINT("Paper square count: %d", paper_squares_count);

    return paper_squares_count;
}

int32_t day04_part1(FILE* file) {
    struct Grid* grid = grid_create(read_all_text(file));

    if (grid == nullptr) {
        assert(false && "failed to parse grid");
        abort();
    }

    int32_t accessible_squares_count = 0;

    for (grid_coord x = 0; x < grid->size_x; x++) {
        for (grid_coord y = 0; y < grid->size_y; y++) {
            const char square = grid_get_square(grid, x, y);
            if (square != '@') {
                continue;
            }

            const uint8_t paper_squares_count = get_adjacent_paper_count(grid, x, y);

            if (paper_squares_count < 4) {
                accessible_squares_count += 1;
                DEBUG_PRINT("Marking square as accessible");
            }
        }
    }

    grid_free(grid);

    return accessible_squares_count;
}

int32_t day04_part2(FILE* file) {
    struct Grid* grid = grid_create(read_all_text(file));

    if (grid == nullptr) {
        assert(false && "failed to parse grid");
        abort();
    }

    struct Vector* coords_to_edit = vector_create(sizeof(struct GridCoordinate));

    // Find initial squares that can be removed
    for (grid_coord x = 0; x < grid->size_x; x++) {
        for (grid_coord y = 0; y < grid->size_y; y++) {
            const char square = grid_get_square(grid, x, y);
            if (square != '@') {
                continue;
            }

            const uint8_t paper_squares_count = get_adjacent_paper_count(grid, x, y);

            if (paper_squares_count < 4) {
                // Reachable; can be removed
                struct GridCoordinate coords = {.x = x, .y = y};
                vector_append(coords_to_edit, &coords);
            }
        }
    }

    // For each reachable square, update it, and then re-check its neighbours to see if those have
    // become reachable now.

    int32_t removed_paper_count = 0;

    do {
        const struct GridCoordinate* coord = vector_pop(coords_to_edit);

        // The co-ordinates we queue up are not guaranteed to be unique, so check if we have
        // already removed a paper roll here
        if (grid_get_square(grid, coord->x, coord->y) != '@') {
            continue;
        }

        grid_set_square(grid, coord->x, coord->y, '.');
        removed_paper_count += 1;

        const struct GridAdjacentCoordinates adjacent_coords =
            grid_get_adjacent_coords(grid, coord->x, coord->y);

        for (uint8_t i = 0; i < adjacent_coords.count; i++) {
            const struct GridCoordinate neighbour_coord = adjacent_coords.coords[i];

            if (grid_get_square(grid, neighbour_coord.x, neighbour_coord.y) != '@') {
                continue;
            }

            const uint8_t paper_squares_count =
                get_adjacent_paper_count(grid, neighbour_coord.x, neighbour_coord.y);

            if (paper_squares_count < 4) {
                vector_append(
                    coords_to_edit,
                    &neighbour_coord); // NOTE: this overwrites coord, do not use after this point
            }
        }
    } while (vector_size(coords_to_edit) > 0);

    grid_free(grid);
    vector_free(coords_to_edit);

    return removed_paper_count;
}