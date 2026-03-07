#include "lib/grid.h"

#include "lib/debug.h"
#include "lib/string.h"
#include "lib/string_view.h"

#include <assert.h>
#include <limits.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>

static grid_coord grid_calc_index(const struct Grid* grid, const grid_coord x, const grid_coord y) {
    if (x < 0 || y < 0 || x >= grid->size_x || y >= grid->size_y) {
        return -1;
    }

    // +1: grid buffer has newlines
    const grid_coord idx = (y * (grid->size_x + 1)) + x;

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
    grid_coord cursor = string_view_find_char(&data_view, '\n', 0);

    if (cursor == -1) {
        DEBUG_PRINT("Failed to create grid: no newlines found");
        return nullptr;
    }
    if (cursor > INT_MAX) {
        DEBUG_PRINT("Failed to create grid: too large");
        return nullptr;
    }

    grid->size_x = cursor;

    grid_coord size_y = 0;

    do {
        size_y += 1;
        const grid_coord new_start = cursor + 1;
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
char grid_get_square(const struct Grid* grid, const grid_coord x, const grid_coord y) {
    const grid_coord idx = grid_calc_index(grid, x, y);
    if (idx == -1) {
        return 0;
    }

    return string_data(grid->data)[idx];
}

bool grid_set_square(struct Grid* grid, const grid_coord x, const grid_coord y, const char value) {
    const grid_coord idx = grid_calc_index(grid, x, y);
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

struct GridAdjacentSquares grid_get_adjacent_squares(const struct Grid* grid, const grid_coord x,
                                                     const grid_coord y) {
    struct GridAdjacentSquares result = {0};

    const struct GridAdjacentCoordinates coords = grid_get_adjacent_coords(grid, x, y);

    result.count = coords.count;

    for (size_t i = 0; i < coords.count; i++) {
        const struct GridCoordinate coord = coords.coords[i];
        result.squares[i] = grid_get_square(grid, coord.x, coord.y);
    }

    return result;
}

struct GridAdjacentCoordinates grid_get_adjacent_coords(const struct Grid* grid, const grid_coord x,
                                                        const grid_coord y) {
    struct GridAdjacentCoordinates result = {0};

    for (grid_coord x_search = x - 1; x_search <= x + 1; x_search++) {
        for (grid_coord y_search = y - 1; y_search <= y + 1; y_search++) {
            if (x_search == x && y_search == y) {
                continue;
            }

            const bool in_bounds = grid_calc_index(grid, x_search, y_search) != -1;
            if (in_bounds) {
                assert(result.count < 8);
                result.coords[result.count] = (struct GridCoordinate){.x = x_search, .y = y_search};
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