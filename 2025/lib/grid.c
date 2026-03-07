#include "lib/grid.h"

#include "lib/debug.h"
#include "lib/string.h"
#include "lib/string_view.h"

#include <assert.h>
#include <limits.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>

static ptrdiff_t grid_calc_index(const struct Grid* grid, const ptrdiff_t x, const ptrdiff_t y) {
    if (x < 0 || y < 0 || x >= grid->size_x || y >= grid->size_y) {
        return -1;
    }

    // +1: grid buffer has newlines
    const ptrdiff_t idx = (y * (grid->size_x + 1)) + x;

    return idx;
}

/**
 * Creates a new grid from a string view. The grid will become the new owner of the string.
 * @param data The input data.
 * @return A grid, or nullptr if the grid could not be parsed.
 */
struct Grid* grid_create(struct String* data) {
    struct Grid* grid = malloc(sizeof(struct Grid));

    grid->data = data;
    const struct StringView data_view = string_to_string_view(data);
    ptrdiff_t cursor = string_view_find_char(&data_view, '\n', 0);

    if (cursor == -1) {
        DEBUG_PRINT("Failed to create grid: no newlines found");
        return nullptr;
    }
    if (cursor > INT_MAX) {
        DEBUG_PRINT("Failed to create grid: too large");
        return nullptr;
    }

    grid->size_x = cursor;

    ptrdiff_t size_y = 0;

    do {
        size_y += 1;
        const ptrdiff_t new_start = cursor + 1;
        cursor = string_view_find_char(&data_view, '\n', new_start);

        if (cursor != -1 && cursor - new_start != grid->size_x) {
            DEBUG_PRINT("Failed to create grid: uneven rows");
            return nullptr;
        }
    } while (cursor != -1);

    grid->size_y = size_y;

    return grid;
}

struct Grid* grid_copy(const struct Grid* source) { return grid_create(string_copy(source->data)); }

/**
 * Gets a square in a grid.
 * @param grid The grid to index.
 * @param x The x co-ordinate of the square to get.
 * @param y The y co-ordinate of the square to get.
 * @return A character at that grid square, or \0 if the co-ordinates were out of range.
 */
char grid_get_square(const struct Grid* grid, const ptrdiff_t x, const ptrdiff_t y) {
    const ptrdiff_t idx = grid_calc_index(grid, x, y);
    if (idx == -1) {
        return 0;
    }

    return string_data(grid->data)[idx];
}

bool grid_set_square(struct Grid* grid, const ptrdiff_t x, const ptrdiff_t y, const char value) {
    const ptrdiff_t idx = grid_calc_index(grid, x, y);
    if (idx == -1) {
        return false;
    }

    string_data(grid->data)[idx] = value;
    return true;
}

void grid_print(const struct Grid* grid) {
    printf("%s", string_data(grid->data));
    fflush(stdout);
}

struct GridAdjacentSquares grid_get_adjacent_squares(const struct Grid* grid, const ptrdiff_t x,
                                                     const ptrdiff_t y) {
    struct GridAdjacentSquares result = {0};

    for (ptrdiff_t x_search = x - 1; x_search <= x + 1; x_search++) {
        for (ptrdiff_t y_search = y - 1; y_search <= y + 1; y_search++) {
            if (x_search == x && y_search == y) {
                continue;
            }

            const char square = grid_get_square(grid, x_search, y_search);
            if (square != 0 && square != '\n') {
                assert(result.count < 8);
                result.squares[result.count] = square;
                result.count += 1;
            }
        }
    }

    return result;
}

void grid_free(struct Grid* grid) {
    string_free(grid->data);
    free(grid);
}