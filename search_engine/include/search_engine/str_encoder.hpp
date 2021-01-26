#ifndef SEARCH_ENGINE_STR_ENCODER_HPP
#define SEARCH_ENGINE_STR_ENCODER_HPP

#include <climits> // MB_LEN_MAX
#include <clocale> // LC_ALL, setlocale
#include <cstddef> // size_t
#include <cwchar> // mbrtowc, mbstate_t, wcrtomb

#include <algorithm> // for_each
#include <string> // basic_string
#include <string_view> // basic_string_view
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
        std::is_invocable_r_v<void, Invocable, std::basic_string<To> &>,
        "invocable must have signature void(std::basic_string<To> &)"
    );

    static constexpr std::size_t capacity = 1UL << 16;

    std::basic_string<To> buffer(capacity, static_cast<To>('\0'));
    std::mbstate_t state{};
    Invocable invocable_{};
};

template<typename From, typename To, typename Invocable>
constexpr str_encoder<From, To, Invocable>::str_encoder(
    const Invocable &invocable
) noexcept(
    std::is_nothrow_copy_constructible_v<Invocable>
) : invocable_(invocable) {
    using std::logic_error, std::setlocale;

    if (setlocale(LC_ALL, "en_US.utf8") == nullptr) [[unlikely]]
        throw logic_error("str_encoder::str_encoder: unable to set locale");
}

template<typename From, typename To, typename Invocable>
constexpr void str_encoder<From, To, Invocable>::operator()(
    std::basic_string_view<From> from
) {
    using std::array, std::for_each, std::generic_category, std::is_same_v,
        std::mbrtowc, std::size_t, std::system_error, std::wcrtomb;
    constexpr const char *what = "str_encoder::operator()";

    if constexpr (is_same_v<From, char> && is_same_v<To, wchar_t>) {
    } else if constexpr (is_same_v<From, wchar_t> && is_same_v<To, char>) {
    } else
        invocable_(from);
}

template<typename From, typename To, typename Invocable>
constexpr Invocable str_encoder<From, To, Invocable>::get_invocable(
) const noexcept(std::is_nothrow_copy_assignable_v<Invocable>) {
    return invocable_;
}

#endif
