#include "day07/day07.h"

#include "lib/debug.h"
#include "lib/fatal_error.h"
#include "lib/file.h"
#include "lib/grid.h"
#include "lib/hash_fns.h"
#include "lib/hash_map.h"
#include "lib/hash_set.h"

#include <assert.h>
#include <stdint.h>
#include <stdio.h>

grid_coord get_initial_beam_pos(const struct Grid* grid) {
    for (grid_coord x = 0; x < grid->size_x; x++) {
        if (grid_get_square(grid, x, 0) == 'S') {
            return x;
        }
    }

    FATAL_ERROR("Failed to find initial beam position");
}

int64_t day07_part1(FILE* file) {
    struct Grid* grid = grid_create(read_all_text(file));

    // Basic idea: split the beam at each ^ square and maintain a set of the x co-ordinates where
    // the beam currently is, which will be used in the iteration covering the next row to build
    // another set. If a splitter at coord x is hit, it adds (x-1, x+1) to the set and removes x
    // from the set. It is important to use a set as it is possible for two splits to dump into
    // the same square.

    struct HashSet x_with_beam =
        hash_set_create(sizeof(grid_coord), hash_ptrdiff_t, equal_ptrdiff_t);

    const grid_coord initial_beam_pos = get_initial_beam_pos(grid);
    hash_set_try_add(&x_with_beam, &initial_beam_pos);

    int64_t split_count = 0;

    for (grid_coord y = 2; y < grid->size_y; y++) { // splitters start at y = 2
        // We need to apply beam splits atomically after we're done scanning the row so that they
        // don't affect our remaining checks on the same row. Additionally unsure if updating the
        // set while iterating it is a good idea
        size_t split_positions_idx = 0;
        grid_coord split_positions[256];

        struct HashSetIterator iter = hash_set_iterator_create(&x_with_beam);

        do {
            const grid_coord x = *(grid_coord*)iter.current_value;
            const char square = grid_get_square(grid, x, y);

            if (square == '^') {
                // The beam has been split
                split_count += 1;

                split_positions[split_positions_idx] = x;
                split_positions_idx += 1;
                assert(split_positions_idx < 256);
            }
        } while (hash_set_iterator_move_next(&iter));

        for (size_t i = 0; i < split_positions_idx; i++) {
            grid_coord x = split_positions[i];

            // Remove this co-ordinate and replace it with left and right
            hash_set_try_remove(&x_with_beam, &x);

            const grid_coord new_beam_pos_1 = x - 1;
            const grid_coord new_beam_pos_2 = x + 1;

            // Input seems to never put a splitter directly on a grid edge, so it should always
            // be in bounds
            assert(new_beam_pos_1 >= 0 && new_beam_pos_1 < grid->size_x);
            assert(new_beam_pos_2 >= 0 && new_beam_pos_2 < grid->size_x);

            hash_set_try_add(&x_with_beam, &new_beam_pos_1);
            hash_set_try_add(&x_with_beam, &new_beam_pos_2);
        }
    }

    grid_free(grid);
    hash_set_free(&x_with_beam);

    return split_count;
}

uint64_t day07_part2(FILE* file) {
    struct Grid* grid = grid_create(read_all_text(file));

    // The idea here is to no longer store x positions in a unique set, but store a hash map of
    // (x position, number of beams). Then each splitter creates 1 new timeline. We can't store
    // the positions directly as that will require too much space, so we group them in a hash map

    struct HashMap x_position_map =
        hash_map_create(sizeof(grid_coord), sizeof(uint64_t), hash_ptrdiff_t, equal_ptrdiff_t);

    const grid_coord initial_beam_pos = get_initial_beam_pos(grid);
    const uint64_t initial_beam_count = 1;
    hash_map_try_add(&x_position_map, &initial_beam_pos, &initial_beam_count);

    for (grid_coord y = 2; y < grid->size_y; y++) { // splitters start at y = 2
        // Same deal from part 1, need to apply the splits after the analysis pass
        size_t split_positions_idx = 0;
        grid_coord split_positions[256];

        DEBUG_PRINT("Processing Y = %td / %td ", y, grid->size_y);

        struct HashMapIterator hash_map_iterator = hash_map_iterator_create(&x_position_map);

        do {
            const grid_coord x = *(grid_coord*)hash_map_iterator.current_key;
            if (grid_get_square(grid, x, y) == '^') {
                split_positions[split_positions_idx] = x;
                split_positions_idx += 1;
            }
        } while (hash_map_iterator_move_next(&hash_map_iterator));

        for (size_t i = 0; i < split_positions_idx; i++) {
            grid_coord x = split_positions[i];

            // Remove this co-ordinate and replace it with left and right
            const grid_coord new_beam_pos_1 = x - 1;
            const grid_coord new_beam_pos_2 = x + 1;

            assert(new_beam_pos_1 >= 0 && new_beam_pos_1 < grid->size_x);
            assert(new_beam_pos_2 >= 0 && new_beam_pos_2 < grid->size_x);

            uint64_t* x_value_ptr = hash_map_get_value_ptr(&x_position_map, &x);
            assert(x_value_ptr != nullptr); // it should have been yielded from the iterator

            uint64_t* new_x_1_value_ptr =
                hash_map_get_value_ptr_or_add_default(&x_position_map, &new_beam_pos_1);
            uint64_t* new_x_2_value_ptr =
                hash_map_get_value_ptr_or_add_default(&x_position_map, &new_beam_pos_2);

            // Each beam now continues in the left and right - effect is multiplied by the
            // number of beams
            *new_x_1_value_ptr += *x_value_ptr;
            *new_x_2_value_ptr += *x_value_ptr;

            *x_value_ptr = 0; // There is no longer any beam at the splitter x co-ord
        }
    }

    // The number of timelines is the sum of hash map values
    struct HashMapIterator hash_map_iterator = hash_map_iterator_create(&x_position_map);

    uint64_t result = 0;

    do {
        result += *(uint64_t*)hash_map_iterator.current_value;
    } while (hash_map_iterator_move_next(&hash_map_iterator));

    hash_map_free(&x_position_map);
    grid_free(grid);

    return result;
}