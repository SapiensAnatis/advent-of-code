//
// Created by jay on 09/03/2026.
//

#ifndef AOC2025_LIB_FATAL_ERROR_H
#define AOC2025_LIB_FATAL_ERROR_H

#ifdef __GNUC__
#define FATAL_ERROR_NORETURN __attribute__((noreturn))
#else
#define FATAL_ERROR_NORETURN
#endif

void fatal_error(const char* file, int line, const char* message) FATAL_ERROR_NORETURN;

#define FATAL_ERROR(reason)                                                                        \
    do {                                                                                           \
        fatal_error(__FILE__, __LINE__, reason);                                                   \
    } while (0)

#endif // AOC2025_LIB_FATAL_ERROR_H
