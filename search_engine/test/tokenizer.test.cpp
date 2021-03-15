#include <string> // wstring
#include <string_view> // wstring_view
#include <vector> // vector

#include <gtest/gtest.h>

// #include <search_engine/tokenizer.hpp>

using std::vector, std::wstring, std::wstring_view;

static vector<wstring> tokenize(wstring_view);

TEST(TokenizerTest, English) {
    ASSERT_EQ(tokenize(L"!"), {});
    ASSERT_EQ(tokenize(L"\""), {});
    ASSERT_EQ(tokenize(L"&"), {});
    ASSERT_EQ(tokenize(L"'"), {});
    ASSERT_EQ(tokenize(L","), {});
    ASSERT_EQ(tokenize(L"-"), {});
    ASSERT_EQ(tokenize(L"."), {});
    ASSERT_EQ(tokenize(L":"), {});
    ASSERT_EQ(tokenize(L";"), {});
    ASSERT_EQ(tokenize(L"?"), {});
    ASSERT_EQ(tokenize(L"\\"), {});
    ASSERT_EQ(tokenize(L"!\"&',-.:;?\\"), {});


    ASSERT_EQ(tokenize(L"The pi number approximately is 3.141592653589793."), {
        L"The", L"pi", L"number", L"approximately", L"is", L"3,141592653589793"
    });
    ASSERT_EQ(tokenize(L"God Bless the U.S.A."), {
        L"The", L"pi", L"number", L"approximately", L"is", L"3,141592653589793"
    });
    ASSERT_EQ(tokenize(L"No. is an abbreviation of Number"), {
        L"No", L"is", L"an", L"abbreviation", L"of", L"Number"
    });
    ASSERT_EQ(tokenize(
        L"Karl Witte graduate Ph.D. from University of Giessen at age 13."
    ), {
        L"Karl", L"Witte", L"graduate", L"Ph.D.", L"from", L"University", L"of",
        L"Giessen", L"at", L"age", L"13"
    });

    "The SARS-CoV-2 end";
}

/*
если в тексте есть тире
инженер-механик co-education
или точки
C.A.T. U.S.A.
то они обрабатываются в токенизаторе, а не нормализаторе
*/

TEST(TokenizerTest, Russian) {
    ASSERT_EQ(tokenize(L"слово"), { L"слово" });
    ASSERT_EQ(tokenize(L"инженер-механик"), { L"инженер", L"механик" });
    ASSERT_EQ(tokenize(L"математик-программист"), { L"математик", L"программист" });
    ASSERT_EQ(tokenize(L"инженер, но не механик"), { L"инженер", L"но", L"не", L"механик" });
    ASSERT_EQ(tokenize(L"математик и программист"), { L"математик", L"и", L"программист" });
    ASSERT_EQ(tokenize(L"Число Пи приблизительно равно 3,141592653589793."), {
        L"Число", L"Пи", L"3,141592653589793"
    });
    ASSERT_EQ(tokenize(L"м.н.с. - аббревиатура от младший научный сотрудник"), {"м.н.с.", "аббревиатура"});
    ASSERT_EQ(tokenize(L"Предложение заканчивается точкой."), {
        L"Предложение", L"заканчивается", L"точкой"
    });
    ASSERT_EQ(tokenize(L"Популярные языки программирования: Java, C, Python"), {
        L"Популярные", L"языки", L"программирования", L"Java", L"C", L"Python"
    });
}

static vector<wstring> tokenize(const wstring_view str) {
    vector<wstring> tokens;
    for ()
    tokenizer tokenizer([&tokens](const size_t position, wstring &wcs) constexpr -> void {
        buffer.push_back(c);
    });
    for_each(str.cbegin(), str.cend(), tokenizer);
    return tokens;
}
