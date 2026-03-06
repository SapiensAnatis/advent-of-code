#include "day01/day01.h"
#include "lib/debug.h"
#include "lib/file.h"

#include <assert.h>
#include <stdio.h>
#include <stdlib.h>

void day01_part1(FILE* file) {
    char line_buffer[1024];
    long dial_position = 50;
    int dial_zero_counter = 0;

    while (read_line(file, line_buffer, sizeof(line_buffer))) {
        char direction_char = line_buffer[0];
        long magnitude = strtol(line_buffer + 1, nullptr, 10);
        long right_magnitude = 0;

        if (direction_char == 'L') {
            right_magnitude = 100 - (magnitude % 100);
        } else if (direction_char == 'R') {
            right_magnitude = magnitude;
        }

        long new_dial_position = (dial_position + right_magnitude) % 100;

        DEBUG_PRINT("Moving dial from %ld to %ld", dial_position, new_dial_position);
        dial_position = new_dial_position;

        if (dial_position == 0) {
            DEBUG_PRINT("Dial is at zero");
            dial_zero_counter += 1;
        }
    }

    printf("Dial zero count: %d\n", dial_zero_counter);
}

void day01_part2(FILE* file) {
    char line_buffer[1024];
    long dial_position = 50;
    long dial_zero_counter = 0;

    while (read_line(file, line_buffer, sizeof(line_buffer))) {
        DEBUG_PRINT("Processing instruction %s", line_buffer);

        char direction_char = line_buffer[0];
        long magnitude = strtol(line_buffer + 1, nullptr, 10);
        long direction = 0;
        long local_dial_zero_counter = 0;

        if (direction_char == 'L') {
            direction = -1;
        } else if (direction_char == 'R') {
            direction = 1;
        } else {
            assert(false && "invalid direction");
        }

        DEBUG_PRINT("Current dial position: %ld", dial_position);

        for (int i = 0; i < magnitude; i++) {
            dial_position += direction;

            if (dial_position == 100) {
                dial_position = 0;
            }
            if (dial_position == -1) {
                dial_position = 99;
            }

            if (dial_position == 0) {
                local_dial_zero_counter += 1;
            }
        }

        DEBUG_PRINT("New dial position: %ld", dial_position);
        DEBUG_PRINT("Zero crossings: %ld", local_dial_zero_counter);

        dial_zero_counter += local_dial_zero_counter;
    }

    printf("Dial zero count: %ld\n", dial_zero_counter);
}
