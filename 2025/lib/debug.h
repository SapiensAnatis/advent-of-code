#ifndef AOC2025_LIB_DEBUG_H
#define AOC2025_LIB_DEBUG_H

// Define this in CMakePresets.json
// #define DEBUG_PRINT_ENABLED 1

void debug_print(const char* file, int line, const char* fmt, ...);

#ifdef DEBUG_PRINT_ENABLED
    #define DEBUG_PRINT(fmt, ...) \
        do { debug_print(__FILE__, __LINE__, fmt __VA_OPT__(,) __VA_ARGS__); } while (0)
#else
    #define DEBUG_PRINT(fmt, ...)  // Do nothing
#endif // DEBUG_PRINT_ENABLED

#endif // AOC2025_LIB_DEBUG_H
