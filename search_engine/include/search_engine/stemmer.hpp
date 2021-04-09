#ifndef SEARCH_ENGINE_STEMMER_HPP
#define SEARCH_ENGINE_STEMMER_HPP

#include <cassert> // assert
#include <cstddef> // size_t

#include <algorithm> // is_sorted, lexicographical_compare
#include <array> // array
#include <stdexcept> // logic_error
#include <string> // wstring
#include <string_view> // wstring_view
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

    constexpr void operator()(std::wstring &) noexcept(
        std::is_nothrow_invocable_r_v<void, Invocable, std::wstring &>);

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

    static constexpr std::array<std::wstring_view, 30U> suffixes {
        L"ing",
        L"es",
        L"ness",
        L"ly", // ???

        L"а",
        L"ах",
        L"е",
        L"ое",
        L"ые",
        L"и",
        L"ами",
        L"ыми",
        L"ей",
        L"ой",
        L"ый",
        L"ам",
        L"ем",
        L"ом",
        L"ым",
        L"о",
        L"ого",
        L"у",
        L"ому",
        L"их",
        L"ых",
        L"ю",
        L"ою",
        L"ую",
        L"я",
        L"ая"
    };
    static_assert(
        std::is_sorted(suffixes.cbegin(), suffixes.cend(),
            [](
                const std::wstring_view wcs1, const std::wstring_view wcs2
            ) constexpr noexcept -> bool {
                return std::lexicographical_compare(
                    wcs1.crbegin(), wcs1.crend(), wcs2.crbegin(), wcs2.crend()
                ) || wcs1 == wcs2;
            }
        ),
        "suffixes must be unique and sorted"
    );
    static_assert(!suffixes.empty() && !suffixes.front().empty(),
        "all suffix must not be empty"
    );

    Invocable invocable_{};
};

template<typename Invocable>
constexpr stemmer<Invocable>::stemmer(const Invocable &invocable) noexcept(
    std::is_nothrow_copy_constructible_v<Invocable>
) : invocable_(invocable) {}

template<typename Invocable>
constexpr void stemmer<Invocable>::operator()(std::wstring &wcs) noexcept(
    std::is_nothrow_invocable_r_v<void, Invocable, std::wstring &>
) {
    using std::array, std::equal_range, std::logic_error, std::size_t,
        std::wstring_view;

    if (wcs.empty()) [[unlikely]]
        throw logic_error("stemmer::operator(): empty token");

    const size_t size = wcs.size();
    array::const_iterator first = suffixes.cbegin(), last = suffixes.cend();
    for (size_t i = 0; i < size; ++i) {
        [first, last] = equal_range(first, last, wcs,
            [i](
                const wstring_view wcs1, const wstring_view wcs2
            ) constexpr -> bool {
                assert(i < wcs1.size() || i < wcs2.size());
                assert(i == 0 || wcs1[wcs1.size() - i] == wcs2[wcs2.size() - i]);
                if (i >= wcs1.size())
                    return true;
                if (i >= wcs2.size())
                    return false;
                return wcs1[wcs1.size() - i - 1U] < wcs2[wcs2.size() - i - 1U];
            }
        );
    }
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
