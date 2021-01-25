#include <stdexcept> // logic_error
#include <string> // string
#include <string_view> // string_view

#include <gtest/gtest.h>

#include <search_engine/str_parser.hpp>

using std::logic_error, std::string, std::string_view;

static string parse_string(string_view);

TEST(StrParserTest, Pangram) {
    ASSERT_EQ(parse_string(
        "\"The quick brown fox jumps over the lazy dog.\""
        ), "The quick brown fox jumps over the lazy dog."
    );
    ASSERT_EQ(parse_string(
        "\"Съешь еще этих мягких французских булок, да выпей чаю.\""
        ), "Съешь еще этих мягких французских булок, да выпей чаю."
    );
}

TEST(StrParserTest, ParseEscape) {
    ASSERT_EQ(parse_string("\"\""), "");
    ASSERT_EQ(parse_string("\"\\u0000\""), string(1, '\0'));
    ASSERT_EQ(parse_string("\"\\u0001\""), "\x01");
    ASSERT_EQ(parse_string("\"\\u0002\""), "\x02");
    ASSERT_EQ(parse_string("\"\\u0003\""), "\x03");
    ASSERT_EQ(parse_string("\"\\u0004\""), "\x04");
    ASSERT_EQ(parse_string("\"\\u0005\""), "\x05");
    ASSERT_EQ(parse_string("\"\\u0006\""), "\x06");
    ASSERT_EQ(parse_string("\"\\u0007\""), "\x07");
    ASSERT_EQ(parse_string(    "\"\\b\""),   "\b");
    ASSERT_EQ(parse_string(    "\"\\t\""),   "\t");
    ASSERT_EQ(parse_string(    "\"\\n\""),   "\n");
    ASSERT_EQ(parse_string("\"\\u000b\""), "\x0B");
    ASSERT_EQ(parse_string(    "\"\\f\""),   "\f");
    ASSERT_EQ(parse_string(    "\"\\r\""),   "\r");
    ASSERT_EQ(parse_string("\"\\u000e\""), "\x0E");
    ASSERT_EQ(parse_string("\"\\u000f\""), "\x0F");
    ASSERT_EQ(parse_string("\"\\u0010\""), "\x10");
    ASSERT_EQ(parse_string("\"\\u0011\""), "\x11");
    ASSERT_EQ(parse_string("\"\\u0012\""), "\x12");
    ASSERT_EQ(parse_string("\"\\u0013\""), "\x13");
    ASSERT_EQ(parse_string("\"\\u0014\""), "\x14");
    ASSERT_EQ(parse_string("\"\\u0015\""), "\x15");
    ASSERT_EQ(parse_string("\"\\u0016\""), "\x16");
    ASSERT_EQ(parse_string("\"\\u0017\""), "\x17");
    ASSERT_EQ(parse_string("\"\\u0018\""), "\x18");
    ASSERT_EQ(parse_string("\"\\u0019\""), "\x19");
    ASSERT_EQ(parse_string("\"\\u001a\""), "\x1A");
    ASSERT_EQ(parse_string("\"\\u001b\""), "\x1B");
    ASSERT_EQ(parse_string("\"\\u001c\""), "\x1C");
    ASSERT_EQ(parse_string("\"\\u001d\""), "\x1D");
    ASSERT_EQ(parse_string("\"\\u001e\""), "\x1E");
    ASSERT_EQ(parse_string("\"\\u001f\""), "\x1F");
    ASSERT_EQ(parse_string("\"\\u007f\""), "\x7F");
}

TEST(StrParserTest, Throw) {
    ASSERT_THROW(parse_string("\"\\"), logic_error);
    ASSERT_THROW(parse_string("\"\\\""), logic_error);
    ASSERT_THROW(parse_string("\"\\u0008\""), logic_error);
    ASSERT_THROW(parse_string("\"\\u0009\""), logic_error);
    ASSERT_THROW(parse_string("\"\\u000a\""), logic_error);
    ASSERT_THROW(parse_string("\"\\u000c\""), logic_error);
    ASSERT_THROW(parse_string("\"\\u000d\""), logic_error);
    ASSERT_THROW(parse_string("\"\\u007F\""), logic_error);
    ASSERT_THROW(parse_string("\"\\u0080\""), logic_error);
    ASSERT_THROW(parse_string("\"\\u00ff\""), logic_error);
}

static string parse_string(const string_view str) {
    string buffer;
    str_parser parser([&buffer](const char c) constexpr -> void {
        buffer.push_back(c);
    });
    parser.get_invocable();
    parser(str.cbegin(), str.cend());
    return buffer;
}
