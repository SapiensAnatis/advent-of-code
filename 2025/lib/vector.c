#include "lib/debug.h"
#include "lib/vector.h"

#include <assert.h>
#include <stdlib.h>
#include <string.h>

struct Vector {
    void* data;
    size_t size;
    size_t capacity;
    size_t element_size;
};

static constexpr size_t VECTOR_INITIAL_CAPACITY = 4;

struct Vector* vector_create(size_t element_size) {
    DEBUG_PRINT("Creating new vector with element_size %d", element_size);

    struct Vector* result = malloc(sizeof(struct Vector));
    assert(result != nullptr && "failed to allocate vector");

    void* data = malloc(element_size * VECTOR_INITIAL_CAPACITY);
    assert(data != nullptr && "failed to allocate vector data");

    result->data = data;
    result->size = 0;
    result->capacity = VECTOR_INITIAL_CAPACITY;
    result->element_size = element_size;

    return result;
}

void* vector_at(const struct Vector* vector, size_t index) {
    assert(index < vector->size && "out of bounds accesss to vector");

    void* element_ptr = ((char*)vector->data) + (index * vector->element_size);

    return element_ptr;
}

void vector_append(struct Vector* vector, const void* element) {
    vector_append_range(vector, element, 1);
}

void vector_append_range(struct Vector* vector, const void* elements, size_t elements_size) {
    DEBUG_PRINT("Appending elements to vector at %p: %p -> +%d", vector, elements, elements_size);

    size_t required_capacity = vector->size + elements_size;

    if (required_capacity >= vector->capacity) {
        size_t new_capacity = vector->capacity;
        while (new_capacity < required_capacity) {
            new_capacity *= 2;
        }

        DEBUG_PRINT("Expanding vector at %p: from %zu to %zu", vector, vector->capacity,
                    new_capacity);

        void* new_data = realloc(vector->data, new_capacity * vector->element_size);
        assert(new_data != nullptr && "failed to allocate expanded vector data");

        vector->capacity = new_capacity;
        vector->data = new_data;
    }

    void* dest = ((char*)vector->data) + (vector->size * vector->element_size);

    memcpy(dest, elements, elements_size * vector->element_size);
    vector->size += elements_size;
}

size_t vector_size(const struct Vector* vector) { return vector->size; }

void* vector_data(struct Vector* vector) { return vector->data; }

void vector_pop(struct Vector* vector) {
    assert(vector->size > 0 && "attempted to pop from empty vector");
    vector->size -= 1;
}

void vector_free(struct Vector* vector) {
    DEBUG_PRINT("Destroying vector at %p", vector);

    free(vector->data);
    free(vector);
}
