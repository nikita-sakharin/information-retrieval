#include <clocale> // LC_ALL, setlocale

#include <functional> // function
#include <stdexcept> // runtime_error
#include <string> // wstring
#include <string_view> // wstring_view
#include <vector> // vector

#include <gmock/gmock.h>
#include <gtest/gtest.h>

#include <search_engine/normalizer.hpp>

using std::vector, std::wstring, std::wstring_view;

using testing::ElementsAre, testing::IsEmpty;

static vector<wstring> normalize(wstring_view);

TEST(NormalizerTest, Acronym) {
    ASSERT_THAT(normalize(L"Q.U.I.C.K"), ElementsAre(L"quick"));
    ASSERT_THAT(normalize(L"Q.U.I.C.K."), ElementsAre(L"quick"));
    ASSERT_THAT(normalize(L"Q.u.i.c.k"), ElementsAre(L"quick"));
    ASSERT_THAT(normalize(L"Q.u.i.c.k."), ElementsAre(L"quick"));
    ASSERT_THAT(normalize(L"q.u.i.c.k"), ElementsAre(L"quick"));
    ASSERT_THAT(normalize(L"q.u.i.c.k."), ElementsAre(L"quick"));
}

TEST(NormalizerTest, CyrillicIo) {
    ASSERT_THAT(normalize(L"ЕЩЁ"), ElementsAre(L"еще"));
    ASSERT_THAT(normalize(L"Ещё"), ElementsAre(L"еще"));
    ASSERT_THAT(normalize(L"ещё"), ElementsAre(L"еще"));
}
/*
TEST(NormalizerTest, Empty) { empty allowed ???
    ASSERT_THAT(normalize(L""), IsEmpty());
}
*/
TEST(NormalizerTest, English) {
    ASSERT_THAT(normalize(L"THE"), ElementsAre(L"the"));
    ASSERT_THAT(normalize(L"The"), ElementsAre(L"the"));
    ASSERT_THAT(normalize(L"the"), ElementsAre(L"the"));
    ASSERT_THAT(normalize(L"QUICK"), ElementsAre(L"quick"));
    ASSERT_THAT(normalize(L"Quick"), ElementsAre(L"quick"));
    ASSERT_THAT(normalize(L"quick"), ElementsAre(L"quick"));
    ASSERT_THAT(normalize(L"BROWN"), ElementsAre(L"brown"));
    ASSERT_THAT(normalize(L"Brown"), ElementsAre(L"brown"));
    ASSERT_THAT(normalize(L"brown"), ElementsAre(L"brown"));
    ASSERT_THAT(normalize(L"FOX"), ElementsAre(L"fox"));
    ASSERT_THAT(normalize(L"Fox"), ElementsAre(L"fox"));
    ASSERT_THAT(normalize(L"fox"), ElementsAre(L"fox"));
    ASSERT_THAT(normalize(L"JUMPS"), ElementsAre(L"jumps"));
    ASSERT_THAT(normalize(L"Jumps"), ElementsAre(L"jumps"));
    ASSERT_THAT(normalize(L"jumps"), ElementsAre(L"jumps"));
    ASSERT_THAT(normalize(L"OVER"), ElementsAre(L"over"));
    ASSERT_THAT(normalize(L"Over"), ElementsAre(L"over"));
    ASSERT_THAT(normalize(L"over"), ElementsAre(L"over"));
    ASSERT_THAT(normalize(L"LAZY"), ElementsAre(L"lazy"));
    ASSERT_THAT(normalize(L"Lazy"), ElementsAre(L"lazy"));
    ASSERT_THAT(normalize(L"lazy"), ElementsAre(L"lazy"));
    ASSERT_THAT(normalize(L"DOG"), ElementsAre(L"dog"));
    ASSERT_THAT(normalize(L"Dog"), ElementsAre(L"dog"));
    ASSERT_THAT(normalize(L"dog"), ElementsAre(L"dog"));
}

TEST(NormalizerTest, PossessiveAffix) {
    ASSERT_THAT(normalize(L"dog'S"), ElementsAre(L"dog"));
    ASSERT_THAT(normalize(L"dog's"), ElementsAre(L"dog"));
}

TEST(NormalizerTest, Russian) {
    ASSERT_THAT(normalize(L"СЪЕШЬ"), ElementsAre(L"съешь"));
    ASSERT_THAT(normalize(L"Съешь"), ElementsAre(L"съешь"));
    ASSERT_THAT(normalize(L"съешь"), ElementsAre(L"съешь"));
    ASSERT_THAT(normalize(L"ЕЩЕ"), ElementsAre(L"еще"));
    ASSERT_THAT(normalize(L"Еще"), ElementsAre(L"еще"));
    ASSERT_THAT(normalize(L"еще"), ElementsAre(L"еще"));
    ASSERT_THAT(normalize(L"ЭТИХ"), ElementsAre(L"этих"));
    ASSERT_THAT(normalize(L"Этих"), ElementsAre(L"этих"));
    ASSERT_THAT(normalize(L"этих"), ElementsAre(L"этих"));
    ASSERT_THAT(normalize(L"МЯГКИХ"), ElementsAre(L"мягких"));
    ASSERT_THAT(normalize(L"Мягких"), ElementsAre(L"мягких"));
    ASSERT_THAT(normalize(L"мягких"), ElementsAre(L"мягких"));
    ASSERT_THAT(normalize(L"ФРАНЦУЗСКИХ"), ElementsAre(L"французских"));
    ASSERT_THAT(normalize(L"Французских"), ElementsAre(L"французских"));
    ASSERT_THAT(normalize(L"французских"), ElementsAre(L"французских"));
    ASSERT_THAT(normalize(L"БУЛОК"), ElementsAre(L"булок"));
    ASSERT_THAT(normalize(L"Булок"), ElementsAre(L"булок"));
    ASSERT_THAT(normalize(L"булок"), ElementsAre(L"булок"));
    ASSERT_THAT(normalize(L"ДА"), ElementsAre(L"да"));
    ASSERT_THAT(normalize(L"Да"), ElementsAre(L"да"));
    ASSERT_THAT(normalize(L"да"), ElementsAre(L"да"));
    ASSERT_THAT(normalize(L"ВЫПЕЙ"), ElementsAre(L"выпей"));
    ASSERT_THAT(normalize(L"Выпей"), ElementsAre(L"выпей"));
    ASSERT_THAT(normalize(L"выпей"), ElementsAre(L"выпей"));
    ASSERT_THAT(normalize(L"ЧАЮ"), ElementsAre(L"чаю"));
    ASSERT_THAT(normalize(L"Чаю"), ElementsAre(L"чаю"));
    ASSERT_THAT(normalize(L"чаю"), ElementsAre(L"чаю"));
}

static vector<wstring> normalize(const wstring_view wcs) {
    using std::function, std::runtime_error, std::setlocale, std::size_t;

    if (setlocale(LC_ALL, "en_US.utf8") == nullptr) [[unlikely]]
        throw runtime_error("convert: unable to set locale");

    vector<wstring> tokens;
    normalizer<function<void(size_t, const wstring &)>> str_normalizer(
        [&tokens, n = static_cast<size_t>(0U)](
            const size_t position, const wstring &token
        ) mutable -> void {
            ASSERT_EQ(position, n);
            tokens.push_back(token);
            ++n;
        }
    );
    wstring buffer(wcs);
    str_normalizer(buffer);

    return tokens;
}
