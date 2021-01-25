#include <stdexcept> // logic_error
#include <string> // string, wstring
#include <string_view> // string_view, wstring_view

#include <gtest/gtest.h>

#include <search_engine/encoder.hpp>

using std::logic_error, std::string, std::string_view, std::wstring,
    std::wstring_view;

static constexpr wstring encode_string(string_view);
static constexpr string encode_wstring(wstring_view);

TEST(EncoderTest, EncodeString) {
    ASSERT_EQ(encode_string(
        "The quick brown fox jumps over the lazy dog."
        ), L"The quick brown fox jumps over the lazy dog."
    );
    ASSERT_EQ(encode_string(
        "Съешь еще этих мягких французских булок, да выпей чаю."
        ), L"Съешь еще этих мягких французских булок, да выпей чаю."
    );
}

TEST(EncoderTest, EncodeWstring) {
    ASSERT_EQ(encode_wstring(
        L"The quick brown fox jumps over the lazy dog."
        ), "The quick brown fox jumps over the lazy dog."
    );
    ASSERT_EQ(encode_wstring(
        L"Съешь еще этих мягких французских булок, да выпей чаю."
        ), "Съешь еще этих мягких французских булок, да выпей чаю."
    );
}

TEST(EncoderTest, Throw) {
/*
    ASSERT_THROW(parse_string("\"\\"), logic_error);
    ASSERT_THROW(parse_string("\"\\\""), logic_error);
    ASSERT_THROW(parse_string("\"\\u007F\""), logic_error);
    ASSERT_THROW(parse_string("\"\\u00ff\""), logic_error);
*/
}

static constexpr wstring encode_string(const string_view str) {
    wstring wbuffer;
    encoder string_encoder([&wbuffer](const wchar_t wc) constexpr -> void {
        wbuffer.push_back(wc);
    });
    foreach(str.cbegin(), str.cend(), string_encoder);
    return wbuffer;
}

static constexpr string encode_wstring(const wstring_view wstr) {
    string buffer;
    encoder string_encoder([&buffer](const char c) constexpr -> void {
        buffer.push_back(c);
    });
    foreach(wstr.cbegin(), wstr.cend(), string_encoder);
    return buffer;
}
