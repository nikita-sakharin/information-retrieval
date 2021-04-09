#include <clocale> // LC_ALL, setlocale
#include <cstddef> // size_t

#include <functional> // function
#include <initializer_list> // initializer_list
#include <stdexcept> // logic_error, runtime_error
#include <string> // wstring
#include <string_view> // wstring_view
#include <utility> // pair
#include <vector> // vector

#include <gmock/gmock.h>
#include <gtest/gtest.h>

#include <search_engine/normalizer.hpp>

using std::initializer_list, std::pair, std::size_t, std::vector, std::wstring,
    std::wstring_view;

using testing::ElementsAre, testing::IsEmpty, testing::Pair;

template<bool StopWords = false>
static vector<pair<size_t, wstring>> normalize(initializer_list<wstring_view>);

TEST(NormalizerTest, Acronym) {
    static const auto expected = ElementsAre(Pair(0U, L"quick"));

    ASSERT_THAT(normalize({ L"Q.U.I.C.K" }), expected);
    ASSERT_THAT(normalize({ L"Q.U.I.C.K." }), expected);
    ASSERT_THAT(normalize({ L"Q.u.i.c.k" }), expected);
    ASSERT_THAT(normalize({ L"Q.u.i.c.k." }), expected);
    ASSERT_THAT(normalize({ L"q.u.i.c.k" }), expected);
    ASSERT_THAT(normalize({ L"q.u.i.c.k." }), expected);
}

TEST(NormalizerTest, CyrillicIo) {
    static const auto expected = ElementsAre(Pair(0U, L"еще"));

    ASSERT_THAT(normalize({ L"ЕЩЁ" }), expected);
    ASSERT_THAT(normalize({ L"Ещё" }), expected);
    ASSERT_THAT(normalize({ L"ещё" }), expected);
}

TEST(NormalizerTest, Empty) {
    using std::logic_error;

    ASSERT_THROW(normalize({ L"" }), logic_error);
}

TEST(NormalizerTest, English) {
    static const auto expected = ElementsAre(
        Pair(0U, L"the"), Pair(1U, L"quick"), Pair(2U, L"brown"),
        Pair(3U, L"fox"), Pair(4U, L"jumps"), Pair(5U, L"over"),
        Pair(6U, L"lazy"), Pair(7U, L"dog")
    );

    ASSERT_THAT(normalize({
            L"THE", L"QUICK", L"BROWN", L"FOX", L"JUMPS", L"OVER", L"LAZY",
            L"DOG"
        }), expected
    );
    ASSERT_THAT(normalize({
            L"the", L"quick", L"brown", L"fox", L"jumps", L"over", L"lazy",
            L"dog"
        }), expected
    );
    ASSERT_THAT(normalize({
            L"The", L"Quick", L"Brown", L"Fox", L"Jumps", L"Over", L"Lazy",
            L"Dog"
        }), expected
    );
}

TEST(NormalizerTest, EnglishStopWords) {
    static const auto expected = ElementsAre(Pair(0U, L"the"));

    ASSERT_THAT(normalize<true>({ L"T.H.E." }), expected);
    ASSERT_THAT(normalize<true>({ L"T.h.e." }), expected);
    ASSERT_THAT(normalize<true>({ L"t.h.e." }), expected);

    ASSERT_THAT(normalize<true>({ L"THE" }), IsEmpty());
    ASSERT_THAT(normalize<true>({ L"THE'S" }), IsEmpty());
    ASSERT_THAT(normalize<true>({ L"THE's" }), IsEmpty());
    ASSERT_THAT(normalize<true>({ L"The" }), IsEmpty());
    ASSERT_THAT(normalize<true>({ L"The'S" }), IsEmpty());
    ASSERT_THAT(normalize<true>({ L"The's" }), IsEmpty());
    ASSERT_THAT(normalize<true>({ L"the" }), IsEmpty());
    ASSERT_THAT(normalize<true>({ L"the'S" }), IsEmpty());
    ASSERT_THAT(normalize<true>({ L"the's" }), IsEmpty());
}

TEST(NormalizerTest, PossessiveAffix) {
    static const auto expected = ElementsAre(Pair(0U, L"dog"));

    ASSERT_THAT(normalize({ L"DOG'S" }), expected);
    ASSERT_THAT(normalize({ L"DOG's" }), expected);
    ASSERT_THAT(normalize({ L"Dog'S" }), expected);
    ASSERT_THAT(normalize({ L"Dog's" }), expected);
    ASSERT_THAT(normalize({ L"dog'S" }), expected);
    ASSERT_THAT(normalize({ L"dog's" }), expected);
}

TEST(NormalizerTest, Russian) {
    static const auto expected = ElementsAre(
        Pair(0U, L"съешь"), Pair(1U, L"еще"), Pair(2U, L"этих"),
        Pair(3U, L"мягких"), Pair(4U, L"французских"), Pair(5U, L"булок"),
        Pair(6U, L"да"), Pair(7U, L"выпей"), Pair(8U, L"чаю")
    );

    ASSERT_THAT(normalize({
            L"СЪЕШЬ", L"ЕЩЕ", L"ЭТИХ", L"МЯГКИХ", L"ФРАНЦУЗСКИХ", L"БУЛОК",
            L"ДА", L"ВЫПЕЙ", L"ЧАЮ",
        }), expected
    );
    ASSERT_THAT(normalize({
            L"Съешь", L"Еще", L"Этих", L"Мягких", L"Французских", L"Булок",
            L"Да", L"Выпей", L"Чаю"
        }), expected
    );
    ASSERT_THAT(normalize({
            L"съешь", L"еще", L"этих", L"мягких", L"французских", L"булок",
            L"да", L"выпей", L"чаю"
        }), expected
    );
}

TEST(NormalizerTest, RussianStopWords) {
    static const auto expected = ElementsAre(Pair(0U, L"еще"));

    ASSERT_THAT(normalize<true>({ L"Е.Щ.Ё." }), expected);
    ASSERT_THAT(normalize<true>({ L"Е.щ.ё." }), expected);
    ASSERT_THAT(normalize<true>({ L"е.щ.ё." }), expected);

    ASSERT_THAT(normalize<true>({ L"ЕЩЁ" }), IsEmpty());
    ASSERT_THAT(normalize<true>({ L"ЕЩЕ" }), IsEmpty());
    ASSERT_THAT(normalize<true>({ L"Еще" }), IsEmpty());
    ASSERT_THAT(normalize<true>({ L"Ещё" }), IsEmpty());
    ASSERT_THAT(normalize<true>({ L"еще" }), IsEmpty());
    ASSERT_THAT(normalize<true>({ L"ещё" }), IsEmpty());
}

template<bool StopWords>
static vector<pair<size_t, wstring>> normalize(
    const initializer_list<wstring_view> init
) {
    using std::function, std::runtime_error, std::setlocale;

    if (setlocale(LC_ALL, "en_US.utf8") == nullptr) [[unlikely]]
        throw runtime_error("normalize: unable to set locale");

    vector<pair<size_t, wstring>> tokens;
    normalizer<function<void(size_t, const wstring &)>, StopWords> str_normalizer(
        [&tokens](
            const size_t position, const wstring &token
        ) constexpr -> void {
            tokens.emplace_back(position, token);
        }
    );
    wstring buffer;
    for (const wstring_view wcs : init) {
        buffer = wcs;
        str_normalizer(buffer);
    }

    return tokens;
}
