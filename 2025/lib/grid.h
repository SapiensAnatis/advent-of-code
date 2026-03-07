//
// Created by jay on 07/03/2026.
//

#ifndef AOC_LIB_GRID_H
#define AOC_LIB_GRID_H

#include "lib/string_view.h"

struct Grid {
    struct String* data; // Not StringView: helpful to be able to edit squares
    ptrdiff_t size_x;
    ptrdiff_t size_y;
};

struct GridAdjacentSquares {
    uint8_t count;
    char squares[8];
};

struct Grid* grid_create(struct String* data);

struct Grid* grid_copy(const struct Grid* source);

char grid_get_square(const struct Grid* grid, ptrdiff_t x, ptrdiff_t y);

bool grid_set_square(struct Grid* grid, ptrdiff_t x, ptrdiff_t y, char value);

void grid_print(const struct Grid* grid);

struct GridAdjacentSquares grid_get_adjacent_squares(const struct Grid* grid, ptrdiff_t x,
                                                     ptrdiff_t y);

void grid_free(struct Grid* grid);

#endif // AOC_LIB_GRID_H
