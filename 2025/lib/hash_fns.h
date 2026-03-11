//
// Created by jay on 10/03/2026.
//

#ifndef AOC2025_LIB_HASH_FNS_H
#define AOC2025_LIB_HASH_FNS_H

#include <stddef.h>

typedef size_t (*HashFn)(const void*);
typedef bool (*EqualFn)(const void*, const void*);

size_t hash_int32(const void* value);
bool equal_int32(const void* value1, const void* value2);

size_t hash_ptrdiff_t(const void* value);
bool equal_ptrdiff_t(const void* value1, const void* value2);

#endif // AOC2025_LIB_HASH_FNS_H
