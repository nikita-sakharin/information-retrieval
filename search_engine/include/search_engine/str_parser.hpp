#ifndef SEARCH_ENGINE_STR_PARSER_HPP
#define SEARCH_ENGINE_STR_PARSER_HPP

#include <cassert> // assert

#include <stdexcept> // logic_error
#include <string_view> // string_view
#include <type_traits> // is_invocable_r_v

#include <search_engine/types.hpp>

template<typename Invocable>
class str_parser final {
public:
    constexpr str_parser() noexcept(
        std::is_nothrow_default_constructible_v<Invocable>) = default;
    constexpr explicit str_parser(const Invocable &) noexcept(
        std::is_nothrow_copy_constructible_v<Invocable>);
    constexpr str_parser(const str_parser &) noexcept(
        std::is_nothrow_copy_constructible_v<Invocable>) = default;
    constexpr str_parser(str_parser &&) noexcept(
        std::is_nothrow_move_constructible_v<Invocable>) = default;
    constexpr str_parser &operator=(const str_parser &) noexcept(
        std::is_nothrow_copy_assignable_v<Invocable>) = default;
    constexpr str_parser &operator=(str_parser &&) noexcept(
        std::is_nothrow_move_assignable_v<Invocable>) = default;
    constexpr ~str_parser() noexcept(
        std::is_nothrow_destructible_v<Invocable>) = default;

    constexpr std::string_view::const_iterator operator()(
        std::string_view::const_iterator,
        std::string_view::const_iterator
    );

    constexpr const Invocable &invocable() const noexcept;
    constexpr Invocable &invocable() noexcept;

private:
    static_assert(std::is_invocable_r_v<void, Invocable, char>,
        "Invocable must have signature void(char)"
    );

    static constexpr uint hex_digit(char);

    constexpr std::string_view::const_iterator parse_escape(
        std::string_view::const_iterator,
        std::string_view::const_iterator
    );

    Invocable invocable_{};
};

template<typename Invocable>
constexpr str_parser<Invocable>::str_parser(
    const Invocable &invocable
) noexcept(
    std::is_nothrow_copy_constructible_v<Invocable>
) : invocable_(invocable) {}

template<typename Invocable>
constexpr std::string_view::const_iterator str_parser<Invocable>::operator()(
    std::string_view::const_iterator first,
    const std::string_view::const_iterator last
) {
    using std::logic_error;

    if (first >= last) [[unlikely]] throw logic_error(
        "str_parser::operator(): first must be less than last"
    );
    if (*first != '\"') [[unlikely]]
        throw logic_error("str_parser::operator(): invalid string");
    ++first;

    bool is_escape = false;
    while (first < last && (is_escape || *first != '\"')) {
        if (is_escape) [[unlikely]] {
            is_escape = false;
            first = parse_escape(first, last);
        } else if (*first == '\\') [[unlikely]] {
            is_escape = true;
            ++first;
        } else [[likely]] {
            assert(*first != '\"');
            invocable_(*first++);
        }
    }

    assert(first <= last);
    if (first == last) [[unlikely]] throw logic_error(
        "str_parser::operator(): string is not properly ended"
    );
    assert(!is_escape && *first == '\"');

    return first + 1;
}

template<typename Invocable>
constexpr const Invocable &str_parser<Invocable>::invocable() const noexcept {
    return invocable_;
}

template<typename Invocable>
constexpr Invocable &str_parser<Invocable>::invocable() noexcept {
    return invocable_;
}

template<typename Invocable>
constexpr uint str_parser<Invocable>::hex_digit(const char c) {
    using std::logic_error;

    if (c >= '0' && c <= '9') [[likely]]
        return c - '0';
    else if (c >= 'a' && c <= 'f') [[likely]]
        return c - 'a' + 10U;
    else [[unlikely]]
        throw logic_error("str_parser::hex_digit: invalid hex digit");
}

template<typename Invocable>
constexpr std::string_view::const_iterator str_parser<Invocable>::parse_escape(
    const std::string_view::const_iterator first,
    const std::string_view::const_iterator last
) {
    using std::logic_error;
    constexpr const char *what =
        "str_parser::parse_escape: invalid escape sequence";
    assert(first < last);

    switch (*first) {
        [[likely]]   case '\"': invocable_('\"'); return first + 1;
        [[likely]]   case '\\': invocable_('\\'); return first + 1;
        [[unlikely]] case  'b': invocable_('\b'); return first + 1;
        [[likely]]   case  'f': invocable_('\f'); return first + 1;
        [[likely]]   case  'n': invocable_('\n'); return first + 1;
        [[likely]]   case  'r': invocable_('\r'); return first + 1;
        [[likely]]   case  't': invocable_('\t'); return first + 1;
        [[unlikely]] case  'u': {
            if (first + 4 >= last || first[1] != '0' || first[2] != '0')
                [[unlikely]] throw logic_error(what);
            const uint value = hex_digit(first[3]) * 16U + hex_digit(first[4]);
            if (value >= 8U && value != 11U &&
                (value < 14U || value >= 32U) && value != 127U
            ) [[unlikely]] throw logic_error(what);
            invocable_(value);
            return first + 5;
        }
        [[unlikely]] default: throw logic_error(what);
    }
}

#endif
