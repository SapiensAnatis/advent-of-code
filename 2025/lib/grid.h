//
// Created by jay on 07/03/2026.
//

#ifndef AOC2025_LIB_GRID_H
#define AOC2025_LIB_GRID_H

#include "lib/string_view.h"

typedef ptrdiff_t grid_coord;

struct Grid {
    struct String* data; // Not StringView: helpful to be able to edit squares
    grid_coord size_x;
    grid_coord size_y;
};

struct GridCoordinate {
    grid_coord x;
    grid_coord y;
};

struct GridAdjacentSquares {
    uint8_t count;
    char squares[8];
};

struct GridAdjacentCoordinates {
    uint8_t count;
    struct GridCoordinate coords[8];
};

struct Grid* grid_create(struct String* data);

struct Grid* grid_copy(const struct Grid* source);

char grid_get_square(const struct Grid* grid, grid_coord x, grid_coord y);

bool grid_set_square(struct Grid* grid, grid_coord x, grid_coord y, char value);

void grid_print(const struct Grid* grid);

struct GridAdjacentSquares grid_get_adjacent_squares(const struct Grid* grid, grid_coord x,
                                                     grid_coord y);

struct GridAdjacentCoordinates grid_get_adjacent_coords(const struct Grid* grid, grid_coord x,
                                                        grid_coord y);

void grid_free(struct Grid* grid);

#endif // AOC2025_LIB_GRID_H
