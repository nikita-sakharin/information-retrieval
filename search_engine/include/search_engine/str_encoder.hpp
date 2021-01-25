#ifndef SEARCH_ENGINE_STR_ENCODER_HPP
#define SEARCH_ENGINE_STR_ENCODER_HPP

#include <cerrno> // errno
#include <climits> // MB_LEN_MAX
#include <clocale> // LC_ALL, setlocale
#include <cstddef> // size_t
#include <cwchar> // mbrtowc, mbstate_t, wcrtomb

#include <algorithm> // for_each
#include <system_error> // generic_category, system_error
#include <type_traits> // is_invocable_r_v, is_nothrow_*_v, is_same_v

static_assert(__STDC_ISO_10646__ >= 201103L,
    "Unicode version 2011 or later required");

template<typename From, typename To, typename Invocable>
class str_encoder final {
public:
    constexpr str_encoder() noexcept(
        std::is_nothrow_default_constructible_v<Invocable>) = default;
    constexpr str_encoder(const Invocable &invocable) noexcept(
        std::is_nothrow_copy_constructible_v<Invocable>);
    constexpr str_encoder(const str_encoder &) noexcept(
        std::is_nothrow_copy_constructible_v<Invocable>) = default;
    constexpr str_encoder(str_encoder &&) noexcept(
        std::is_nothrow_move_constructible_v<Invocable>) = default;
    constexpr str_encoder &operator=(const str_encoder &) noexcept(
        std::is_nothrow_copy_assignable_v<Invocable>) = default;
    constexpr str_encoder &operator=(str_encoder &&) noexcept(
        std::is_nothrow_move_assignable_v<Invocable>) = default;
    constexpr ~str_encoder() noexcept(
        std::is_nothrow_destructible_v<Invocable>) = default;

    constexpr void operator()(std::basic_string_view<From>);

    constexpr Invocable get_invocable() const noexcept(
        std::is_nothrow_copy_assignable_v<Invocable>);

private:
    static_assert(
        (std::is_same_v<From, char> || std::is_same_v<From, wchar_t>) &&
        (std::is_same_v<To, char> || std::is_same_v<To, wchar_t>),
        "template arguments From and To must both have type char or wchar_t"
    );
    static_assert(
        std::is_invocable_r_v<void, Invocable, std::basic_string<To>>,
        "invocable must have signature void(To)"
    );

    std::mbstate_t state{};
    Invocable invocable_{};
};

template<typename From, typename To, typename Invocable>
constexpr str_encoder<From, To, Invocable>::str_encoder(
    const Invocable &invocable
) noexcept(
    std::is_nothrow_copy_constructible_v<Invocable>
) : invocable_(invocable) {
    if (std::setlocale(LC_ALL, "en_US.utf8") == nullptr) [[unlikely]]
        throw std::logic_error("str_encoder::str_encoder: unable to set locale");
}

template<typename From, typename To, typename Invocable>
constexpr void str_encoder<From, To, Invocable>::operator()(
    const From from
) {
    using std::array, std::for_each, std::generic_category, std::is_same_v,
        std::mbrtowc, std::size_t, std::system_error, std::wcrtomb;
    constexpr const char *what = "str_encoder::operator()";

    if constexpr (is_same_v<From, char> && is_same_v<To, wchar_t>) {
        wchar_t wc;
        const size_t returns = mbrtowc(&wc, &from, 1U, &state);
        if (returns == static_cast<size_t>(-1)) [[unlikely]]
            throw system_error(errno, generic_category(), what);
        if (returns == static_cast<size_t>(-2))
            return;
        invocable_(wc);
    } else if constexpr (is_same_v<From, wchar_t> && is_same_v<To, char>) {
        array<char, MB_LEN_MAX> s;
        const size_t returns = wcrtomb(s.data(), from, &state);
        if (returns == static_cast<size_t>(-1)) [[unlikely]]
            throw system_error(errno, generic_category(), what);
        for_each(s.cbegin(), s.cbegin() + returns, invocable_);
    } else
        invocable_(from);
}

template<typename From, typename To, typename Invocable>
constexpr Invocable str_encoder<From, To, Invocable>::get_invocable(
) const noexcept(std::is_nothrow_copy_assignable_v<Invocable>) {
    return invocable_;
}

#endif
