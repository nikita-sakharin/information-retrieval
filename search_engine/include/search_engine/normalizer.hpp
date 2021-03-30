#ifndef SEARCH_ENGINE_NORMALIZER_HPP
#define SEARCH_ENGINE_NORMALIZER_HPP

#include <cstddef> // size_t
#include <cwctype> // towlower

#include <string> // wstring
#include <string_view> // wstring_view
#include <type_traits> // is_invocable_r_v, is_nothrow_*_v

template<typename Invocable>
class normalizer final {
public:
    constexpr normalizer() noexcept(
        std::is_nothrow_default_constructible_v<Invocable>) = default;
    constexpr normalizer(const Invocable &) noexcept(
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

    constexpr void operator()(std::wstring &) noexcept( // TODO one-to-one or one-to-many ???
        std::is_nothrow_invocable_r_v<void, Invocable, std::size_t, std::wstring &>);

    constexpr const Invocable &invocable() const noexcept;
    constexpr Invocable &invocable() noexcept;

private:
    static_assert(__STDC_ISO_10646__ >= 201103L,
        "Unicode version 2011 or later required"
    );
    static_assert(std::is_invocable_r_v<void, Invocable, std::size_t, std::wstring &>,
        "Invocable must have signature void(size_t, wstring &)"
    );

    static constexpr std::wstring_view possessive_affix = L"'s";

    std::size_t position_ = 0U;
    Invocable invocable_{};
};

template<typename Invocable>
constexpr normalizer<Invocable>::normalizer(
    const Invocable &invocable
) noexcept(
    std::is_nothrow_copy_constructible_v<Invocable>
) : invocable_(invocable) {}

template<typename Invocable>
constexpr void normalizer<Invocable>::operator()(std::wstring &wcs) noexcept(
    std::is_nothrow_invocable_r_v<void, Invocable, std::size_t, std::wstring &>
) {
    using std::towlower;

    for (wchar_t &wc : wcs)
        wc = towlower(wc);
    invocable_(position_++, wcs);
}

template<typename Invocable>
constexpr const Invocable &normalizer<Invocable>::invocable() const noexcept {
    return invocable_;
}

template<typename Invocable>
constexpr Invocable &normalizer<Invocable>::invocable() noexcept {
    return invocable_;
}

#endif
