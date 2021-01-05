#ifndef SEARCH_ENGINE_STR_PARSER_HPP
#define SEARCH_ENGINE_STR_PARSER_HPP

#include <iterator> // input_iterator_tag
#include <stdexcept> // logic_error
#include <string_view> // string_view

class str_parser final {
public:
    using iterator_category = std::input_iterator_tag;
    using difference_type = void;
    using value_type = std::string_view::value_type;
    using pointer = std::string_view::pointer;
    using reference = std::string_view::reference;

    constexpr str_parser() noexcept = default;
    constexpr explicit str_parser(std::string_view);
    constexpr str_parser(const char *, std::size_t);
    constexpr str_parser(const char *, const char *);
    constexpr str_parser(const str_parser &) noexcept = default;
    constexpr str_parser(str_parser &&) noexcept = default;
    constexpr str_parser &operator=(const str_parser &) noexcept = default;
    constexpr str_parser &operator=(str_parser &&) noexcept = default;
    constexpr ~str_parser() noexcept = default;

    friend constexpr bool operator==(
        const str_parser &,
        const str_parser &
    ) noexcept;
    friend constexpr bool operator!=(
        const str_parser &,
        const str_parser &
    ) noexcept = default;

    constexpr operator const char *() const noexcept;

private:
    std::string_view view_;
};

constexpr str_parser::str_parser(
    const std::string_view view
) : view_(view) {
    assert(view_.data() <= view_.data() + view_.size());

    if (view_.size() < 2 || view_.front() != '\"') [[unlikely]]
        throw std::logic_error("str_parser::str_parser: invalid string");
    view_ = view_.substr(1);
}

constexpr str_parser::str_parser(
    const char * const data,
    const std::size_t size
) : str_parser(std::string_view(data, size)) {}

constexpr str_parser::str_parser(
    const char * const first,
    const char * const last
) : str_parser(std::string_view(first, last)) {}

constexpr bool operator==(
    const str_parser &lhs,
    const str_parser &rhs
) noexcept {
    if (lhs.view_.size() == 0 && rhs.view_.size() == 0)
        return true;
    return lhs.view_ == rhs.view_;
}

constexpr str_parser::operator const char *() const noexcept {
    return view_.data();
}

#endif
