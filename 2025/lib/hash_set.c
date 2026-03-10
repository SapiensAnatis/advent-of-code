//
// Created by jay on 10/03/2026.
//

#include "lib/hash_set.h"

#include "lib/hash_fns.h"
#include "lib/hash_map.h"

#include <stddef.h>
#include <stdint.h>
#include <string.h>

static constexpr uint8_t HASH_MAP_VALUE = 2; // because why not

/**
 * Creates a new hash set.
 * @param value_size The size of the value type to store in the set.
 * @param hash_fn The function that should be used to hash values.
 * @param equal_fn The function that should be used to compare the equality of values.
 * @return A hash set struct.
 */
struct HashSet hash_set_create(const size_t value_size, const HashFn hash_fn,
                               const EqualFn equal_fn) {
    const size_t hash_map_key_size = value_size;

    return (struct HashSet){
        .hash_map = hash_map_create(hash_map_key_size, sizeof(HASH_MAP_VALUE), hash_fn, equal_fn)};
}

/**
 * Checks whether a value exists in the hash set.
 * @param hash_set The hash set to search.
 * @param value The value to search for.
 * @return True if the value exists in the hash set, false otherwise.
 */
bool hash_set_contains(const struct HashSet* hash_set, const void* value) {
    return hash_map_contains_key(&hash_set->hash_map, value);
}

/**
 * Tries to add a value to the hash set.
 * @param hash_set The hash set to add to.
 * @param value The value to add.
 * @return A boolean - if true the value was added, if false, the value already existed in
 * the hash set.
 */
bool hash_set_try_add(struct HashSet* hash_set, const void* value) {
    return hash_map_try_add(&hash_set->hash_map, value, &HASH_MAP_VALUE);
}

/**
 * Ensure that the hash set has at least a given number of buckets.
 * @param hash_set The hash set to potentially resize.
 * @param capacity The desired bucket count.
 */
void hash_set_ensure_capacity(struct HashSet* hash_set, const size_t capacity) {
    hash_map_ensure_capacity(&hash_set->hash_map, capacity);
}

/**
 * Gets the number of values in the provided hash set.
 * @param hash_set The hash set to get the size of.
 * @return The number of values in the hash set.
 */
size_t hash_set_size(const struct HashSet* hash_set) { return hash_map_size(&hash_set->hash_map); }

/**
 * Frees a hash set. The provided set must not be used after calling this function.
 * @param hash_set The hash set to free.
 */
void hash_set_free(struct HashSet* hash_set) {
    hash_map_free(&hash_set->hash_map);

    // Do not free hash_set; it is stack allocated
    memset(hash_set, 0, sizeof(*hash_set));
}

/**
 * Creates an iterator over the hash set's values.
 * @param hash_set The hash set to iterate over.
 * @return An iterator, whose current_value field can be used to access a value in the hash set.
 * @note The order that values in the set are yielded in is effectively random as it is based on
 * their hash and the size of the bucket array.
 * @note It is illegal to call this function with an empty hash set.
 */
struct HashSetIterator hash_set_iterator_create(const struct HashSet* hash_set) {
    const struct HashMapIterator hash_map_iterator = hash_map_iterator_create(&hash_set->hash_map);

    return (struct HashSetIterator){
        .hash_map = &hash_set->hash_map,
        .hash_map_iterator = hash_map_iterator,
        .current_value = hash_map_iterator.current_value,
    };
}

/**
 * Advances the iterator to the next value in the hash set.
 * @param iter The iterator to advance.
 * @return True if the iterator was advanced to a valid next value, false if there are no more
 * values to iterate over.
 */
bool hash_set_iterator_move_next(struct HashSetIterator* iter) {
    if (!hash_map_iterator_move_next(&iter->hash_map_iterator)) {
        return false;
    }

    iter->current_value = iter->hash_map_iterator.current_value;
    return true;
}