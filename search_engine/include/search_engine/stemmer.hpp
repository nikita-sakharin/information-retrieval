#ifndef SEARCH_ENGINE_STEMMER_HPP
#define SEARCH_ENGINE_STEMMER_HPP

#include <algorithm> // is_sorted, lexicographical_compare
#include <array> // array
#include <string> // wstring
#include <string_view> // wstring_view

class stemmer final {
public:
    constexpr std::wstring &stem(std::wstring &) const noexcept;

private:
    static constexpr std::array<std::wstring_view, 13> suffixes = { // " = {" or "{" ???
        L"ое",
        L"ые",
        L"ыми",
        L"ой",
        L"ый",
        L"ом",
        L"ым",
        L"ого",
        L"ому",
        L"ых",
        L"ою",
        L"ую",
        L"ая"
    };
    static_assert(std::is_sorted(suffixes.cbegin(), suffixes.cend(),
            [](
                const std::wstring_view wcs1, const std::wstring_view wcs2
            ) constexpr noexcept -> bool {
                return std::lexicographical_compare(
                    wcs1.crbegin(), wcs1.crend(), wcs2.crbegin(), wcs2.crend()
                );
            }
        ),
        "suffixes must be sorted"
    );
    static_assert(*suffixes.cbegin() != std::wstring_view(),
        "all suffix must not be empty"
    );
};

constexpr std::wstring &stemmer::stem(std::wstring &wcs) const noexcept {
    return wcs;
}

#endif
