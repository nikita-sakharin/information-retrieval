#ifndef SEARCH_ENGINE_STR_PARSER_HPP
#define SEARCH_ENGINE_STR_PARSER_HPP

#include <cstdio> // EOF

#include <iterator> // input_iterator_tag
#include <locale> // isxdigit, locale
#include <stdexcept> // logic_error
#include <string_view> // string_view

#include <search_engine/header.hpp>

class str_parser final {
public:
    using iterator_category = std::input_iterator_tag;
    using difference_type = void;
    using value_type = char;
    using pointer = void;
    using reference = void;

    constexpr str_parser() noexcept = default;
    constexpr explicit str_parser(std::string_view);
    constexpr str_parser(const char *, std::size_t);
    constexpr str_parser(const char *, const char *);
    constexpr str_parser(const str_parser &) noexcept = default;
    constexpr str_parser(str_parser &&) noexcept = default;
    constexpr str_parser &operator=(const str_parser &) noexcept = default;
    constexpr str_parser &operator=(str_parser &&) noexcept = default;
    constexpr ~str_parser() noexcept = default;

    constexpr const char &operator*() const;
    constexpr const char *operator->() const;
    constexpr str_parser &operator++();
    constexpr str_parser operator++(int);

    constexpr explicit operator const char *() const noexcept;

    friend constexpr bool operator==(
        const str_parser &,
        const str_parser &
    ) noexcept;
    friend constexpr bool operator!=(
        const str_parser &,
        const str_parser &
    ) noexcept = default;

private:
    static constexpr uint hex_digit(char);
    constexpr char parse_escape();

    std::string_view view_;
    char value_ = -1;
};

constexpr str_parser::str_parser(
    const std::string_view view
) : view_(view) {
    assert(view_.data() <= view_.data() + view_.size());

    if (view_.size() < 2 || view_.front() != '\"') [[unlikely]]
        throw std::logic_error("str_parser::str_parser: invalid string");
    view_.remove_prefix(1);
    operator++();
}

constexpr str_parser::str_parser(
    const char * const data,
    const std::size_t size
) : str_parser(std::string_view(data, size)) {}

constexpr str_parser::str_parser(
    const char * const first,
    const char * const last
) : str_parser(std::string_view(first, last)) {}

constexpr const char &str_parser::operator*() const {
    return value_;
}

constexpr const char *str_parser::operator->() const {
    return &value_;
}

constexpr str_parser &str_parser::operator++() {
    constexpr const char *what =
        "str_parser::operator++: string not properly ended";
    assert(view_.data() < view_.data() + view_.size());

    if (view_.size() == 1 && view_.front() != '\"') [[unlikely]]
        throw std::logic_error(what);

    if (view_.front() == '\\') [[unlikely]] {
        view_.remove_prefix(1);
        value_ = parse_escape();
        if (view_.size() == 0) [[unlikely]] throw std::logic_error(what);
    } else if (view_.front() == '\"') [[unlikely]] {
        view_.remove_prefix(1);
        view_.remove_suffix(view_.size());
    } else [[likely]] {
        value_ = view_.front();
        view_.remove_prefix(1);
    }
    return *this;
}

constexpr str_parser str_parser::operator++(int) {
    const str_parser temp(*this);
    operator++();
    return temp;
}

constexpr str_parser::operator const char *() const noexcept {
    return view_.data();
}

constexpr bool operator==(
    const str_parser &lhs,
    const str_parser &rhs
) noexcept {
    if (lhs.view_.size() == 0 && rhs.view_.size() == 0) [[unlikely]]
        return true;
    return lhs.view_ == rhs.view_;
}

constexpr uint str_parser::hex_digit(const char c) {
    if (c >= '0' && c <= '9') [[likely]]
        return c - '0';
    else if (c >= 'a' && c <= 'f') [[likely]] // upper hex not allowed
        return 10U + c - 'a';
    else [[unlikely]]
        throw std::logic_error("str_parser::hex_digit: invalid hex digit");
}

constexpr char str_parser::parse_escape() {
    constexpr const char *what =
        "str_parser::parse_escape: invalid escape sequence";
    assert(view_.data() < view_.data() + view_.size());

    switch (view_.front()) {
        [[likely]]   case '\"': view_.remove_prefix(1); return '\"';
        [[likely]]   case '\\': view_.remove_prefix(1); return '\\';
        [[unlikely]] case  'b': view_.remove_prefix(1); return '\b';
        [[likely]]   case  'f': view_.remove_prefix(1); return '\f';
        [[likely]]   case  'n': view_.remove_prefix(1); return '\n';
        [[likely]]   case  'r': view_.remove_prefix(1); return '\r';
        [[likely]]   case  't': view_.remove_prefix(1); return '\t';
        [[unlikely]] case  'u': {
            if (view_.size() < 5 || view_[1] != '0' || view_[2] != '0')
                [[unlikely]] throw std::logic_error(what);
            const uint value = hex_digit(view_[3]) * 16 + hex_digit(view_[4]);
            if (value >= 8 && value != 11 &&
                (value < 14 || value >= 32) && value != 127
            ) [[unlikely]] throw std::logic_error(what);
            view_.remove_prefix(5);
            return static_cast<char>(value);
        }
        [[unlikely]] default: throw std::logic_error(what);
    }
}

#endif
