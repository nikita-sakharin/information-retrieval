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
    static const auto soft = ElementsAre(L"мягк"), warm = ElementsAre(L"тёпл"),
        tee = ElementsAre(L"ча");

    ASSERT_THAT(stem(L"мягка"), soft);
    ASSERT_THAT(stem(L"мягкая"), soft);
    ASSERT_THAT(stem(L"мягки"), soft);
    ASSERT_THAT(stem(L"мягкие"), soft);
    ASSERT_THAT(stem(L"мягкий"), soft);
    ASSERT_THAT(stem(L"мягким"), soft);
    ASSERT_THAT(stem(L"мягкими"), soft);
    ASSERT_THAT(stem(L"мягких"), soft);
    ASSERT_THAT(stem(L"мягко"), soft);
    ASSERT_THAT(stem(L"мягкого"), soft);
    ASSERT_THAT(stem(L"мягкое"), soft);
    ASSERT_THAT(stem(L"мягкой"), soft);
    ASSERT_THAT(stem(L"мягком"), soft);
    ASSERT_THAT(stem(L"мягкому"), soft);
    ASSERT_THAT(stem(L"мягкою"), soft);
    ASSERT_THAT(stem(L"мягкую"), soft);
    ASSERT_THAT(stem(L"тёплы"), warm);
    ASSERT_THAT(stem(L"тёплые"), warm);
    ASSERT_THAT(stem(L"тёплый"), warm);
    ASSERT_THAT(stem(L"тёплым"), warm);
    ASSERT_THAT(stem(L"тёплыми"), warm);
    ASSERT_THAT(stem(L"тёплых"), warm);
    ASSERT_THAT(stem(L"чай"), tee);
    ASSERT_THAT(stem(L"чаю"), tee);
    ASSERT_THAT(stem(L"чая"), tee);
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
