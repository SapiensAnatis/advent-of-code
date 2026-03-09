extern "C" {
#include "lib/hash_map.h"
#include "lib/string_split.h"
#include "lib/string_view.h"
}

#include <cstdint>
#include <cstdlib>
#include <memory>

#include "gtest/gtest.h"

namespace {

std::unique_ptr<char, decltype(&free)> segment_to_string(const struct StringSplitIterator* iter) {
    return {string_view_to_string(&iter->current_segment), free};
}

} // namespace

TEST(StringSplitIterator, SplitsStrings) {
    struct StringSplitIterator iter =
        string_split_create("Hello world !", " ", STRING_SPLIT_DEFAULT);

    auto current = segment_to_string(&iter);
    EXPECT_STREQ("Hello", current.get());

    EXPECT_TRUE(string_split_move_next(&iter));
    current = segment_to_string(&iter);
    EXPECT_STREQ("world", current.get());

    EXPECT_TRUE(string_split_move_next(&iter));
    current = segment_to_string(&iter);
    EXPECT_STREQ("!", current.get());
}

TEST(StringSplitIterator, RemovesEmptyEntries) {
    struct StringSplitIterator iter =
        string_split_create("Hello     world      ! ", " ", STRING_SPLIT_REMOVE_EMPTY_ENTRIES);
    auto current = segment_to_string(&iter);
    EXPECT_STREQ("Hello", current.get());

    EXPECT_TRUE(string_split_move_next(&iter));
    current = segment_to_string(&iter);
    EXPECT_STREQ("world", current.get());

    EXPECT_TRUE(string_split_move_next(&iter));
    current = segment_to_string(&iter);
    EXPECT_STREQ("!", current.get());

    EXPECT_FALSE(string_split_move_next(&iter));
}

TEST(HashMap, GetAndRetrieveValues) {
    struct HashMap hash_map =
        hash_map_create(sizeof(int32_t), sizeof(int32_t), hash_int32, equal_int32);

    for (int32_t i = 0; i < 100; i++) {
        const int32_t key = i;
        const int32_t value = i + 1;

        const bool added = hash_map_try_add(&hash_map, &key, &value);
        ASSERT_TRUE(added);
    }

    // Separate loop: make sure we didn't lose anything during resizing

    for (int32_t i = 0; i < 100; i++) {
        const int32_t key = i;
        const int32_t expected_value = i + 1;
        int32_t value = 0;

        const bool retrieved = hash_map_try_get(&hash_map, &key, &value);
        ASSERT_TRUE(retrieved);

        ASSERT_EQ(expected_value, value);
    }

    hash_map_free(&hash_map);
}

TEST(HashMap, EnsureCapacity) {
    struct HashMap hash_map =
        hash_map_create(sizeof(int32_t), sizeof(int32_t), hash_int32, equal_int32);

    hash_map_ensure_capacity(&hash_map, 100);

    const size_t initial_bucket_len = hash_map.buckets_len;

    for (int32_t i = 0; i < 100; i++) {
        const int32_t key = i;
        const int32_t value = i + 1;

        const bool added = hash_map_try_add(&hash_map, &key, &value);
        ASSERT_TRUE(added);
    }

    ASSERT_EQ(initial_bucket_len, hash_map.buckets_len);

    hash_map_free(&hash_map);
}