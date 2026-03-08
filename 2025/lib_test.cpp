extern "C" {
#include "lib/string_split.h"
#include "lib/string_view.h"
}

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