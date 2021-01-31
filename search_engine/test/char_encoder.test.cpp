#include <clocale> // LC_ALL, setlocale

#include <algorithm> // for_each
#include <functional> // function
#include <stdexcept> // runtime_error
#include <string> // basic_string
#include <string_view> // basic_string_view
#include <system_error> // system_error

#include <gtest/gtest.h>

#include <search_engine/char_encoder.hpp>

using std::basic_string, std::basic_string_view, std::for_each, std::function,
    std::runtime_error, std::setlocale, std::system_error;

template<typename From, typename To>
static basic_string<To> convert(basic_string_view<From>);

TEST(CharEncoderTest, Identity) {
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

TEST(CharEncoderTest, EncodeString) {
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

TEST(CharEncoderTest, EncodeWstring) {
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

TEST(CharEncoderTest, Throw) {
    ASSERT_THROW((convert<char, wchar_t>("\x80")), system_error);
    ASSERT_THROW((convert<char, wchar_t>("\xC0\x80")), system_error);
    ASSERT_THROW((convert<char, wchar_t>("\xE0\x80\x80")), system_error);
    ASSERT_THROW((convert<char, wchar_t>("\xF0\x80\x80\x80")), system_error);
}

template<typename From, typename To>
static basic_string<To> convert(const basic_string_view<From> str) {
    if (setlocale(LC_ALL, "en_US.utf8") == nullptr) [[unlikely]]
        throw runtime_error("convert: unable to set locale");

    basic_string<To> buffer;
    char_encoder<From, To, function<void (To)>> encoder(
        [&buffer](const To c) constexpr -> void {
            buffer.push_back(c);
        }
    );
    for_each(str.cbegin(), str.cend(), encoder);

    return buffer;
}
