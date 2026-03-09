//
// Created by jay on 09/03/2026.
//
#include "lib/hash_map.h"

#include "lib/debug.h"
#include "lib/fatal_error.h"
#include "lib/vector.h"

#include <inttypes.h>
#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

static constexpr size_t INITIAL_BUCKETS_SIZE = 10;
static constexpr double RESIZE_LOAD_FACTOR = 0.75; // from Java

static size_t get_bucket_index(const size_t buckets_len, const HashFn hash_fn, const void* key) {
    const size_t hash = hash_fn(key);
    const size_t index = hash % buckets_len;
    return index;
}

static struct HashMapBucket* hash_map_get_bucket(const struct HashMap* hash_map, const void* key) {
    const size_t index = get_bucket_index(hash_map->buckets_len, hash_map->hash_fn, key);
    return &hash_map->buckets[index];
}

static void hash_map_entry_free(void* entry) {
    // DEBUG_PRINT("Destroying hash map entry at %p", entry);
    // TODO: allow specifying key and value deleters
    // until then, the below is correct-ish
    struct HashMapEntry* casted_entry = entry;
    free(casted_entry->key);
    free(casted_entry->value);
}

static void hash_map_buckets_free(struct HashMap* hash_map) {
    for (size_t i = 0; i < hash_map->buckets_len; i++) {
        const struct HashMapBucket* bucket = &hash_map->buckets[i];
        vector_free(bucket->entries);
    }

    free(hash_map->buckets);
}

static struct HashMapEntry hash_map_create_new_entry(const struct HashMap* hash_map,
                                                     const void* key, const void* value) {
    struct HashMapEntry new_entry;
    new_entry.key = malloc(hash_map->key_size);
    new_entry.value = malloc(hash_map->value_size);

    if (new_entry.key == nullptr || new_entry.value == nullptr) {
        FATAL_ERROR("Failed to allocate hash map entry key and value");
    }

    memcpy(new_entry.key, key, hash_map->key_size);
    memcpy(new_entry.value, value, hash_map->value_size);

    return new_entry;
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

    // n.b. MUST use calloc, as hash_map_free will access each bucket and try to free the
    // owned vector, if uninitialised buckets are not zeroed then that is UB.
    struct HashMapBucket* hash_map_buckets =
        calloc(INITIAL_BUCKETS_SIZE, sizeof(struct HashMapBucket));
    if (hash_map_buckets == nullptr) {
        FATAL_ERROR("Failed to allocate hash map buckets");
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
 * @param out_value If the value is found, the destination to write it to. Must not be
 * nullptr.
 * @return A boolean indicating whether the value was found.
 */
bool hash_map_try_get(const struct HashMap* hash_map, const void* key, void* out_value) {
    const struct HashMapBucket* bucket = hash_map_get_bucket(hash_map, key);
    if (bucket->entries == nullptr) {
        return false;
    }

    for (size_t i = 0; i < vector_size(bucket->entries); i++) {
        const struct HashMapEntry* entry = vector_at(bucket->entries, i);
        if (hash_map->equal_fn(key, entry->key)) {
            // DEBUG_PRINT("Found value in hash map");
            memcpy(out_value, entry->value, hash_map->value_size);
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
 * @return A boolean - if true the value was added, if false, the value already existed in
 * the hash map.
 */
bool hash_map_try_add(struct HashMap* hash_map, const void* key, const void* value) {
    struct HashMapBucket* bucket = hash_map_get_bucket(hash_map, key);

    if (bucket->entries == nullptr) {
        bucket->entries = vector_create(sizeof(struct HashMapEntry), hash_map_entry_free);
    }

    for (size_t i = 0; i < vector_size(bucket->entries); i++) {
        const struct HashMapEntry* entry = vector_at(bucket->entries, i);
        if (hash_map->equal_fn(key, entry->key)) {
            // Value already exists
            DEBUG_PRINT("Value to add already existed in hash map");
            return false;
        }
    }

    const struct HashMapEntry new_entry = hash_map_create_new_entry(hash_map, key, value);
    vector_append(bucket->entries, &new_entry);

    hash_map->total_values_count += 1;

    // Only bother doing the resize routine after writing

    const double current_load =
        (double)hash_map->total_values_count / (double)hash_map->buckets_len;

    if (current_load > RESIZE_LOAD_FACTOR) {
        DEBUG_PRINT("Resizing hash map due to exceeded load factor: current = %.3f (%zu entries / "
                    "%zu buckets)",
                    current_load, hash_map->total_values_count, hash_map->buckets_len);

        const size_t new_buckets_len = hash_map->buckets_len * 2;
        DEBUG_PRINT("New size: %zu buckets", new_buckets_len);

        hash_map_ensure_capacity(hash_map, new_buckets_len);
    }

    return true;
}

/**
 * Ensure that the hash map has at least a given number of buckets.
 * @param hash_map The hash map to potentially resize.
 * @param capacity The desired bucket count.
 */
void hash_map_ensure_capacity(struct HashMap* hash_map, const size_t capacity) {
    size_t new_buckets_len = hash_map->buckets_len;
    while (new_buckets_len != 0 && new_buckets_len < capacity) {
        new_buckets_len *= 2;
    }

    if (new_buckets_len <= hash_map->buckets_len) {
        return;
    }

    struct HashMapBucket* new_buckets = calloc(new_buckets_len, sizeof(struct HashMapBucket));
    if (new_buckets == nullptr) {
        FATAL_ERROR("Failed to reallocate hash map buckets");
    }

    hash_map->total_values_count = 0;

    for (size_t i = 0; i < hash_map->buckets_len; i++) {
        const struct HashMapBucket* bucket = &hash_map->buckets[i];

        if (bucket->entries == nullptr) {
            continue; // No work to do
        }

        while (vector_size(bucket->entries) > 0) {
            struct HashMapEntry old_entry;
            vector_pop(bucket->entries, &old_entry);

            const size_t new_bucket_idx =
                get_bucket_index(new_buckets_len, hash_map->hash_fn, old_entry.key);
            struct HashMapBucket* new_bucket = &new_buckets[new_bucket_idx];

            if (new_bucket->entries == nullptr) {
                new_bucket->entries =
                    vector_create(sizeof(struct HashMapEntry), hash_map_entry_free);
            }

            // Move semantics!!!!
            // This is safe because we are shrinking the vector's length, so hash_map_buckets_free
            // will not free these entries
            vector_append(new_bucket->entries, &old_entry);
            hash_map->total_values_count += 1;
        }
    }

    hash_map_buckets_free(hash_map);
    hash_map->buckets = new_buckets;
    hash_map->buckets_len = new_buckets_len;
}

/**
 * Frees a hash map. The provided map must not be used after calling this function.
 * @param hash_map The hash map to free.
 */
void hash_map_free(struct HashMap* hash_map) {
    hash_map_buckets_free(hash_map);
    memset(hash_map, 0, sizeof(struct HashMap));
    // free(hash_map); stack allocated
}

size_t hash_int32(const void* value) { return *(int32_t*)value; }
bool equal_int32(const void* value1, const void* value2) {
    return (*(int32_t*)value1) == (*(int32_t*)value2);
}