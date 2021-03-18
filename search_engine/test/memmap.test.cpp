#include <fstream> // ofstream
#include <iostream> // ios_base
#include <string_view> // string_view

#include <gtest/gtest.h>

#include <search_engine/memmap.hpp>

using std::ios_base, std::ofstream, std::string_view;

TEST(MemmapTest, Empty) {
    static constexpr const char *filename = "empty.txt";
    ofstream(filename, ios_base::binary | ios_base::out | ios_base::trunc);
    const memmap map(filename);
    ASSERT_EQ(map.data(), nullptr);
    ASSERT_TRUE(map.empty());
    ASSERT_TRUE(map.is_open());
    ASSERT_EQ(static_cast<string_view>(map), string_view());
    ASSERT_EQ(map.size(), 0);
}

TEST(MemmapTest, Pangram) {
    static constexpr string_view data =
        "The quick brown fox jumps over the lazy dog.\n"
        "Съешь еще этих мягких французских булок, да выпей чаю.\n"
        "いろはにほへと\nちりぬるを\nわかよたれそ\nつねならむ\nうゐのおくやま\n"
        "けふこえて\nあさきゆめみし\nゑひもせす\n";
    static constexpr const char *filename = "pangram.txt";
    ofstream(
        filename,
        ios_base::binary | ios_base::out | ios_base::trunc
    ).write(data.data(), data.size());

    const memmap map(filename);
    ASSERT_NE(map.data(), nullptr);
    ASSERT_STREQ(map.data(), data.data());
    ASSERT_FALSE(map.empty());
    ASSERT_TRUE(map.is_open());
    ASSERT_EQ(static_cast<string_view>(map), data);
    ASSERT_EQ(map.size(), data.size());
}
