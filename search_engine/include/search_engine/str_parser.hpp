#ifndef SEARCH_ENGINE_STR_PARSER_HPP
#define SEARCH_ENGINE_STR_PARSER_HPP

#include <stdexcept> // logic_error
#include <string_view> // string_view
#include <type_traits> // is_invocable_r_v

#include <search_engine/header.hpp>

class str_parser final {
public:
    constexpr str_parser() noexcept = default;
    constexpr str_parser(const str_parser &) noexcept = default;
    constexpr str_parser(str_parser &&) noexcept = default;
    constexpr str_parser &operator=(const str_parser &) noexcept = default;
    constexpr str_parser &operator=(str_parser &&) noexcept = default;
    constexpr ~str_parser() noexcept = default;

    template<typename Invocable>
    static constexpr std::string_view::const_iterator parse(
        std::string_view::const_iterator,
        std::string_view::const_iterator,
        const Invocable &
    );
/*
    constexpr std::string_view::const_iterator operator()(
        std::string_view::const_iterator,
        std::string_view::const_iterator
    );

    constexpr Invocable invocable() const noexcept(...) // by value!
*/
private:
    static constexpr uint hex_digit(char);

    static constexpr std::string_view::const_iterator parse_escape(
        std::string_view::const_iterator,
        std::string_view::const_iterator,
        char &
    );

    // Invocable invocable_{};
};

template<typename Invocable>
constexpr std::string_view::const_iterator str_parser::parse(
    std::string_view::const_iterator first,
    const std::string_view::const_iterator last,
    const Invocable &invocable
) {
    static_assert(std::is_invocable_r_v<void, Invocable, char>,
        "invocable must have signature void(char)");
    assert(first < last);

    if (*first != '\"') [[unlikely]]
        throw std::logic_error("str_parser::parse: invalid string");
    ++first;

    bool is_escape = false;
    while (first < last && (is_escape || *first != '\"')) {
        if (is_escape) [[unlikely]] {
            is_escape = false;
            char value;
            first = parse_escape(first, last, value);
            invocable(value);
        } else if (*first == '\\') [[unlikely]] {
            is_escape = true;
            ++first;
        } else [[likely]] {
            assert(*first != '\"');
            invocable(*first++);
        }
    }

    assert(first <= last);
    if (first == last) [[unlikely]]
        throw std::logic_error("str_parser::parse: string not properly ended");
    assert(!is_escape && *first == '\"');

    return first + 1;
}

constexpr uint str_parser::hex_digit(const char c) {
    if (c >= '0' && c <= '9') [[likely]]
        return c - '0';
    else if (c >= 'a' && c <= 'f') [[likely]] // upper hex not allowed
        return c - 'a'+ 10U;
    else [[unlikely]]
        throw std::logic_error("str_parser::hex_digit: invalid hex digit");
}

constexpr std::string_view::const_iterator str_parser::parse_escape(
    const std::string_view::const_iterator first,
    const std::string_view::const_iterator last,
    char &value_ref
) {
    constexpr const char *what =
        "str_parser::parse_escape: invalid escape sequence";
    assert(first < last);

    switch (*first) {
        [[likely]]   case '\"': value_ref = '\"'; return first + 1;
        [[likely]]   case '\\': value_ref = '\\'; return first + 1;
        [[unlikely]] case  'b': value_ref = '\b'; return first + 1;
        [[likely]]   case  'f': value_ref = '\f'; return first + 1;
        [[likely]]   case  'n': value_ref = '\n'; return first + 1;
        [[likely]]   case  'r': value_ref = '\r'; return first + 1;
        [[likely]]   case  't': value_ref = '\t'; return first + 1;
        [[unlikely]] case  'u': {
            if (first + 4 >= last || first[1] != '0' || first[2] != '0')
                [[unlikely]] throw std::logic_error(what);
            const uint value = hex_digit(first[3]) * 16 + hex_digit(first[4]);
            if (value >= 8 && value != 11 &&
                (value < 14 || value >= 32) && value != 127
            ) [[unlikely]] throw std::logic_error(what);
            value_ref = value;
            return first + 5;
        }
        [[unlikely]] default: throw std::logic_error(what);
    }
}

#endif
