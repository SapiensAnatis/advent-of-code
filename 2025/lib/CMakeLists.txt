add_library(aoc_lib)

option(DEBUG_PRINT_ENABLED "Enable debug print" 0)

if (${DEBUG_PRINT_ENABLED})
  target_compile_definitions(aoc_lib PUBLIC DEBUG_PRINT_ENABLED)
endif()

target_sources(aoc_lib PRIVATE
  debug.h debug.c
  file.h file.c
  string.h string.c
  string_split.h string_split.c
  string_view.h string_view.c
  vector.h vector.c
)
target_include_directories(aoc_lib PUBLIC ${CMAKE_CURRENT_LIST_DIR}/..)
