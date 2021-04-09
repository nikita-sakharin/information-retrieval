#ifndef SEARCH_ENGINE_NORMALIZER_HPP
#define SEARCH_ENGINE_NORMALIZER_HPP

#include <cstddef> // size_t
#include <cwctype> // iswalpha, towlower

#include <algorithm> // copy_if, is_sorted
#include <array> // array
#include <functional> // less_equal
#include <stdexcept> // logic_error
#include <string> // wstring
#include <string_view> // wstring_view
#include <type_traits> // is_invocable_r_v, is_nothrow_*_v

template<typename Invocable, bool StopWords = false>
class normalizer final {
public:
    constexpr normalizer() noexcept(
        std::is_nothrow_default_constructible_v<Invocable>) = default;
    constexpr explicit normalizer(const Invocable &) noexcept(
        std::is_nothrow_copy_constructible_v<Invocable>);
    constexpr normalizer(const normalizer &) noexcept(
        std::is_nothrow_copy_constructible_v<Invocable>) = default;
    constexpr normalizer(normalizer &&) noexcept(
        std::is_nothrow_move_constructible_v<Invocable>) = default;
    constexpr normalizer &operator=(const normalizer &) noexcept(
        std::is_nothrow_copy_assignable_v<Invocable>) = default;
    constexpr normalizer &operator=(normalizer &&) noexcept(
        std::is_nothrow_move_assignable_v<Invocable>) = default;
    constexpr ~normalizer() noexcept(
        std::is_nothrow_destructible_v<Invocable>) = default;

    constexpr void operator()(std::wstring &) noexcept(
        std::is_nothrow_invocable_r_v<void, Invocable,
            std::size_t, std::wstring &>
    );

    constexpr const Invocable &invocable() const noexcept;
    constexpr Invocable &invocable() noexcept;

    constexpr std::size_t position() const noexcept;

    constexpr void reset_position() noexcept;

private:
    static_assert(__STDC_ISO_10646__ >= 201103L,
        "Unicode version 2011 or later required"
    );
    static_assert(
        std::is_invocable_r_v<void, Invocable, std::size_t, std::wstring &>,
        "Invocable must have signature void(size_t, wstring &)"
    );

    static constexpr std::wstring_view possessive_affix = L"'s";

    static constexpr std::array<std::wstring_view, 184U> stop_words {
        L"a", L"an", L"and", L"are", L"as", L"at", L"be", L"but", L"by", L"for",
        L"if", L"in", L"into", L"is", L"it", L"no", L"not", L"of", L"on", L"or",
        L"such", L"that", L"the", L"their", L"then", L"there", L"these",
        L"they", L"this", L"to", L"was", L"will", L"with",

        L"а", L"без", L"более", L"больше", L"будет", L"будто", L"бы", L"был",
        L"была", L"были", L"было", L"быть", L"в", L"вам", L"вас", L"вдруг",
        L"ведь", L"во", L"вот", L"впрочем", L"все", L"всегда", L"всего",
        L"всех", L"всю", L"вы", L"где", L"да", L"даже", L"два", L"для", L"до",
        L"другой", L"его", L"ее", L"ей", L"ему", L"если", L"есть", L"еще", L"ж",
        L"же", L"за", L"зачем", L"здесь", L"и", L"из", L"или", L"им", L"иногда",
        L"их", L"к", L"как", L"какая", L"какой", L"когда", L"конечно", L"кто",
        L"куда", L"ли", L"лучше", L"между", L"меня", L"мне", L"много", L"может",
        L"можно", L"мой", L"моя", L"мы", L"на", L"над", L"надо", L"наконец",
        L"нас", L"не", L"него", L"нее", L"ней", L"нельзя", L"нет", L"ни",
        L"нибудь", L"никогда", L"ним", L"них", L"ничего", L"но", L"ну", L"о",
        L"об", L"один", L"он", L"она", L"они", L"опять", L"от", L"перед", L"по",
        L"под", L"после", L"потом", L"потому", L"почти", L"при", L"про", L"раз",
        L"разве", L"с", L"сам", L"свою", L"себе", L"себя", L"сейчас", L"со",
        L"совсем", L"так", L"такой", L"там", L"тебя", L"тем", L"теперь", L"то",
        L"тогда", L"того", L"тоже", L"только", L"том", L"тот", L"три", L"тут",
        L"ты", L"у", L"уж", L"уже", L"хорошо", L"хоть", L"чего", L"чем",
        L"через", L"что", L"чтоб", L"чтобы", L"чуть", L"эти", L"этого", L"этой",
        L"этом", L"этот", L"эту", L"я"
    };
    static_assert(
        std::is_sorted(stop_words.cbegin(), stop_words.cend(),
            std::less_equal()
        ),
        "stop words must be unique and sorted"
    );
    static_assert(!stop_words.empty() && !stop_words.front().empty(),
        "all stop words must not be empty"
    );

    std::size_t position_ = 0U;
    Invocable invocable_{};
};

template<typename Invocable, bool StopWords>
constexpr normalizer<Invocable, StopWords>::normalizer(
    const Invocable &invocable
) noexcept(
    std::is_nothrow_copy_constructible_v<Invocable>
) : invocable_(invocable) {}

template<typename Invocable, bool StopWords>
constexpr void normalizer<Invocable, StopWords>::operator()(
    std::wstring &wcs
) noexcept(
    std::is_nothrow_invocable_r_v<void, Invocable, std::size_t, std::wstring &>
) {
    using std::copy_if, std::iswalpha, std::logic_error, std::size_t,
        std::towlower, std::wstring;

    if (wcs.empty()) [[unlikely]]
        throw logic_error("normalizer::operator(): empty token");

    bool is_acronym = true;
    for (size_t i = 0U; i < wcs.size(); ++i) {
        wcs[i] = towlower(wcs[i]);
        if (wcs[i] == L'ё') [[unlikely]]
            wcs[i] = L'е';
        if (is_acronym && (
                (i % 2U == 0U && !iswalpha(wcs[i])) ||
                (i % 2U == 1U && wcs[i] != L'.')
            )
        ) is_acronym = false;
    }
    if (wcs.ends_with(possessive_affix))
        wcs.resize(wcs.size() - possessive_affix.size());

    if constexpr (StopWords)
        if (binary_search(stop_words.cbegin(), stop_words.cend(), wcs))
            return;

    if (is_acronym) [[unlikely]] {
        const wstring::iterator last = copy_if(
            wcs.cbegin(), wcs.cend(), wcs.begin(),
            [](const wchar_t wc) constexpr noexcept -> bool {
                return wc != L'.';
            }
        );
        wcs.erase(last, wcs.end());
    }

    invocable_(position_++, wcs);
}

template<typename Invocable, bool StopWords>
constexpr const Invocable &normalizer<Invocable, StopWords>::invocable(
) const noexcept {
    return invocable_;
}

template<typename Invocable, bool StopWords>
constexpr Invocable &normalizer<Invocable, StopWords>::invocable() noexcept {
    return invocable_;
}

template<typename Invocable, bool StopWords>
constexpr std::size_t normalizer<Invocable, StopWords>::position(
) const noexcept {
    return position_;
}

template<typename Invocable, bool StopWords>
constexpr void normalizer<Invocable, StopWords>::reset_position() noexcept {
    position_ = 0U;
}

#endif
