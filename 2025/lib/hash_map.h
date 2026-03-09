//
// Created by jay on 09/03/2026.
//

#ifndef AOC2025_LIB_HASH_MAP_H
#define AOC2025_LIB_HASH_MAP_H

#include "lib/vector.h"

#include <stdint.h>

struct HashMapEntry {
    void* key;
    void* value;
};

struct HashMapBucket {
    struct Vector* elements; // vector<HashMapEntry>
};

typedef size_t (*HashFn)(const void*);
typedef size_t (*EqualFn)(const void*, const void*);

struct HashMap {
    struct HashMapBucket* buckets;
    size_t buckets_len;
    const size_t key_size;
    const size_t value_size;
    const HashFn hash_fn;
    const EqualFn equal_fn;
};

struct HashMap hash_map_create(size_t key_size, size_t value_size, HashFn hash_fn,
                               EqualFn equal_fn);

bool hash_map_try_get(const struct HashMap* hash_map, const void* key, void* out_value);

bool hash_map_try_add(const struct HashMap* hash_map, const void* key, const void* value);

void hash_map_free(struct HashMap* hash_map);

/** Hash / equality helpers **/

size_t hash_int32(const int32_t value);
bool equal_int32(const int32_t value1, const int32_t value2);

#endif // AOC2025_LIB_HASH_MAP_H
