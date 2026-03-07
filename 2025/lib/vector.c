#include "lib/vector.h"
#include "lib/debug.h"

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

struct Vector* vector_create(const size_t element_size) {
    DEBUG_PRINT("Creating new vector with element_size %ld", element_size);

    struct Vector* result = malloc(sizeof(struct Vector));
    if (result == nullptr) {
        assert(false && "failed to allocate struct Vector");
        abort();
    }

    void* data = malloc(element_size * VECTOR_INITIAL_CAPACITY);
    if (data == nullptr) {
        assert(false && "failed to allocate vector data");
        abort();
    }

    result->data = data;
    result->size = 0;
    result->capacity = VECTOR_INITIAL_CAPACITY;
    result->element_size = element_size;

    return result;
}

void* vector_at(const struct Vector* vector, const size_t index) {
    if (index >= vector->size) {
        assert(false && "out of bounds access to vector");
        abort();
    }

    void* element_ptr = ((char*)vector->data) + (index * vector->element_size);

    return element_ptr;
}

void vector_append(struct Vector* vector, const void* element) {
    vector_append_range(vector, element, 1);
}

void vector_append_range(struct Vector* vector, const void* elements, const size_t elements_size) {
    DEBUG_PRINT("Appending elements to vector at %p: %p -> +%ld", (void*)vector, elements,
                elements_size);

    size_t required_capacity = vector->size + elements_size;

    if (required_capacity >= vector->capacity) {
        size_t new_capacity = vector->capacity;
        while (new_capacity < required_capacity) {
            new_capacity *= 2;
        }

        DEBUG_PRINT("Expanding vector at %p: from %zu to %zu", (void*)vector, vector->capacity,
                    new_capacity);

        void* new_data = realloc(vector->data, new_capacity * vector->element_size);
        if (new_data == nullptr) {
            assert(false && "failed to allocate expanded vector data");
            abort();
        }

        vector->capacity = new_capacity;
        vector->data = new_data;
    }

    void* dest = ((char*)vector->data) + (vector->size * vector->element_size);

    memcpy(dest, elements, elements_size * vector->element_size);
    vector->size += elements_size;
}

size_t vector_size(const struct Vector* vector) { return vector->size; }

void* vector_data(struct Vector* vector) { return vector->data; }

/**
 * Remove an element from the end of the vector.
 * @param vector The vector to remove from.
 * @return The removed element.
 */
void* vector_pop(struct Vector* vector) {
    if (vector->size == 0) {
        assert(false && "attempted to pop from empty vector");
        abort();
    }
    void* element = vector_at(vector, vector->size - 1);
    vector->size -= 1;
    return element;
}

void vector_remove_at(struct Vector* vector, const size_t index) {
    if (index >= vector->size) {
        assert(false && "out of bounds access to vector");
        abort();
    }

    const size_t num_elements_to_move = vector->size - index - 1;

    if (num_elements_to_move > 0) {
        memmove(vector_at(vector, index), vector_at(vector, index + 1),
                num_elements_to_move * vector->element_size);
    }

    vector->size -= 1;
}

void vector_free(struct Vector* vector) {
    DEBUG_PRINT("Destroying vector at %p", (void*)vector);

    if (vector == nullptr) {
        return;
    }

    free(vector->data);
    free(vector);
}
