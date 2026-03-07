#ifndef AOC2025_LIB_VECTOR_H
#define AOC2025_LIB_VECTOR_H

#include <stddef.h>
#include <stdlib.h>

struct Vector;

struct Vector* vector_create(size_t element_size);

void* vector_at(const struct Vector* vector, size_t index);

void vector_append(struct Vector* vector, const void* element);

void vector_append_range(struct Vector* vector, const void* elements, size_t elements_size);

size_t vector_size(const struct Vector* vector);

void* vector_data(struct Vector* vector);

void* vector_pop(struct Vector* vector);

void vector_free(struct Vector* vector);

#endif // AOC2025_LIB_VECTOR_H
