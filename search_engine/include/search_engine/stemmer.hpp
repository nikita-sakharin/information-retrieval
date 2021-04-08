#ifndef SEARCH_ENGINE_STEMMER_HPP
#define SEARCH_ENGINE_STEMMER_HPP

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

    static constexpr std::array<std::wstring_view, 18U> suffixes {
        L"ing",
        L"es",
        L"ness",
        L"ly", // ???

        L"а",
        L"ое",
        L"ые",
        L"ыми",
        L"ой",
        L"ый",
        L"ам",
        L"ом",
        L"ым",
        L"о",
        L"ого",
        L"у",
        L"ому",
        L"их",
        L"ых",
        L"ою",
        L"ую",
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
    using std::equal_range, std::lexicographical_compare, std::logic_error,
        std::wstring_view;

    if (wcs.empty()) [[unlikely]]
        throw logic_error("stemmer::operator(): empty token");

    array::const_iterator first = suffixes.cbegin(), last = suffixes.cend();
    for (size_t i = 0; ; ++i) {
        [first, last] = equal_range(first, last, wcs,
            [](
                const wstring_view wcs1, const wstring_view wcs2
            ) constexpr -> bool {
            }
        );
    }
    invocable_(wcs);
/*
    equal_range(suffixes.cbegin(), suffixes.cend(), lexicographical_compare(
        wcs1.crbegin(), wcs1.crend(), wcs2.crbegin(), wcs2.crend()
    ));
*/
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
