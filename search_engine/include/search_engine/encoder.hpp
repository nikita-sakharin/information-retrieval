#ifndef SEARCH_ENGINE_ENCODER_HPP
#define SEARCH_ENGINE_ENCODER_HPP

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

    constexpr void operator()(From) noexcept(
        std::is_nothrow_invocable_r_v<void, Invocable, To>);

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

    Invocable invocable_{};
};

template<typename, typename, typename Invocable>
constexpr encoder::encoder(const Invocable &invocable) noexcept(
    std::is_nothrow_copy_constructible_v<Invocable>
) : invocable_(invocable) {}

template<typename From, typename To, typename Invocable>
constexpr void encoder::operator()(const From from) noexcept(
    std::is_nothrow_invocable_r_v<void, Invocable, To>
) {
    if constexpr (is_same_v<From, char> && is_same_v<To, wchar_t>) {
    } else if constexpr (is_same_v<From, wchar_t> && is_same_v<To, char>) {
    } else invocable_(from);
}

#endif
