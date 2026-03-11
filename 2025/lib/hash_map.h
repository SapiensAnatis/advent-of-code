//
// Created by jay on 09/03/2026.
//

#ifndef AOC2025_LIB_HASH_MAP_H
#define AOC2025_LIB_HASH_MAP_H

#include "lib/hash_fns.h"
#include "lib/vector.h"

#include <stdint.h>

struct HashMapEntry {
    void* key;
    void* value;
};

struct HashMapBucket {
    struct Vector* entries; // vector<HashMapEntry>
};

struct HashMap {
    struct HashMapBucket* buckets;
    size_t buckets_len;
    size_t total_values_count;
    const size_t key_size;
    const size_t value_size;
    const HashFn hash_fn;
    const EqualFn equal_fn;
};

struct HashMapIterator {
    const struct HashMap* hash_map;
    size_t bucket_idx;
    size_t bucket_entry_idx;
    void* current_key;
    void* current_value;
};

struct HashMap hash_map_create(size_t key_size, size_t value_size, HashFn hash_fn,
                               EqualFn equal_fn);

bool hash_map_try_get(const struct HashMap* hash_map, const void* key, void* out_value);

void* hash_map_get_value_ptr(struct HashMap* hash_map, const void* key);

void* hash_map_get_value_ptr_or_add_default(struct HashMap* hash_map, const void* key);

bool hash_map_contains_key(const struct HashMap* hash_map, const void* key);

bool hash_map_try_add(struct HashMap* hash_map, const void* key, const void* value);

void hash_map_ensure_capacity(struct HashMap* hash_map, size_t capacity);

size_t hash_map_size(const struct HashMap* hash_map);

bool hash_map_try_remove(struct HashMap* hash_map, const void* key);

void hash_map_free(struct HashMap* hash_map);

struct HashMapIterator hash_map_iterator_create(const struct HashMap* hash_map);

bool hash_map_iterator_move_next(struct HashMapIterator* iter);

#endif // AOC2025_LIB_HASH_MAP_H
