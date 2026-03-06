#include "lib/file.h"

#include "lib/string.h"

#include <assert.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

FILE* get_example_input(int day) {
    char filename[32];
    sprintf(filename, "./day%02d/example.txt", day);
    auto file = fopen(filename, "r");

    if (file == nullptr) {
        perror("get_example_input");
        abort();
    }

    return file;
}

FILE* get_real_input(int day) {
    char filename[32];
    sprintf(filename, "./day%02d/input.txt", day);
    auto file = fopen(filename, "r");

    if (file == nullptr) {
        perror("get_real_input");
        fprintf(stderr, "get_real_input: check the gitignored input file at %s exists", filename);
        abort();
    }

    return file;
}

struct String* read_all_text(FILE* file) {
    char internal_buffer[1024];

    struct String* result = string_create("");
    size_t bytes_read = 0;

    while ((bytes_read = fread(internal_buffer, sizeof(char), sizeof(internal_buffer) - 1, file)),
           bytes_read != 0) {
        assert(ferror(file) == 0 && "ferror detected");
        assert(bytes_read < sizeof(internal_buffer));
        internal_buffer[bytes_read] = '\0';
        string_append(result, internal_buffer);
    }

    return result;
}

bool read_line(FILE* file, char* buffer, size_t buffer_size) {
    char* newline_ptr = nullptr;
    char* write_position = buffer;
    [[maybe_unused]] size_t remaining_size = buffer_size;

    char internal_buffer[128];

    do {
        const char* fgets_result = fgets(internal_buffer, sizeof(internal_buffer), file);
        if (fgets_result == nullptr) {
            // Read error or EOF
            if (ferror(file)) {
                assert(false && "fgets error reading file");
                abort();
            }
            return false;
        }

        newline_ptr = strstr(internal_buffer, "\n");
        size_t read_size = strlen(internal_buffer);
        if (newline_ptr) {
            *newline_ptr = '\0'; // Don't include newline in returned string
        }

        assert(read_size < remaining_size && "buffer too small to contain line");

        memcpy(write_position, internal_buffer, read_size);
        write_position += read_size;
        remaining_size -= read_size;
    } while (newline_ptr == nullptr);

    return true;
}
