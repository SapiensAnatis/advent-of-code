//
// Created by jay on 09/03/2026.
//
#include "lib/hash_map.h"

#include "lib/fatal_error.h"
#include "lib/vector.h"

#include <stdint.h>
#include <stdlib.h>
#include <string.h>

static constexpr size_t INITIAL_BUCKETS_SIZE = 10;

static struct HashMapBucket* get_bucket(const struct HashMap* hash_map, const void* key) {
    const size_t hash = hash_map->hash_fn(key);
    const size_t index = hash % hash_map->buckets_len;
    return &hash_map->buckets[index];
}

static void hash_map_entry_free(void* entry) {
    // TODO: allow specifying key and value deleters
    // until then, the below is correct-ish
    struct HashMapEntry* casted_entry = entry;
    free(casted_entry->key);
    free(casted_entry->value);
}

/**
 * Creates a new hash map.
 * @param key_size The size of the key type.
 * @param value_size The size of the value type.
 * @param hash_fn The function that should be used to hash keys.
 * @param equal_fn The function that should be used to compare the equality of keys.
 * @return A hash map struct.
 */
struct HashMap hash_map_create(const size_t key_size, const size_t value_size, const HashFn hash_fn,
                               const EqualFn equal_fn) {

    // n.b. MUST use calloc, as hash_map_free will access each bucket and try to free the owned
    // vector, if uninitialised buckets are not zeroed then that is UB.
    // ReSharper disable once CppDFAMemoryLeak - owned by hash map and freed in hash_map_free
    struct HashMapBucket* hash_map_buckets =
        calloc(INITIAL_BUCKETS_SIZE, sizeof(struct HashMapBucket));
    if (hash_map_buckets == nullptr) {
        FATAL_ERROR("failed to allocate hash map buckets");
    }

    const struct HashMap hash_map = {
        .buckets = hash_map_buckets,
        .buckets_len = INITIAL_BUCKETS_SIZE,
        .value_size = value_size,
        .key_size = key_size,
        .hash_fn = hash_fn,
        .equal_fn = equal_fn,
    };

    return hash_map;
}

/**
 * Tries to retrieve a value from the hash map with the given key.
 * @param hash_map The hash map in which to search for the key.
 * @param key The key to search for.
 * @param out_value If the value is found, the destination to write it to. Must not be nullptr.
 * @return A boolean indicating whether the value was found.
 */
bool hash_map_try_get(const struct HashMap* hash_map, const void* key, void* out_value) {
    const struct HashMapBucket* bucket = get_bucket(hash_map, key);
    if (bucket->elements == nullptr) {
        return false;
    }

    for (size_t i = 0; i < vector_size(bucket->elements); i++) {
        const struct HashMapEntry* entry = vector_at(bucket->elements, i);
        if (hash_map->equal_fn(&key, &entry->key)) {
            memcpy(out_value, &entry->value, hash_map->value_size);
            return true;
        }
    }

    return false;
}

/**
 * Tries to add a value to the hash map with the given key.
 * @param hash_map The hash map to add to.
 * @param key The key to add at.
 * @param value The value to add.
 * @return A boolean - if true the value was added, if false, the value already existed in the hash
 * map.
 */
bool hash_map_try_add(const struct HashMap* hash_map, const void* key, const void* value) {
    struct HashMapBucket* bucket = get_bucket(hash_map, key);

    if (bucket->elements == nullptr) {
        bucket->elements = vector_create(sizeof(struct HashMapEntry), hash_map_entry_free);
    }

    for (size_t i = 0; i < vector_size(bucket->elements); i++) {
        const struct HashMapEntry* entry = vector_at(bucket->elements, i);
        if (hash_map->equal_fn(&key, &entry->key)) {
            // Value already exists
            return false;
        }
    }

    vector_append(bucket->elements, value);
    return true;
}

/**
 * Frees a hash map. The provided map must not be used after calling this function.
 * @param hash_map The hash map to free.
 */
void hash_map_free(struct HashMap* hash_map) {
    for (size_t i = 0; i < hash_map->buckets_len; i++) {
        const struct HashMapBucket bucket = hash_map->buckets[i];
        vector_free(bucket.elements);
    }

    free(hash_map->buckets);
    memset(hash_map, 0, sizeof(struct HashMap));
    // free(hash_map); stack allocated
}

size_t hash_int32(const int32_t value) { return value; }
bool equal_int32(const int32_t value1, const int32_t value2) { return value1 == value2; }