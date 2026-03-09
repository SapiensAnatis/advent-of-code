add_library(aoc2025_lib)

option(DEBUG_PRINT_ENABLED "Enable debug print" 0)

if (${DEBUG_PRINT_ENABLED})
    target_compile_definitions(aoc2025_lib PUBLIC DEBUG_PRINT_ENABLED)
endif ()

target_sources(aoc2025_lib PRIVATE
        debug.h debug.c
        deleter.h
        fatal_error.h fatal_error.c
        file.h file.c
        grid.h grid.c
        hash_map.h hash_map.c
        range.h range.c
        string.h string.c
        string_split.h string_split.c
        string_view.h string_view.c
        vector.h vector.c
)

target_include_directories(aoc2025_lib PUBLIC ${CMAKE_CURRENT_LIST_DIR}/..)
