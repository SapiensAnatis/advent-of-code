configure_file("${CMAKE_CURRENT_SOURCE_DIR}/example.txt" "${CMAKE_CURRENT_BINARY_DIR}/example.txt" COPYONLY)
configure_file("${CMAKE_CURRENT_SOURCE_DIR}/example2.txt" "${CMAKE_CURRENT_BINARY_DIR}/example2.txt" COPYONLY)
configure_file("${CMAKE_CURRENT_SOURCE_DIR}/example3.txt" "${CMAKE_CURRENT_BINARY_DIR}/example3.txt" COPYONLY)
configure_file("${CMAKE_CURRENT_SOURCE_DIR}/example4.txt" "${CMAKE_CURRENT_BINARY_DIR}/example4.txt" COPYONLY)
configure_file("${CMAKE_CURRENT_SOURCE_DIR}/example5.txt" "${CMAKE_CURRENT_BINARY_DIR}/example5.txt" COPYONLY)

if (EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/input.txt")
    configure_file("${CMAKE_CURRENT_SOURCE_DIR}/input.txt" "${CMAKE_CURRENT_BINARY_DIR}/input.txt" COPYONLY)
endif ()

target_sources(aoc2024 PRIVATE
        day12.cpp
        day12.h
        day12_test.cpp
)
