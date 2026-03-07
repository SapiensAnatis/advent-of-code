//
// Created by jay on 07/03/2026.
//

#ifndef AOC_LIB_GRID_H
#define AOC_LIB_GRID_H

#include "lib/string_view.h"

struct Grid {
    struct StringView data;
    ptrdiff_t size_x;
    ptrdiff_t size_y;
};

struct GridAdjacentSquares {
    uint8_t squares_len;
    char squares[8];
};

struct Grid* grid_create(struct StringView data);

char grid_get_square(const struct Grid* grid, ptrdiff_t x, ptrdiff_t y);

struct GridAdjacentSquares grid_get_adjacent_squares(const struct Grid* grid, ptrdiff_t x,
                                                     ptrdiff_t y);

void grid_free(struct Grid* grid);

#endif // AOC_LIB_GRID_H
