#include <functional> // function
#include <string> // basic_string
#include <system_error> // system_error

#include <gtest/gtest.h>

#include <search_engine/str_encoder.hpp>

using std::function, std::basic_string, std::system_error;

template<typename From, typename To>
static basic_string<To> convert(const basic_string<From> &);

TEST(StrEncoderTest, Identity) {
    ASSERT_EQ((convert<char, char>(
        "The quick brown fox jumps over the lazy dog."
        )), "The quick brown fox jumps over the lazy dog."
    );
    ASSERT_EQ((convert<char, char>(
        "Съешь еще этих мягких французских булок, да выпей чаю."
        )), "Съешь еще этих мягких французских булок, да выпей чаю."
    );
    ASSERT_EQ((convert<wchar_t, wchar_t>(
        L"The quick brown fox jumps over the lazy dog."
        )), L"The quick brown fox jumps over the lazy dog."
    );
    ASSERT_EQ((convert<wchar_t, wchar_t>(
        L"Съешь еще этих мягких французских булок, да выпей чаю."
        )), L"Съешь еще этих мягких французских булок, да выпей чаю."
    );
}

TEST(StrEncoderTest, EncodeString) {
    ASSERT_EQ((convert<char, wchar_t>(
        "The quick brown fox jumps over the lazy dog."
        )), L"The quick brown fox jumps over the lazy dog."
    );
    ASSERT_EQ((convert<char, wchar_t>(
        "Съешь еще этих мягких французских булок, да выпей чаю."
        )), L"Съешь еще этих мягких французских булок, да выпей чаю."
    );
    ASSERT_EQ((convert<char, wchar_t>(
        "\U00010000\U00010001\U00010002\U00010003"
        )), L"\U00010000\U00010001\U00010002\U00010003"
    );
}

TEST(StrEncoderTest, EncodeWstring) {
    ASSERT_EQ((convert<wchar_t, char>(
        L"The quick brown fox jumps over the lazy dog."
        )), "The quick brown fox jumps over the lazy dog."
    );
    ASSERT_EQ((convert<wchar_t, char>(
        L"Съешь еще этих мягких французских булок, да выпей чаю."
        )), "Съешь еще этих мягких французских булок, да выпей чаю."
    );
    ASSERT_EQ((convert<wchar_t, char>(
        L"\U00010000\U00010001\U00010002\U00010003"
        )), "\U00010000\U00010001\U00010002\U00010003"
    );
}

TEST(StrEncoderTest, Throw) {
    ASSERT_THROW((convert<char, wchar_t>("\x80")), system_error);
    ASSERT_THROW((convert<char, wchar_t>("\xC0\x80")), system_error);
    ASSERT_THROW((convert<char, wchar_t>("\xE0\x80\x80")), system_error);
    ASSERT_THROW((convert<char, wchar_t>("\xF0\x80\x80\x80")), system_error);
}

template<typename From, typename To>
static basic_string<To> convert(const basic_string<From> &str) {
    basic_string<To> buffer;
    str_encoder<From, To, function<void(basic_string<To> &)>> encoder(
        [&buffer](basic_string<To> &s) constexpr -> void {
            buffer = s;
        }
    );
    encoder(str);

    return buffer;
}