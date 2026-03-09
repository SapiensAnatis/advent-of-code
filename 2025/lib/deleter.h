//
// Created by jay on 09/03/2026.
//

#ifndef AOC2025_LIB_DELETER_H
#define AOC2025_LIB_DELETER_H

/**
 * @brief A deleter function is to be used by containers to free elements of a statically unknown
 * type at runtime when the container is resized or when the container is itself freed.
 */
typedef void (*Deleter)(void*);

/**
 * @brief The default deleter.
 *
 * @note Containers can use this sentinel value to understand that no special deletion behaviour is
 * required (as is the case when the data to be deleted does not own any pointers).
 */
static constexpr Deleter DEFAULT_DELETER = nullptr;

#endif // AOC2025_LIB_DELETER_H
