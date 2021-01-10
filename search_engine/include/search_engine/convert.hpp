#ifndef SEARCH_ENGINE_CONVERT_HPP
#define SEARCH_ENGINE_CONVERT_HPP

#include <type_traits>

static_assert(__STDC_ISO_10646__ >= 201103L); // Unicode revision

template<typename From, typename To, typename Invocable>
class convert final {
public:
    constexpr convert() noexcept(
        std::is_nothrow_default_constructible_v<Invocable>) = default;
    constexpr convert(const Invocable &invocable) noexcept(
        std::is_nothrow_copy_constructible_v<Invocable>);
    constexpr convert(const convert &) noexcept(
        std::is_nothrow_copy_constructible_v<Invocable>) = default;
    constexpr convert(convert &&) noexcept(
        std::is_nothrow_move_constructible_v<Invocable>) = default;
    constexpr convert &operator=(const convert &) noexcept(
        std::is_nothrow_copy_assignable_v<Invocable>) = default;
    constexpr convert &operator=(convert &&) noexcept(
        std::is_nothrow_move_assignable_v<Invocable>) = default;
    constexpr ~convert() noexcept(
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
constexpr convert::convert(const Invocable &invocable) noexcept(
    std::is_nothrow_copy_constructible_v<Invocable>
) : invocable_(invocable) {}

template<typename From, typename To, typename Invocable>
constexpr void convert::operator()(const From from) noexcept(
    std::is_nothrow_invocable_r_v<void, Invocable, To>
) {
    if constexpr (is_same_v<From, char> && is_same_v<To, wchar_t>) {
    } else if constexpr (is_same_v<From, wchar_t> && is_same_v<To, char>) {
    } else invocable_(from);
}

#endif
