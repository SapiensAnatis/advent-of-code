//
// Created by jay on 10/03/2026.
//

#include "lib/hash_fns.h"

#include <stddef.h>
#include <stdint.h>

size_t hash_int32(const void* value) {
    // Negative numbers will be reinterpreted as very large numbers
    return *(int32_t*)value;
}
bool equal_int32(const void* value1, const void* value2) {
    return *(int32_t*)value1 == *(int32_t*)value2;
}