#ifndef SEARCH_ENGINE_TOKENIZER_HPP
#define SEARCH_ENGINE_TOKENIZER_HPP

#include <cstddef> // size_t
#include <cwctype> // iswalnum, iswpunct, iswspace

#include <string> // wstring
#include <type_traits> // is_invocable_r_v, is_nothrow_*_v

static_assert(__STDC_ISO_10646__ >= 201103L,
    "Unicode version 2011 or later required");

template<typename Invocable>
class tokenizer final {
public:
    constexpr tokenizer() noexcept(
        std::is_nothrow_default_constructible_v<Invocable>) = default;
    constexpr tokenizer(const Invocable &) noexcept(
        std::is_nothrow_copy_constructible_v<Invocable>);
    constexpr tokenizer(const tokenizer &) = default;
    constexpr tokenizer(tokenizer &&) noexcept(
        std::is_nothrow_move_constructible_v<Invocable>) = default;
    constexpr tokenizer &operator=(const tokenizer &) = default;
    constexpr tokenizer &operator=(tokenizer &&) noexcept(
        std::is_nothrow_move_assignable_v<Invocable>) = default;
    constexpr ~tokenizer() noexcept(
        std::is_nothrow_destructible_v<Invocable>) = default; // TODO

    constexpr void operator()(wchar_t);

    constexpr void flush_buf() noexcept(
        std::is_nothrow_invocable_r_v<void, Invocable, std::wstring &>);

    constexpr const Invocable &invocable() const noexcept;
    constexpr Invocable &invocable() noexcept;

    constexpr void reserve(std::size_t);

private:
    static_assert(std::is_invocable_r_v<void, Invocable, std::wstring &>,
        "Invocable must have signature void(wstring &)"
    );

    enum class token_type : uint {
        acronym,
        alphanum,
        apostrophe,
        company,
        host,
        num
    };

    std::wstring buffer_{};
    token_type type_{};
    Invocable invocable_{};
};

template<typename Invocable>
constexpr tokenizer<Invocable>::tokenizer(
    const Invocable &invocable
) noexcept(
    std::is_nothrow_copy_constructible_v<Invocable>
) : invocable_(invocable) {}

template<typename Invocable>
constexpr void tokenizer<Invocable>::operator()(const wchar_t value) {
    // iswspace(value), iswpunct(value)
    if (iswalnum(value))
        buffer_.push_back(value);
    else if (!buffer_.empty()) {
        invocable_(buffer_);
        buffer_.clear();
    }
    if (buffer_.is_empty()) {
        if (iswalnum(value)) {
            buffer_.push_back(value);
            type_ = token_type::alphanum;
            if (iswdigit(value))
                type_ = token_type::num;
        }
        return;
    }

    switch (type_) {
        case token_type::acronym:
            break;
        case token_type::alphanum:
            if (value == ',')
                type_ = token_type::num;
            else if (value == '.')
                type_ = token_type::host;
            break;
        case token_type::apostrophe:
            break;
        case token_type::company:
            break;
        case token_type::host:
            break;
        case token_type::num:
            if (value == ',' || value == '.') {
                if (buffer_.back() == ',' || buffer_.back() == '.') {
                    buffer_.pop_back();
                    assert(!buffer_.empty() && iswdigit(buffer_.back()));
                    return flush_buf();
                }
                assert(iswdigit(buffer_.back()));
                buffer_.push_back(value);
            } else if () {
            }
            break;
        default: assert(false);
    }
}

template<typename Invocable>
constexpr void tokenizer<Invocable>::flush_buf() noexcept(
    std::is_nothrow_invocable_r_v<void, Invocable, std::wstring &>
) {
    if (buffer_.empty())
        return;
    invocable_(buffer_);
    buffer_.clear();
}

template<typename Invocable>
constexpr const Invocable &tokenizer<Invocable>::invocable() const noexcept {
    return invocable_;
}

template<typename Invocable>
constexpr Invocable &tokenizer<Invocable>::invocable() noexcept {
    return invocable_;
}

template<typename Invocable>
constexpr void tokenizer<Invocable>::reserve(
    const std::size_t capacity
) {
    buffer_.reserve(capacity);
}

#endif
