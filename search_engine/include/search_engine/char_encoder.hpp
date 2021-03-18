#ifndef SEARCH_ENGINE_CHAR_ENCODER_HPP
#define SEARCH_ENGINE_CHAR_ENCODER_HPP

#include <cerrno> // errno
#include <climits> // MB_LEN_MAX
#include <cstddef> // size_t
#include <cwchar> // mbrtowc, mbstate_t, wcrtomb

#include <array> // array
#include <string_view> // string_view
#include <system_error> // generic_category, system_error
#include <type_traits> // is_invocable_r_v, is_nothrow_*_v, is_same_v

template<typename From, typename To, typename Invocable>
class char_encoder final {
public:
    constexpr char_encoder() noexcept(
        std::is_nothrow_default_constructible_v<Invocable>) = default;
    constexpr char_encoder(const Invocable &) noexcept(
        std::is_nothrow_copy_constructible_v<Invocable>);
    constexpr char_encoder(const char_encoder &) noexcept(
        std::is_nothrow_copy_constructible_v<Invocable>) = default;
    constexpr char_encoder(char_encoder &&) noexcept(
        std::is_nothrow_move_constructible_v<Invocable>) = default;
    constexpr char_encoder &operator=(const char_encoder &) noexcept(
        std::is_nothrow_copy_assignable_v<Invocable>) = default;
    constexpr char_encoder &operator=(char_encoder &&) noexcept(
        std::is_nothrow_move_assignable_v<Invocable>) = default;
    constexpr ~char_encoder() noexcept(
        std::is_nothrow_destructible_v<Invocable>) = default;

    constexpr void operator()(From);

    constexpr const Invocable &invocable() const noexcept;
    constexpr Invocable &invocable() noexcept;

private:
    static_assert(
        (std::is_same_v<From, char> || std::is_same_v<From, wchar_t>) &&
        (std::is_same_v<To, char> || std::is_same_v<To, wchar_t>),
        "template arguments From and To must both have type char or wchar_t"
    );
    static_assert(!std::is_same_v<From, To>,
        "template arguments From and To must have different types"
    );
    static_assert(
        std::is_invocable_r_v<void, Invocable, To>,
        "Invocable must have signature void(To)"
    );

    std::mbstate_t state_{};
    Invocable invocable_{};
};

template<typename From, typename To, typename Invocable>
constexpr char_encoder<From, To, Invocable>::char_encoder(
    const Invocable &invocable
) noexcept(
    std::is_nothrow_copy_constructible_v<Invocable>
) : invocable_(invocable) {}

template<typename From, typename To, typename Invocable>
constexpr void char_encoder<From, To, Invocable>::operator()(
    const From from
) {
    using std::array, std::generic_category, std::is_same_v, std::mbrtowc,
        std::size_t, std::string_view, std::system_error, std::wcrtomb;
    constexpr const char *what = "char_encoder::operator()";

    if constexpr (is_same_v<From, char>) {
        wchar_t wc;
        const size_t size = mbrtowc(&wc, &from, 1U, &state_);
        if (size == static_cast<size_t>(-1)) [[unlikely]]
            throw system_error(errno, generic_category(), what);
        if (size == static_cast<size_t>(-2))
            return;
        invocable_(wc);
    } else {
        array<char, MB_LEN_MAX> str;
        char * const data = str.data();
        const size_t size = wcrtomb(data, from, &state_);
        if (size == static_cast<size_t>(-1)) [[unlikely]]
            throw system_error(errno, generic_category(), what);
        for (const char value : string_view(data, size))
            invocable_(value);
    }
}

template<typename From, typename To, typename Invocable>
constexpr const Invocable &char_encoder<From, To, Invocable>::invocable(
) const noexcept {
    return invocable_;
}

template<typename From, typename To, typename Invocable>
constexpr Invocable &char_encoder<From, To, Invocable>::invocable() noexcept {
    return invocable_;
}

#endif
