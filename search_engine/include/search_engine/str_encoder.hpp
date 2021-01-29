#ifndef SEARCH_ENGINE_STR_ENCODER_HPP
#define SEARCH_ENGINE_STR_ENCODER_HPP

#include <clocale> // LC_ALL, setlocale
#include <cstddef> // size_t
#include <cwchar> // mbstowcs, wcstombs

#include <string> // basic_string
#include <system_error> // generic_category, system_error
#include <type_traits> // is_invocable_r_v, is_nothrow_*_v, is_same_v

static_assert(__STDC_ISO_10646__ >= 201103L,
    "Unicode version 2011 or later required");

template<typename From, typename To, typename Invocable>
class str_encoder final {
public:
    constexpr str_encoder() = default;
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

    constexpr void operator()(const std::basic_string<From> &);

    constexpr Invocable get_invocable() const noexcept(
        std::is_nothrow_copy_assignable_v<Invocable>);

private:
    static_assert(
        (std::is_same_v<From, char> || std::is_same_v<From, wchar_t>) &&
        (std::is_same_v<To, char> || std::is_same_v<To, wchar_t>),
        "template arguments From and To must both have type char or wchar_t"
    );
    static_assert(
        std::is_invocable_r_v<void, Invocable, std::basic_string<To> &>,
        "invocable must have signature void(std::basic_string<To> &)"
    );

    static constexpr std::size_t capacity = 1U << 12;

    std::basic_string<To> buffer =
        std::basic_string<To>(capacity, static_cast<To>('\0'));
    Invocable invocable_{};
};

template<typename From, typename To, typename Invocable>
constexpr str_encoder<From, To, Invocable>::str_encoder(
    const Invocable &invocable
) noexcept(
    std::is_nothrow_copy_constructible_v<Invocable>
) : invocable_(invocable) {
    using std::logic_error, std::setlocale;
    constexpr const char *locale = "en_US.utf8";

    if (setlocale(LC_ALL, locale) == nullptr) [[unlikely]]
        throw logic_error("str_encoder::str_encoder: unable to set locale");
}

template<typename From, typename To, typename Invocable>
constexpr void str_encoder<From, To, Invocable>::operator()(
    const std::basic_string<From> &from
) {
    using std::generic_category, std::is_same_v, std::mbstowcs, std::size_t,
        std::system_error, std::wcstombs;
    constexpr const char *what = "str_encoder::operator()";

    if constexpr (is_same_v<From, char> && is_same_v<To, wchar_t>) {
        const size_t max_size = from.size();
        buffer.resize(max_size);
        const size_t size = mbstowcs(buffer.data(), from.c_str(), max_size);
        if (size == static_cast<size_t>(-1)) [[unlikely]]
            throw system_error(errno, generic_category(), what);
        buffer.resize(size);
        invocable_(buffer);
    } else if constexpr (is_same_v<From, wchar_t> && is_same_v<To, char>) {
        const size_t max_size = from.size() * MB_CUR_MAX;
        buffer.resize(max_size);
        const size_t size = wcstombs(buffer.data(), from.c_str(), max_size);
        if (size == static_cast<size_t>(-1)) [[unlikely]]
            throw system_error(errno, generic_category(), what);
        buffer.resize(size);
        invocable_(buffer);
    } else {
        buffer = from;
        invocable_(buffer);
    }
}

template<typename From, typename To, typename Invocable>
constexpr Invocable str_encoder<From, To, Invocable>::get_invocable(
) const noexcept(std::is_nothrow_copy_assignable_v<Invocable>) {
    return invocable_;
}

#endif
