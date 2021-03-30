#ifndef SEARCH_ENGINE_NORMALIZER_HPP
#define SEARCH_ENGINE_NORMALIZER_HPP

#include <cwctype> // towlower

#include <string> // wstring

template<typename Invocable>
class normalizer final {
public:
    constexpr wstring &normalize(std::wstring &token) const noexcept;
};

constexpr wstring &normalizer::normalize(std::wstring &token) noexcept {
    for (wchar_t &wc : token)
        wc = towlower(wc);
    return token;
}

#endif
