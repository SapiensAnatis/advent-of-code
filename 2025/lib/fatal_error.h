//
// Created by jay on 09/03/2026.
//

#ifndef AOC2025_LIB_FATAL_ERROR_H
#define AOC2025_LIB_FATAL_ERROR_H

[[noreturn]] void fatal_error(const char* file, int line, const char* message);

#define FATAL_ERROR(reason)                                                                        \
    do {                                                                                           \
        fatal_error(__FILE__, __LINE__, reason);                                                   \
    } while (0)

#endif // AOC2025_LIB_FATAL_ERROR_H
