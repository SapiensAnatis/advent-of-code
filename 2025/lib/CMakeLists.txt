add_library(aoc2025_lib)

option(DEBUG_PRINT_ENABLED "Enable debug print" 0)

if (${DEBUG_PRINT_ENABLED})
    target_compile_definitions(aoc2025_lib PUBLIC DEBUG_PRINT_ENABLED)
endif ()

target_sources(aoc2025_lib
        PRIVATE
        debug.c
        fatal_error.c
        file.c
        grid.c
        hash_fns.c
        hash_map.c
        hash_set.c
        range.c
        string.c
        string_split.c
        string_view.c
        vector.c

        PUBLIC
        FILE_SET HEADERS
        BASE_DIRS ${CMAKE_CURRENT_LIST_DIR}/..
        FILES
        debug.h
        deleter.h
        fatal_error.h
        file.h
        grid.h
        hash_fns.h
        hash_map.h
        hash_set.h
        range.h
        string.h
        string_split.h
        string_view.h
        vector.h
)
