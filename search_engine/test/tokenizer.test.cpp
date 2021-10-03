#include <clocale> // LC_ALL, setlocale

#include <functional> // function
#include <stdexcept> // runtime_error
#include <string> // wstring
#include <string_view> // wstring_view
#include <vector> // vector

#include <gmock/gmock.h>
#include <gtest/gtest.h>

#include <search_engine/tokenizer.hpp>

using std::vector, std::wstring, std::wstring_view;

using testing::ElementsAre, testing::IsEmpty;

static vector<wstring> tokenize(wstring_view);

TEST(TokenizerTest, Digit) {
    ASSERT_THAT(tokenize(L"0123456789'"), ElementsAre(L"0123456789"));
    ASSERT_THAT(tokenize(L"0123456789''"), ElementsAre(L"0123456789"));
    ASSERT_THAT(tokenize(L"0123456789,"), ElementsAre(L"0123456789"));
    ASSERT_THAT(tokenize(L"0123456789,,"), ElementsAre(L"0123456789"));
    ASSERT_THAT(tokenize(L"0123456789-"), ElementsAre(L"0123456789"));
    ASSERT_THAT(tokenize(L"0123456789--"), ElementsAre(L"0123456789"));
    ASSERT_THAT(tokenize(L"0123456789."), ElementsAre(L"0123456789"));
    ASSERT_THAT(tokenize(L"0123456789.."), ElementsAre(L"0123456789"));
    ASSERT_THAT(tokenize(L"123456789''0"), ElementsAre(L"123456789", L"0"));
    ASSERT_THAT(tokenize(L"123456789'0"), ElementsAre(L"123456789", L"0"));
    ASSERT_THAT(tokenize(L"123456789,,0"), ElementsAre(L"123456789", L"0"));
    ASSERT_THAT(tokenize(L"123456789,0"), ElementsAre(L"123456789,0"));
    ASSERT_THAT(tokenize(L"123456789--0"), ElementsAre(L"123456789", L"0"));
    ASSERT_THAT(tokenize(L"123456789-0"), ElementsAre(L"123456789", L"0"));
    ASSERT_THAT(tokenize(L"123456789..0"), ElementsAre(L"123456789", L"0"));
    ASSERT_THAT(tokenize(L"123456789.0"), ElementsAre(L"123456789.0"));

    ASSERT_THAT(tokenize(L"3,141,592,653,589,793"),
        ElementsAre(L"3,141,592,653,589,793")
    );
    ASSERT_THAT(tokenize(L"3,141.592.653.589.793"),
        ElementsAre(L"3,141.592.653.589.793")
    );
    ASSERT_THAT(tokenize(L"3,141592653589793"),
        ElementsAre(L"3,141592653589793")
    );
    ASSERT_THAT(tokenize(L"3.141,592,653,589,793"),
        ElementsAre(L"3.141,592,653,589,793")
    );
    ASSERT_THAT(tokenize(L"3.141.592.653.589.793"),
        ElementsAre(L"3.141.592.653.589.793")
    );
    ASSERT_THAT(tokenize(L"3.141592653589793"),
        ElementsAre(L"3.141592653589793")
    );
}

TEST(TokenizerTest, Empty) {
    ASSERT_THAT(tokenize(L""), IsEmpty());
}

TEST(TokenizerTest, English) {
    ASSERT_THAT(tokenize(L"A'B'C'D'E'F'"), ElementsAre(L"A'B'C'D'E'F"));
    ASSERT_THAT(tokenize(L"A,B,C,D,E,F,"),
        ElementsAre(L"A", L"B", L"C", L"D", L"E", L"F")
    );
    ASSERT_THAT(tokenize(L"A-B-C-D-E-F-"),
        ElementsAre(L"A", L"B", L"C", L"D", L"E", L"F")
    );
    ASSERT_THAT(tokenize(L"A.B.C.D.E.F."), ElementsAre(L"A.B.C.D.E.F"));
    ASSERT_THAT(tokenize(L"ABC''DEF"), ElementsAre(L"ABC", L"DEF"));
    ASSERT_THAT(tokenize(L"ABC'DEF"), ElementsAre(L"ABC'DEF"));
    ASSERT_THAT(tokenize(L"ABC,,DEF"), ElementsAre(L"ABC", L"DEF"));
    ASSERT_THAT(tokenize(L"ABC,DEF"), ElementsAre(L"ABC", L"DEF"));
    ASSERT_THAT(tokenize(L"ABC--DEF"), ElementsAre(L"ABC", L"DEF"));
    ASSERT_THAT(tokenize(L"ABC-DEF"), ElementsAre(L"ABC", L"DEF"));
    ASSERT_THAT(tokenize(L"ABC..DEF"), ElementsAre(L"ABC", L"DEF"));
    ASSERT_THAT(tokenize(L"ABC.DEF"), ElementsAre(L"ABC.DEF"));
    ASSERT_THAT(tokenize(L"ABCDEF"), ElementsAre(L"ABCDEF"));

    ASSERT_THAT(tokenize(
            L"The quick brown fox jumps over the lazy dog."
        ),
        ElementsAre(L"The", L"quick", L"brown", L"fox", L"jumps", L"over",
            L"the", L"lazy", L"dog"
        )
    );
}

TEST(TokenizerTest, Punctuation) {
    ASSERT_THAT(tokenize(L"!\"#$%&'()*+,-./:;<=>?@[\\]^_`{|}~"), IsEmpty());
}

TEST(TokenizerTest, Russian) {
    ASSERT_THAT(tokenize(
            L"Съешь еще этих мягких французских булок, да выпей чаю."
        ),
        ElementsAre(L"Съешь", L"еще", L"этих", L"мягких", L"французских",
            L"булок", L"да", L"выпей", L"чаю"
        )
    );
}

TEST(TokenizerTest, Space) {
    ASSERT_THAT(tokenize(L"\t\n\v\f\r "), IsEmpty());
}

static vector<wstring> tokenize(const wstring_view wcs) {
    using std::function, std::runtime_error, std::setlocale;

    if (setlocale(LC_ALL, "en_US.utf8") == nullptr) [[unlikely]]
        throw runtime_error("tokenize: unable to set locale");

    vector<wstring> tokens;
    tokenizer<function<void(const wstring &)>> invocable(
        [&tokens](const wstring &token) constexpr -> void {
            tokens.push_back(token);
        }
    );
    for (const wchar_t wc : wcs)
        invocable(wc);
    invocable.flush_buffer();

    return tokens;
}
