#include <string_view> // string_view

#include <gtest/gtest.h>

#include <search_engine/memmap.hpp>

using std::string_view;

TEST(MemmapTest, Empty) {
    const memmap map("empty.txt");
    ASSERT_EQ(map.data(), nullptr);
    ASSERT_TRUE(map.empty());
    ASSERT_TRUE(map.is_open());
    ASSERT_EQ(static_cast<string_view>(map), string_view());
    ASSERT_EQ(map.size(), 0);
}

TEST(MemmapTest, Pangram) {
    static constexpr const char *data =
        "The quick brown fox jumps over the lazy dog.\n"
        "Съешь еще этих мягких французских булок, да выпей чаю.\n";
    const memmap map("pangram.txt");
    ASSERT_EQ(map.data(), data);
    ASSERT_TRUE(map.empty());
    ASSERT_TRUE(map.is_open());
    ASSERT_EQ(static_cast<string_view>(map), string_view(data));
    ASSERT_EQ(map.size(), 100U);
}
