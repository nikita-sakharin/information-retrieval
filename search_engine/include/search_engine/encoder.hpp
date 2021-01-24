#ifndef SEARCH_ENGINE_ENCODER_HPP
#define SEARCH_ENGINE_ENCODER_HPP

#include <cerrno> // errno
#include <climits> // MB_LEN_MAX
#include <clocale> // LC_ALL, setlocale
#include <cstddef> // size_t

#include <system_error> // generic_category, system_error
#include <type_traits> // is_invocable_r_v, is_nothrow_*_v, is_same_v

static_assert(__STDC_ISO_10646__ >= 201103L,
    "Unicode version 2011 or later required");

template<typename From, typename To, typename Invocable>
class encoder final {
public:
    constexpr encoder() noexcept(
        std::is_nothrow_default_constructible_v<Invocable>) = default;
    constexpr encoder(const Invocable &invocable) noexcept(
        std::is_nothrow_copy_constructible_v<Invocable>);
    constexpr encoder(const encoder &) noexcept(
        std::is_nothrow_copy_constructible_v<Invocable>) = default;
    constexpr encoder(encoder &&) noexcept(
        std::is_nothrow_move_constructible_v<Invocable>) = default;
    constexpr encoder &operator=(const encoder &) noexcept(
        std::is_nothrow_copy_assignable_v<Invocable>) = default;
    constexpr encoder &operator=(encoder &&) noexcept(
        std::is_nothrow_move_assignable_v<Invocable>) = default;
    constexpr ~encoder() noexcept(
        std::is_nothrow_destructible_v<Invocable>) = default;

    constexpr void operator()(From) noexcept( // enable_if
        std::is_nothrow_invocable_r_v<void, Invocable, To>);
    constexpr void operator()(basic_string_view<From>) noexcept( // enable_if
        std::is_nothrow_invocable_r_v<void, Invocable, To>); // string_view of span

private:
    static_assert(
        (std::is_same_v<From, char> || std::is_same_v<From, wchar_t>) &&
        (std::is_same_v<To, char> || std::is_same_v<To, wchar_t>),
        "template arguments From and To must both have type char or wchar_t"
    );
    static_assert(
        std::is_invocable_r_v<void, Invocable, To>,
        "invocable must have signature void(To)"
    );

    std::mbstate_t state{};
    Invocable invocable_{};
};

template<typename From, typename To, typename Invocable>
constexpr encoder<From, To, Invocable>::encoder(
    const Invocable &invocable
) noexcept(
    std::is_nothrow_copy_constructible_v<Invocable>
) : invocable_(invocable) {
    std::setlocale(LC_ALL, "en_US.utf8");
}

template<typename From, typename To, typename Invocable>
constexpr void encoder<From, To, Invocable>::operator()(
    const From from
) noexcept(std::is_nothrow_invocable_r_v<void, Invocable, To>) {
    invocable_(from);
}

template<typename Invocable>
constexpr void encoder<char, wchar_t, Invocable>::operator()(
    const char from
) noexcept(std::is_nothrow_invocable_r_v<void, Invocable, wchar_t>) {
    wchar_t wc;
    const std::size_t returns = mbrtowc(&wc, &from, 1U, &state);
    if (returns == static_cast<std::size_t>(-1)) [[unlikely]]
        throw system_error(errno, generic_category(), "encoder::operator()");
    if (returns == static_cast<std::size_t>(-2))
        return;
    invocable_(wc);
}

template<typename Invocable>
constexpr void encoder<wchar_t, char, Invocable>::operator()(
    const wchar_t from
) noexcept(std::is_nothrow_invocable_r_v<void, Invocable, char>) {
    std::array<char, MB_LEN_MAX> s;
    const std::size_t returns = wcrtomb(s.data(), from, &state);
    if (returns == static_cast<std::size_t>(-1)) [[unlikely]]
        throw system_error(errno, generic_category(), "encoder::operator()");
    foreach(s.cbegin(), s.cbegin() + returns, invocable_);
}

#endif
