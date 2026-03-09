//
// Created by jay on 09/03/2026.
//

#include "fatal_error.h"

#include <stdio.h>
#include <stdlib.h>

void fatal_error(const char* file, const int line, const char* message) {
    fprintf(stderr, "[%s:%d] FATAL ERROR: '%s'", file, line, message);
    abort();
}