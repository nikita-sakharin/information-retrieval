#ifndef SEARCH_ENGINE_NORMALIZER_HPP
#define SEARCH_ENGINE_NORMALIZER_HPP

#include <cwctype> // towlower

#include <string> // wstring
#include <string_view> // wstring_view
#include <type_traits> // is_invocable_r_v, is_nothrow_*_v

template<typename Invocable>
class normalizer final {
public:
    constexpr void operator()(std::wstring &) noexcept( // one-to-one or one-to-many ???
        std::is_nothrow_invocable_r_v<void, Invocable, std::size_t, std::wstring &>);

private:
    static_assert(__STDC_ISO_10646__ >= 201103L,
        "Unicode version 2011 or later required"
    );
    static_assert(std::is_invocable_r_v<void, Invocable, std::size_t, std::wstring &>,
        "Invocable must have signature void(wstring &)"
    );

    static constexpr std::wstring_view possessive_affix = L"'s";

    std::size_t position_ = 0U;
    Invocable invocable_{};
};

template<typename Invocable>
constexpr void normalizer::operator()(std::wstring &wcs) noexcept(
    std::is_nothrow_invocable_r_v<void, Invocable, std::size_t, std::wstring &>
) {
    using std::towlower;

    for (wchar_t &wc : wcs)
        wc = towlower(wc);
    invocable_(position_++, buffer_);
}

#endif
