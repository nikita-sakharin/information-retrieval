#include <algorithm> // for_each
#include <functional> // function
#include <system_error> // generic_category, system_error
#include <string> // string, wstring
#include <string_view> // string_view, wstring_view

#include <gtest/gtest.h>

#include <search_engine/encoder.hpp>

using std::for_each, std::function, std::string, std::string_view,
    std::system_error, std::wstring, std::wstring_view;

static wstring encode_string(string_view);
static string encode_wstring(wstring_view);

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
    ASSERT_THROW(encode_string("\x80"), system_error);
    ASSERT_THROW(encode_string("\xC0\x80"), system_error);
}

static wstring encode_string(const string_view str) {
    wstring wbuffer;
    encoder<char, wchar_t, function<void(wchar_t)>> string_encoder(
        [&wbuffer](const wchar_t wc) constexpr -> void {
            wbuffer.push_back(wc);
        }
    );
    for_each(str.cbegin(), str.cend(), string_encoder);

    return wbuffer;
}

static string encode_wstring(const wstring_view wstr) {
    string buffer;
    encoder<wchar_t, char, function<void(char)>> string_encoder(
        [&buffer](const char c) constexpr -> void {
            buffer.push_back(c);
        }
    );
    for_each(wstr.cbegin(), wstr.cend(), string_encoder);
    return buffer;
}
