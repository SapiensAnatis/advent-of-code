//
// Created by jay on 09/03/2026.
//

#ifndef AOC2025_LIB_HASH_SET_H
#define AOC2025_LIB_HASH_SET_H

#include "lib/hash_fns.h"
#include "lib/hash_map.h"

/*
 * A hash set is essentially just wrapper over a hash map, where we re-use the hashing properties
 * applied to hash map keys to store hash set values, but store dummy data in the hash map values.
 */

struct HashSet {
    struct HashMap hash_map;
};

struct HashSetIterator {
    const struct HashMap* hash_map;
    struct HashMapIterator hash_map_iterator;
    void* current_value;
};

struct HashSet hash_set_create(size_t value_size, HashFn hash_fn, EqualFn equal_fn);

bool hash_set_contains(const struct HashSet* hash_set, const void* value);

bool hash_set_try_add(struct HashSet* hash_set, const void* value);

void hash_set_ensure_capacity(struct HashSet* hash_set, size_t capacity);

size_t hash_set_size(const struct HashSet* hash_set);

bool hash_set_try_remove(struct HashSet* hash_set, const void* value);

void hash_set_free(struct HashSet* hash_set);

struct HashSetIterator hash_set_iterator_create(const struct HashSet* hash_set);

bool hash_set_iterator_move_next(struct HashSetIterator* iter);

#endif // AOC2025_LIB_HASH_SET_H
