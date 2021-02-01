#ifndef SEARCH_ENGINE_STR_ENCODER_HPP
#define SEARCH_ENGINE_STR_ENCODER_HPP

#include <cstddef> // size_t
#include <cstdlib> // mbstowcs, wcstombs

#include <bit> // bit_ceil
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

    template<typename Encoder>
    constexpr void encode(const std::basic_string<From> &, Encoder);

    std::basic_string<To> buffer =
        std::basic_string<To>(1U << 12, static_cast<To>('\0'));
    Invocable invocable_{};
};

template<typename From, typename To, typename Invocable>
constexpr str_encoder<From, To, Invocable>::str_encoder(
    const Invocable &invocable
) noexcept(
    std::is_nothrow_copy_constructible_v<Invocable>
) : invocable_(invocable) {}

template<typename From, typename To, typename Invocable>
constexpr void str_encoder<From, To, Invocable>::operator()(
    const std::basic_string<From> &from
) {
    using std::is_same_v, std::mbstowcs, std::wcstombs; // cstdlib

    if constexpr (is_same_v<From, char> && is_same_v<To, wchar_t>)
        encode(from, mbstowcs);
    else if constexpr (is_same_v<From, wchar_t> && is_same_v<To, char>)
        encode(from, wcstombs);
    else invocable_(buffer = from);
}

template<typename From, typename To, typename Invocable>
constexpr Invocable str_encoder<From, To, Invocable>::get_invocable(
) const noexcept(std::is_nothrow_copy_assignable_v<Invocable>) {
    return invocable_;
}

template<typename From, typename To, typename Invocable>
template<typename Encoder>
constexpr void str_encoder<From, To, Invocable>::encode(
    const std::basic_string<From> &from,
    const Encoder encoder
) {
    using std::bit_ceil, std::generic_category, std::is_invocable_r_v,
        std::is_same_v, std::size_t, std::system_error;
    static_assert(!is_same_v<From, To>,
        "template arguments From and To must have different types");
    static_assert(is_invocable_r_v<size_t, Encoder, To *, const From *, size_t>,
        "encoder must have signature size_t(To *, const From *, size_t)");

    const size_t max_size = bit_ceil(from.size() * MB_CUR_MAX); // TODO
    buffer.resize(max_size);
    const size_t size = encoder(buffer.data(), from.c_str(), max_size);
    if (size == static_cast<size_t>(-1)) [[unlikely]]
        throw system_error(errno, generic_category(), "str_encoder::encode");
    buffer.resize(size);
    invocable_(buffer);
}

#endif
