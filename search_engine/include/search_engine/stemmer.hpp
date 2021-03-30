#ifndef SEARCH_ENGINE_STEMMER_HPP
#define SEARCH_ENGINE_STEMMER_HPP

#include <algorithm> // is_sorted
#include <array> // array
#include <string> // wstring
#include <string_view> // wstring_view

class stemmer final {
public:
    constexpr std::wstring &stem(std::wstring &) const noexcept;

private:
    std::array<, std::wstring_view> suffixes = { // " = {" or "{" ???
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
    static_assert(std::is_sorted(suffixes.cbegin(), suffixes.cend()),
        "suffixes must be sorted");
};

constexpr std::wstring &stemmer::stem(std::wstring &wcs) const noexcept {
    return wcs;
}

#endif
