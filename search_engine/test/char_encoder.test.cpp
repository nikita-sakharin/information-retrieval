#include <cerrno> // EILSEQ
#include <clocale> // LC_ALL, setlocale

#include <functional> // function
#include <stdexcept> // runtime_error
#include <string> // basic_string
#include <string_view> // basic_string_view, string_view, wstring_view
#include <system_error> // generic_category, system_error

#include <gtest/gtest.h>

#include <search_engine/char_encoder.hpp>

using std::basic_string, std::basic_string_view, std::string_view,
    std::system_error, std::wstring_view;

template<typename From, typename To>
static basic_string<To> convert(basic_string_view<From>);

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
            "いろはにほへと\nちりぬるを\nわかよたれそ\nつねならむ\nうゐのおくやま\n"
            "けふこえて\nあさきゆめみし\nゑひもせす\n"
        )),
        L"いろはにほへと\nちりぬるを\nわかよたれそ\nつねならむ\nうゐのおくやま\n"
        L"けふこえて\nあさきゆめみし\nゑひもせす\n"
    );
    ASSERT_EQ((convert<char, wchar_t>(
            "\U00010000\U00010001\U00010002\U00010003"
            "\U00010004\U00010005\U00010006\U00010007"
        )),
        L"\U00010000\U00010001\U00010002\U00010003"
        L"\U00010004\U00010005\U00010006\U00010007"
    );
    ASSERT_EQ((convert<char, wchar_t>(
            string_view("0123456789\0abcdef\0ghijklmnopqrstuvwxyz", 38)
        )), wstring_view(L"0123456789\0abcdef\0ghijklmnopqrstuvwxyz", 38)
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
            L"いろはにほへと\nちりぬるを\nわかよたれそ\nつねならむ\nうゐのおくやま\n"
            L"けふこえて\nあさきゆめみし\nゑひもせす\n"
        )),
        "いろはにほへと\nちりぬるを\nわかよたれそ\nつねならむ\nうゐのおくやま\n"
        "けふこえて\nあさきゆめみし\nゑひもせす\n"
    );
    ASSERT_EQ((convert<wchar_t, char>(
            L"\U00010000\U00010001\U00010002\U00010003"
            L"\U00010004\U00010005\U00010006\U00010007"
        )),
        "\U00010000\U00010001\U00010002\U00010003"
        "\U00010004\U00010005\U00010006\U00010007"
    );
    ASSERT_EQ((convert<wchar_t, char>(
            wstring_view(L"0123456789\0abcdef\0ghijklmnopqrstuvwxyz", 38)
        )), string_view("0123456789\0abcdef\0ghijklmnopqrstuvwxyz", 38)
    );
}

TEST(CharEncoderTest, Throw) {
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
static basic_string<To> convert(const basic_string_view<From> str) {
    using std::function, std::generic_category, std::runtime_error,
        std::setlocale;

    if (setlocale(LC_ALL, "en_US.utf8") == nullptr) [[unlikely]]
        throw runtime_error("convert: unable to set locale");

    basic_string<To> buffer;
    char_encoder<From, To, function<void(To)>> encoder(
        [&buffer](const To c) constexpr -> void {
            buffer.push_back(c);
        }
    );
    for (const From c : str)
        encoder(c);
    if (!encoder.is_init_state())
        throw system_error(EILSEQ, generic_category(), "convert");

    return buffer;
}
