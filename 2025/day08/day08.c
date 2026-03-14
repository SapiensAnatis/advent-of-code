#include "day08/day08.h"

#include "lib/debug.h"
#include "lib/deleter.h"
#include "lib/fatal_error.h"
#include "lib/file.h"
#include "lib/string_split.h"
#include "lib/string_view.h"
#include "lib/vector.h"

#include <assert.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

struct JunctionBox {
    uint32_t x;
    uint32_t y;
    uint32_t z;
    uint32_t circuit_no; // 0 = no circuit
};

struct JunctionBoxPair {
    struct JunctionBox* box_1;
    struct JunctionBox* box_2;
    uint64_t distance_sq;
};

static uint64_t calc_distance_sq(const struct JunctionBox* box_1, const struct JunctionBox* box_2) {
    const int64_t dx = (int64_t)box_1->x - (int64_t)box_2->x;
    const int64_t dy = (int64_t)box_1->y - (int64_t)box_2->y;
    const int64_t dz = (int64_t)box_1->z - (int64_t)box_2->z;

    return dx * dx + dy * dy + dz * dz;
}

static int compare_box_pairs(const void* a, const void* b) {
    const struct JunctionBoxPair* box_a = a;
    const struct JunctionBoxPair* box_b = b;

    if (box_a->distance_sq < box_b->distance_sq) {
        return -1;
    }
    if (box_a->distance_sq > box_b->distance_sq) {
        return 1;
    }

    return 0;
}

static int compare_uint32_t_desc(const void* a, const void* b) {
    const uint32_t uint_a = *(uint32_t*)a;
    const uint32_t uint_b = *(uint32_t*)b;

    if (uint_a < uint_b) {
        return 1;
    }
    if (uint_a > uint_b) {
        return -1;
    }

    return 0;
}

uint32_t day08_part1(FILE* file, bool example) {
    char buffer[256];

    struct Vector* box_positions = vector_create(sizeof(struct JunctionBox), DEFAULT_DELETER);

    while (read_line(file, buffer, sizeof(buffer))) {
        struct StringSplitIterator iter = string_split_create(buffer, ",", STRING_SPLIT_DEFAULT);
        uint32_t coords[3];
        size_t coords_idx = 0;

        do {
            if (!string_view_try_parse_uint32(&iter.current_segment, &coords[coords_idx])) {
                FATAL_ERROR("Failed to parse number");
            }

            coords_idx += 1;
        } while (string_split_move_next(&iter));

        if (coords_idx != 3) {
            FATAL_ERROR("Wrong number of coordinates");
        }

        struct JunctionBox pos = {.x = coords[0], .y = coords[1], .z = coords[2], .circuit_no = 0};

        vector_append(box_positions, &pos);
    }

    const size_t max_pair_count = example ? 10 : 1000;

    struct Vector* box_pairs = vector_create(sizeof(struct JunctionBoxPair), DEFAULT_DELETER);

    for (size_t i = 0; i < vector_size(box_positions); i++) {
        struct JunctionBox* box_1 = vector_at(box_positions, i);
        for (size_t j = i + 1; j < vector_size(box_positions); j++) {
            struct JunctionBox* box_2 = vector_at(box_positions, j);
            const uint64_t distance_sq = calc_distance_sq(box_1, box_2);

            const struct JunctionBoxPair pair = {
                .box_1 = box_1, .box_2 = box_2, .distance_sq = distance_sq};

            // We only need to check the N closest pairs, and if we don't impose some sort of
            // limit, as the input is 1000 lines, we will have 1000^2 = 1 million pairs in our
            // vector.
            if (vector_size(box_pairs) < max_pair_count) {
                vector_append(box_pairs, &pair);
                // TODO: perhaps a tree or some sort would be nice here for not requiring an
                // explicit sort step each time
                vector_sort(box_pairs, compare_box_pairs);
            } else {
                const struct JunctionBoxPair* furthest_pair = vector_back(box_pairs);
                if (pair.distance_sq < furthest_pair->distance_sq) {
                    vector_pop(box_pairs, nullptr);
                    vector_append(box_pairs, &pair);
                    // Sort again: new pair might not belong at the end of the vector.
                    vector_sort(box_pairs, compare_box_pairs);
                }
            }
        }
    }

    int32_t next_circuit_no = 1;
    size_t made_connections_count = 0;

    // Iterate through the sorted vector
    for (size_t i = 0; i < vector_size(box_pairs) && made_connections_count < max_pair_count; i++) {
        const struct JunctionBoxPair* pair = vector_at(box_pairs, i);
        if (pair->box_1->circuit_no == 0 && pair->box_2->circuit_no == 0) {
            // Create a new circuit
            DEBUG_PRINT("Connecting Box(%u,%u,%u) to Box(%u,%u,%u) in new circuit %d",
                        pair->box_1->x, pair->box_1->y, pair->box_1->z, pair->box_2->x,
                        pair->box_2->y, pair->box_2->z, next_circuit_no);

            pair->box_1->circuit_no = next_circuit_no;
            pair->box_2->circuit_no = next_circuit_no;
            next_circuit_no += 1;
            made_connections_count += 1;
        } else if (pair->box_1->circuit_no == 0) {
            // Join box 1 into box 2's circuit
            DEBUG_PRINT("Connecting Box(%u,%u,%u) to Box(%u,%u,%u) in box 2's circuit %d",
                        pair->box_1->x, pair->box_1->y, pair->box_1->z, pair->box_2->x,
                        pair->box_2->y, pair->box_2->z, pair->box_2->circuit_no);

            pair->box_1->circuit_no = pair->box_2->circuit_no;
            made_connections_count += 1;
        } else if (pair->box_2->circuit_no == 0) {
            // Join box 2 into box 1's circuit
            DEBUG_PRINT("Connecting Box(%u,%u,%u) to Box(%u,%u,%u) in box 1's circuit %d",
                        pair->box_1->x, pair->box_1->y, pair->box_1->z, pair->box_2->x,
                        pair->box_2->y, pair->box_2->z, pair->box_1->circuit_no);

            pair->box_2->circuit_no = pair->box_1->circuit_no;
            made_connections_count += 1;
        } else if (pair->box_2->circuit_no != pair->box_1->circuit_no) { // Merge circuits
            DEBUG_PRINT("Merging Box(%u,%u,%u) circuit %d with Box(%u,%u,%u) circuit %d into new "
                        "circuit %d",
                        pair->box_1->x, pair->box_1->y, pair->box_1->z, pair->box_1->circuit_no,
                        pair->box_2->x, pair->box_2->y, pair->box_2->z, pair->box_2->circuit_no,
                        next_circuit_no);

            const uint32_t target_circuit_no_1 = pair->box_1->circuit_no;
            const uint32_t target_circuit_no_2 = pair->box_2->circuit_no;

            for (size_t j = 0; j < vector_size(box_positions); j++) {
                struct JunctionBox* box = vector_at(box_positions, j);
                if (box->circuit_no == target_circuit_no_1 ||
                    box->circuit_no == target_circuit_no_2) {
                    DEBUG_PRINT("Moving Box(%u,%u,%u) from circuit %d to circuit %d", box->x,
                                box->y, box->z, box->circuit_no, next_circuit_no);
                    box->circuit_no = next_circuit_no;
                }
            }
            next_circuit_no += 1;
            made_connections_count += 1;
        } else {
            DEBUG_PRINT("Cannot connect Box(%u,%u,%u) to Box(%u,%u,%u): both already members of "
                        "the same circuit %d and %d",
                        pair->box_1->x, pair->box_1->y, pair->box_1->z, pair->box_2->x,
                        pair->box_2->y, pair->box_2->z, pair->box_1->circuit_no,
                        pair->box_2->circuit_no);
            made_connections_count += 1; // I don't get this
        }
    }

    uint32_t circuit_sizes[1000] = {0};
    for (size_t j = 0; j < vector_size(box_positions); j++) {
        const struct JunctionBox* box = vector_at(box_positions, j);
        assert(box->circuit_no < 1000);
        if (box->circuit_no != 0) {
            circuit_sizes[box->circuit_no] += 1;
        }
    }

    for (size_t i = 1; i < 10; i++) {
        DEBUG_PRINT("Circuit no. %zu has size %d", i, circuit_sizes[i]);
    }

    qsort(circuit_sizes, sizeof(circuit_sizes) / sizeof(uint32_t), sizeof(uint32_t),
          compare_uint32_t_desc);

    DEBUG_PRINT("Largest circuits: %u, %u, and %u\n", circuit_sizes[0], circuit_sizes[1],
                circuit_sizes[2]);

    const uint32_t result = circuit_sizes[0] * circuit_sizes[1] * circuit_sizes[2];

    vector_free(box_positions);
    vector_free(box_pairs);

    return result;
}