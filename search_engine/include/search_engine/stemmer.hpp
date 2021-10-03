#ifndef SEARCH_ENGINE_STEMMER_HPP
#define SEARCH_ENGINE_STEMMER_HPP

#include <cassert> // assert
#include <cstddef> // size_t

#include <algorithm> // is_sorted, lexicographical_compare, min
#include <array> // array
#include <stdexcept> // logic_error
#include <string> // wstring
#include <string_view> // wstring_view
#include <tuple> // tie
#include <type_traits> // is_invocable_r_v, is_nothrow_*_v

template<typename Invocable>
class stemmer final {
public:
    constexpr stemmer() noexcept(
        std::is_nothrow_default_constructible_v<Invocable>) = default;
    constexpr explicit stemmer(const Invocable &) noexcept(
        std::is_nothrow_copy_constructible_v<Invocable>);
    constexpr stemmer(const stemmer &) noexcept(
        std::is_nothrow_copy_constructible_v<Invocable>) = default;
    constexpr stemmer(stemmer &&) noexcept(
        std::is_nothrow_move_constructible_v<Invocable>) = default;
    constexpr stemmer &operator=(const stemmer &) noexcept(
        std::is_nothrow_copy_assignable_v<Invocable>) = default;
    constexpr stemmer &operator=(stemmer &&) noexcept(
        std::is_nothrow_move_assignable_v<Invocable>) = default;
    constexpr ~stemmer() noexcept(
        std::is_nothrow_destructible_v<Invocable>) = default;

    constexpr void operator()(std::wstring &);

    constexpr const Invocable &invocable() const noexcept;
    constexpr Invocable &invocable() noexcept;

private:
    static_assert(__STDC_ISO_10646__ >= 201103L,
        "Unicode version 2011 or later required"
    );
    static_assert(
        std::is_invocable_r_v<void, Invocable, std::wstring &>,
        "Invocable must have signature void(wstring &)"
    );

    static constexpr std::size_t length_before_suffix = 2U;

    static constexpr std::array<std::wstring_view, 40U> suffixes { // TODO
        L"ing", L"al", L"s", L"es", L"ness", L"ly",

        L"а", L"е", L"ее", L"ие", L"ое", L"ые", L"и", L"ли", L"ами", L"ими",
        L"ыми", L"й" L"ей", L"ий", L"ой", L"ый", L"л", L"ам", L"ем", L"им",
        L"ом", L"ым", L"о", L"ого", L"у", L"ому", L"ах", L"их", L"ых", L"ы",
        L"ю", L"ою", L"ую", L"я", L"ая"
    };
    static_assert(
        std::is_sorted(suffixes.cbegin(), suffixes.cend(),
            [](
                const std::wstring_view wcs1,
                const std::wstring_view wcs2
            ) constexpr noexcept -> bool {
                return std::lexicographical_compare(
                    wcs1.crbegin(), wcs1.crend(),
                    wcs2.crbegin(), wcs2.crend()
                ) || wcs1 == wcs2;
            }
        ),
        "suffixes must be unique and sorted"
    );
    static_assert(!suffixes.empty() && !suffixes.front().empty(),
        "suffixes set and all of them must not be empty"
    );

    Invocable invocable_{};
};

template<typename Invocable>
constexpr stemmer<Invocable>::stemmer(const Invocable &invocable) noexcept(
    std::is_nothrow_copy_constructible_v<Invocable>
) : invocable_(invocable) {}

template<typename Invocable>
constexpr void stemmer<Invocable>::operator()(std::wstring &wcs) {
    using std::equal_range, std::logic_error, std::min, std::size_t,
        std::tie, std::wstring_view;

    if (wcs.empty()) [[unlikely]]
        throw logic_error("stemmer::operator(): empty token");

    const size_t size = wcs.size();
    auto first = suffixes.cbegin(), last = suffixes.cend(),
        match = suffixes.cend();
    for (size_t i = 0; i < size && first != last; ++i) {
        if (const size_t suffix_size = first->size(); i == suffix_size)
            match = first;
        tie(first, last) = equal_range(first, last, wcs,
            [i](
                const wstring_view wcs1,
                const wstring_view wcs2
            ) constexpr noexcept -> bool {
                assert(i <= wcs1.size() && i <= wcs2.size() &&
                    (i < wcs1.size() || i < wcs2.size())
                );
                assert(i == 0U ||
                    wcs1[wcs1.size() - i] == wcs2[wcs2.size() - i]
                );

                if (i >= min(wcs1.size(), wcs2.size()))
                    return wcs1.size() < wcs2.size();
                return wcs1[wcs1.size() - i - 1U] < wcs2[wcs2.size() - i - 1U];
            }
        );
    }
    if (match != suffixes.cend())
        if (const size_t suffix = match->size(); size >= suffix + before_suffix)
            wcs.resize(size - suffix);
    invocable_(wcs);
}

template<typename Invocable>
constexpr const Invocable &stemmer<Invocable>::invocable() const noexcept {
    return invocable_;
}

template<typename Invocable>
constexpr Invocable &stemmer<Invocable>::invocable() noexcept {
    return invocable_;
}

#endif
