#ifndef SEARCH_ENGINE_JSON_PARSER_HPP
#define SEARCH_ENGINE_JSON_PARSER_HPP

#include <iterator> // input_iterator_tag
#include <string_view> // string_view
#include <utility> // pair

#include <search_engine/str_parser.hpp>

class json_parser final {
public:
    using iterator_category = std::input_iterator_tag;
    using difference_type = void;
    using value_type = std::pair<std::string_view, str_parser>;
    using pointer = void;
    using reference = void;

    constexpr json_parser() noexcept = default;
    constexpr explicit json_parser(std::string_view) noexcept;
    constexpr json_parser(const char *, std::size_t) noexcept;
    constexpr json_parser(const char *, const char *) noexcept;
    constexpr json_parser(const json_parser &) noexcept = default;
    constexpr json_parser(json_parser &&) noexcept = default;
    constexpr json_parser &operator=(const json_parser &) noexcept = default;
    constexpr json_parser &operator=(json_parser &&) noexcept = default;
    constexpr ~json_parser() noexcept = default;

    std::pair<std::string_view, str_parser> operator*(); // TODO reference???
    json_parser &operator++(); // TODO reference???

private:
    string_view view_;
};

constexpr json_parser::json_parser(
    const std::string_view view
) noexcept : view_(view) { // delegating constructor => fewer assert
    assert(view_.data() <= view_.data() + view_.size());
}

constexpr json_parser::json_parser(
    const char * const data,
    const std::size_t size
) noexcept : view_(data, size) { // delegating constructor => fewer assert
    assert(view_.data() <= view_.data() + view_.size());
}

constexpr json_parser::json_parser(
    const char * const first,
    const char * const last
) noexcept : first_(), last_() { // delegating constructor => fewer assert
    assert(view_.data() <= view_.data() + view_.size());
}
/*
    // const char *first_, *last_;
constexpr json_parser::json_parser() noexcept : first_(), last_() {}

constexpr json_parser::json_parser(
    const char * const data,
    const std::size_t size
) noexcept : first_(data), last_(data + size) {
    assert(first_ <= last_);
}

constexpr json_parser::json_parser(
    const char * const first,
    const char * const last
) noexcept : first_(), last_() {
    if (first_ > last_) [[unlikely]]
        throw logic_error("memmap::file::close: file is not open");
    assert(fildes_ >= 0);
}
*/
#endif
