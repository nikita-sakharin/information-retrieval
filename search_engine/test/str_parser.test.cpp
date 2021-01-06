#include <iostream>
#include <string>

#include <gtest/gtest.h>

#include <search_engine/str_parser.hpp>

static constexpr str_parser end;

TEST(StrParserTest, Hex) {
    const string_view str_0 = "\"\\u0000\"";
    str_parser iter_0(str_0);
    ASSERT_NE(iter_0, end);
    ASSERT_EQ('\0', *iter_0);
    ++iter_0;
    ASSERT_EQ(iter_0, end);

    const string_view str_b = "\"\\u000b\"";
    str_parser iter_b(str_b);
    ASSERT_NE(iter_b, end);
    ASSERT_EQ('\xB', *iter_b);
    ++iter_b;
    ASSERT_EQ(iter_b, end);

    const string_view str_e = "\"\\u000e\"";
    str_parser iter_e(str_e);
    ASSERT_NE(iter_e, end);
    ASSERT_EQ('\xE', *iter_e);
    ++iter_e;
    ASSERT_EQ(iter_e, end);

    const string_view str31 = "\"\\u001f\"";
    str_parser iter31(str31);
    ASSERT_NE(iter31, end);
    ASSERT_EQ('\x1F', *iter31);
    ++iter31;
    ASSERT_EQ(iter31, end);
}

TEST(StrParserTest, Hex) {
    const string_view str_error = "\"\\u007f";
    str_parser iter_error(str_error);
    ASSERT_NE(iter_error, end);
    ASSERT_EQ('\0', *iter_0);
    ++iter_0;
    ASSERT_EQ(iter_0, end);
}

TEST(StrParserTest, Pangram) {
    const string_view str = "\"The quick brown fox jumps over the lazy dog\"";
    str_parser iter(str);
    for (size_t i = 0; i < str.size(); ++i) {
        ASSERT_NE(iter, end);
        ASSERT_EQ(str[i], *iter);
        ++iter;
    }
    ASSERT_EQ(iter, end);
}
