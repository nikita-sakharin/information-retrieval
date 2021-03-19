#include <clocale> // LC_ALL, setlocale

#include <functional> // function
#include <stdexcept> // runtime_error
#include <string> // basic_string
#include <system_error> // system_error

#include <gtest/gtest.h>

#include <search_engine/str_encoder.hpp>

using std::basic_string;

template<typename From, typename To>
static basic_string<To> convert(const basic_string<From> &);

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
            "いろはにほへと\nちりぬるを\nわかよたれそ\nつねならむ\nうゐのおくやま\nけふこえて\n"
            "あさきゆめみし\nゑひもせす\n"
        )),
        L"いろはにほへと\nちりぬるを\nわかよたれそ\nつねならむ\nうゐのおくやま\nけふこえて\n"
        L"あさきゆめみし\nゑひもせす\n"
    );
    ASSERT_EQ((convert<char, wchar_t>(
            "\U00010000\U00010001\U00010002\U00010003"
            "\U00010004\U00010005\U00010006\U00010007"
        )),
        L"\U00010000\U00010001\U00010002\U00010003"
        L"\U00010004\U00010005\U00010006\U00010007"
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
            L"いろはにほへと\nちりぬるを\nわかよたれそ\nつねならむ\nうゐのおくやま\nけふこえて\n"
            L"あさきゆめみし\nゑひもせす\n"
        )),
        "いろはにほへと\nちりぬるを\nわかよたれそ\nつねならむ\nうゐのおくやま\nけふこえて\n"
        "あさきゆめみし\nゑひもせす\n"
    );
    ASSERT_EQ((convert<wchar_t, char>(
            L"\U00010000\U00010001\U00010002\U00010003"
            L"\U00010004\U00010005\U00010006\U00010007"
        )),
        "\U00010000\U00010001\U00010002\U00010003"
        "\U00010004\U00010005\U00010006\U00010007"
    );
}

TEST(StrEncoderTest, Throw) {
    using std::system_error;

    ASSERT_THROW((convert<char, wchar_t>("\x80")),             system_error);
    ASSERT_THROW((convert<char, wchar_t>("\xC0\x80")),         system_error);
    ASSERT_THROW((convert<char, wchar_t>("\xE0\x80\x80")),     system_error);
    ASSERT_THROW((convert<char, wchar_t>("\xF0\x80\x80\x80")), system_error);
    ASSERT_THROW((convert<char, wchar_t>("\xFF")),             system_error);

    ASSERT_THROW((convert<char, wchar_t>("\xC0")),             system_error);
    ASSERT_THROW((convert<char, wchar_t>("\xE0")),             system_error);
    ASSERT_THROW((convert<char, wchar_t>("\xE0\xBF")),         system_error);
    ASSERT_THROW((convert<char, wchar_t>("\xF0")),             system_error);
    ASSERT_THROW((convert<char, wchar_t>("\xF0\xBF")),         system_error);
    ASSERT_THROW((convert<char, wchar_t>("\xF0\xBF\xBF")),     system_error);
}

template<typename From, typename To>
static basic_string<To> convert(const basic_string<From> &str) {
    using std::function, std::runtime_error, std::setlocale;

    if (setlocale(LC_ALL, "en_US.utf8") == nullptr) [[unlikely]]
        throw runtime_error("convert: unable to set locale");

    basic_string<To> buffer;
    str_encoder<From, To, function<void(const basic_string<To> &)>> encoder(
        [&buffer](const basic_string<To> &s) constexpr -> void {
            buffer = s;
        }
    );
    encoder(str);

    return buffer;
}
