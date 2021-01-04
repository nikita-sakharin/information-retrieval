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
}
/*
constexpr str_parser::str_parser(
    const char * const first,
    const char * const last
) noexcept : first_(), last_() {
    if (first_ > last_) [[unlikely]]
        throw logic_error("memmap::file::close: file is not open");
    assert(fildes_ >= 0);
}
*/
#endif
