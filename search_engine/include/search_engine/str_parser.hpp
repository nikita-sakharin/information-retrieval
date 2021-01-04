#ifndef SEARCH_ENGINE_STR_PARSER_HPP
#define SEARCH_ENGINE_STR_PARSER_HPP

#include <iterator> // input_iterator_tag
#include <string_view> // string_view

class str_parser final {
public:
    using iterator_category = std::input_iterator_tag;
    using difference_type = std::string_view::difference_type;
    using value_type = std::string_view::value_type;
    using pointer = std::string_view::pointer;
    using reference = std::string_view::reference;

    constexpr str_parser() noexcept = default;
    constexpr explicit str_parser(std::string_view) noexcept;
    constexpr str_parser(const char *, std::size_t) noexcept;
    constexpr str_parser(const char *, const char *) noexcept;
    constexpr str_parser(const str_parser &) noexcept = default;
    constexpr str_parser(str_parser &&) noexcept = default;
    constexpr str_parser &operator=(const str_parser &) noexcept = default;
    constexpr str_parser &operator=(str_parser &&) noexcept = default;
    constexpr ~str_parser() noexcept = default;

    friend constexpr bool operator==(str_parser, str_parser) noexcept = default;
    friend constexpr bool operator!=(str_parser, str_parser) noexcept = default;

private:
    std::string_view view_;
};

constexpr str_parser::str_parser(
    const std::string_view view
) noexcept : view_(view) {
    assert(view_.data() <= view_.data() + view_.size());

    if (view_.data() < 2 || view_.front() != '\"') [[unlikely]]
        throw logic_error("str_parser::str_parser: invalid string");
    else
        view_ = view_.substr(1);
}

constexpr str_parser::str_parser(
    const char * const data,
    const std::size_t size
) noexcept : str_parser(std::string_view(data, size)) {}

constexpr str_parser::str_parser(
    const char * const first,
    const char * const last
) noexcept : str_parser(std::string_view(first, last)) {}

#endif
