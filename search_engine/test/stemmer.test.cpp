#include <clocale> // LC_ALL, setlocale

#include <functional> // function
#include <stdexcept> // runtime_error
#include <string> // wstring
#include <string_view> // wstring_view
#include <vector> // vector

#include <gmock/gmock.h>
#include <gtest/gtest.h>

#include <search_engine/stemmer.hpp>

using std::vector, std::wstring, std::wstring_view;

using testing::ElementsAre;

static vector<wstring> stem(wstring_view);

TEST(StemmerTest, English) {
    ASSERT_THAT(stem(L"traditional"), ElementsAre(L"tradit")); // ???
    ASSERT_THAT(stem(L"traditional"), ElementsAre(L"tradition")); // ???
}

TEST(StemmerTest, Russian) {
    static const auto expected = ElementsAre(L"мягк");

    ASSERT_THAT(stem(L"мягка"), expected);
    ASSERT_THAT(stem(L"мягкая"), expected);
    ASSERT_THAT(stem(L"мягки"), expected);
    ASSERT_THAT(stem(L"мягкие"), expected);
    ASSERT_THAT(stem(L"мягкий"), expected);
    ASSERT_THAT(stem(L"мягким"), expected);
    ASSERT_THAT(stem(L"мягкими"), expected);
    ASSERT_THAT(stem(L"мягких"), expected);
    ASSERT_THAT(stem(L"мягко"), expected);
    ASSERT_THAT(stem(L"мягкого"), expected);
    ASSERT_THAT(stem(L"мягкое"), expected);
    ASSERT_THAT(stem(L"мягкой"), expected);
    ASSERT_THAT(stem(L"мягком"), expected);
    ASSERT_THAT(stem(L"мягкому"), expected);
    ASSERT_THAT(stem(L"мягкою"), expected);
    ASSERT_THAT(stem(L"мягкую"), expected);
    ASSERT_THAT(stem(L"мягкые"), expected);
    ASSERT_THAT(stem(L"мягкый"), expected);
    ASSERT_THAT(stem(L"мягок"), expected);
}

static vector<wstring> stem(const wstring_view wcs) {
    using std::function, std::runtime_error, std::setlocale;

    if (setlocale(LC_ALL, "en_US.utf8") == nullptr) [[unlikely]]
        throw runtime_error("stem: unable to set locale");

    vector<wstring> tokens;
    stemmer<function<void(const wstring &)>> invocable(
        [&tokens](const wstring &token) constexpr -> void {
            tokens.push_back(token);
        }
    );
    wstring buffer(wcs);
    invocable(buffer);

    return tokens;
}
