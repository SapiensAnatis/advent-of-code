configure_file("${CMAKE_CURRENT_SOURCE_DIR}/example.txt" "${CMAKE_CURRENT_BINARY_DIR}/example.txt" COPYONLY)
configure_file("${CMAKE_CURRENT_SOURCE_DIR}/example2.txt" "${CMAKE_CURRENT_BINARY_DIR}/example2.txt" COPYONLY)
configure_file("${CMAKE_CURRENT_SOURCE_DIR}/example3.txt" "${CMAKE_CURRENT_BINARY_DIR}/example3.txt" COPYONLY)
configure_file("${CMAKE_CURRENT_SOURCE_DIR}/example4.txt" "${CMAKE_CURRENT_BINARY_DIR}/example4.txt" COPYONLY)
configure_file("${CMAKE_CURRENT_SOURCE_DIR}/example5.txt" "${CMAKE_CURRENT_BINARY_DIR}/example5.txt" COPYONLY)
configure_file("${CMAKE_CURRENT_SOURCE_DIR}/example6.txt" "${CMAKE_CURRENT_BINARY_DIR}/example6.txt" COPYONLY)
configure_file("${CMAKE_CURRENT_SOURCE_DIR}/example7.txt" "${CMAKE_CURRENT_BINARY_DIR}/example7.txt" COPYONLY)

if (EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/input.txt")
    configure_file("${CMAKE_CURRENT_SOURCE_DIR}/input.txt" "${CMAKE_CURRENT_BINARY_DIR}/input.txt" COPYONLY)
endif ()

target_sources(aoc2024 PRIVATE
        day10.cpp
        day10.h
        day10_test.cpp
)
