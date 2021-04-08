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

using testing::ElementsAre, testing::IsEmpty;

static vector<wstring> stem(wstring_view);

TEST(StemmerTest, English) {
    ASSERT_THAT(stem(L"traditional"), ElementsAre(L"tradition"));
}

TEST(StemmerTest, Russian) {
    ASSERT_THAT(stem(L"красная"), ElementsAre(L"красн"));
    ASSERT_THAT(stem(L"красное"), ElementsAre(L"красн"));
    ASSERT_THAT(stem(L"красные"), ElementsAre(L"красн"));
    ASSERT_THAT(stem(L"красный"), ElementsAre(L"красн"));
}

static vector<wstring> stem(const wstring_view wcs) {
    using std::function, std::runtime_error, std::setlocale;

    if (setlocale(LC_ALL, "en_US.utf8") == nullptr) [[unlikely]]
        throw runtime_error("stem: unable to set locale");

    vector<wstring> tokens;
    stemmer<function<void(const wstring &)>> str_stemmer(
        [&tokens](const wstring &token) constexpr -> void {
            tokens.push_back(token);
        }
    );
    wstring buffer(wcs);
    str_stemmer(buffer);

    return tokens;
}
