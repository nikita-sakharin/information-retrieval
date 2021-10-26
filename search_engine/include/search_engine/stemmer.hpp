#ifndef SEARCH_ENGINE_STEMMER_HPP
#define SEARCH_ENGINE_STEMMER_HPP

#include <cassert> // assert
#include <cstddef> // size_t

#include <algorithm> // adjacent_find, is_sorted, lexicographical_compare, min
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
    static_assert(std::is_invocable_r_v<void, Invocable, std::wstring &>,
        "Invocable must have signature void(wstring &)"
    );

    static constexpr std::size_t size_before_suffix = 2U;

    static constexpr std::array<std::wstring_view, 59U> suffixes = {
        L"ing", L"al", L"s", L"es", L"ness", L"ly",

        L"а", L"ила", L"ьев", L"ёв", L"е", L"ее", L"ие", L"ое", L"ые", L"и",
        L"ли", L"или", L"ами", L"ими", L"ыми", L"ями", L"ьями", L"й", L"ей",
        L"ий", L"ой", L"ый", L"л", L"ил", L"ам", L"ем", L"им", L"ом", L"ым",
        L"ям", L"ьям", L"ём", L"о", L"ого", L"ло", L"ило", L"у", L"ому", L"ах",
        L"их", L"ых", L"ях", L"ы", L"ь", L"ившись", L"ывшись", L"ю", L"ою",
        L"ую", L"ью", L"я", L"ая", L"ья"
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
                );
            }
        ) &&
        std::adjacent_find(
            suffixes.cbegin(), suffixes.cend()
        ) == suffixes.cend(),
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
    using std::equal_range, std::logic_error, std::min, std::size_t, std::tie,
        std::wstring_view;

    if (wcs.empty()) [[unlikely]]
        throw logic_error("stemmer::operator(): empty token");

    const size_t size = wcs.size();
    auto first = suffixes.cbegin(), last = suffixes.cend(),
        match = suffixes.cend();
    for (size_t i = 0; i < size && first != last; ++i) {
        if (const size_t suffix_size = first->size();
            i == suffix_size && size >= suffix_size + size_before_suffix
        ) match = first;
        tie(first, last) = equal_range(first, last, wcs,
            [i](
                const wstring_view wcs1,
                const wstring_view wcs2
            ) constexpr noexcept -> bool {
                const size_t size1 = wcs1.size(), size2 = wcs2.size();
                assert(i <= size1 && i <= size2 && (i < size1 || i < size2));
                assert(i == 0U || wcs1[size1 - i] == wcs2[size2 - i]);

                if (i >= min(size1, size2))
                    return size1 < size2;
                return wcs1[size1 - i - 1U] < wcs2[size2 - i - 1U];
            }
        );
    }
    if (match != suffixes.cend())
        wcs.resize(size - match->size());
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
