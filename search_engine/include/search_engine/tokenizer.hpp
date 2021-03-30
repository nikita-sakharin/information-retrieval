#ifndef SEARCH_ENGINE_TOKENIZER_HPP
#define SEARCH_ENGINE_TOKENIZER_HPP

#include <cassert> // assert
#include <cstddef> // size_t
#include <cwctype> // iswalnum, iswalpha, iswdigit

#include <string> // wstring
#include <type_traits> // is_invocable_r_v, is_nothrow_*_v

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
        std::is_nothrow_destructible_v<Invocable>) = default;

    constexpr void operator()(wchar_t);

    constexpr void clear_buffer() noexcept;

    constexpr void flush_buffer() noexcept(
        std::is_nothrow_invocable_r_v<void, Invocable, std::wstring &>);

    constexpr const Invocable &invocable() const noexcept;
    constexpr Invocable &invocable() noexcept;

    constexpr void reserve(std::size_t);

private:
    static_assert(__STDC_ISO_10646__ >= 201103L,
        "Unicode version 2011 or later required"
    );
    static_assert(std::is_invocable_r_v<void, Invocable, std::wstring &>,
        "Invocable must have signature void(wstring &)"
    );

    std::wstring buffer_{};
    Invocable invocable_{};
};

template<typename Invocable>
constexpr tokenizer<Invocable>::tokenizer(const Invocable &invocable) noexcept(
    std::is_nothrow_copy_constructible_v<Invocable>
) : invocable_(invocable) {}

template<typename Invocable>
constexpr void tokenizer<Invocable>::operator()(const wchar_t value) {
    using std::iswalnum, std::iswalpha, std::iswdigit;

    const bool is_value_alnum = static_cast<bool>(iswalnum(value));
    if (buffer_.empty()) {
        if (is_value_alnum)
            buffer_.push_back(value);
        return;
    }

    const wchar_t last = buffer_.back();
    assert(last == '\'' || last == ',' || last == '.' || iswalnum(last));
    if (is_value_alnum) {
        if (last != '\'' && last != ',' && last != '.') {
            buffer_.push_back(value);
            return;
        }

        assert(buffer_.size() >= 2U);
        const wchar_t before_last = buffer_[buffer_.size() - 2U];
        assert(iswalnum(before_last));
        if (const bool is_value_alpha = static_cast<bool>(iswalpha(value)),
            is_before_last_alpha = static_cast<bool>(iswalpha(before_last));
            (last == '\'' && is_value_alpha && is_before_last_alpha) ||
            (last == ',' && !is_value_alpha && !is_before_last_alpha) ||
            (last == '.' && (
                (is_value_alpha && is_before_last_alpha) ||
                (!is_value_alpha && !is_before_last_alpha)
            ))
        ) {
            buffer_.push_back(value);
            return;
        }
        flush_buffer();
        return;
    }

    if (const bool is_last_alpha = static_cast<bool>(iswalpha(last)),
        is_last_digit = static_cast<bool>(iswdigit(last));
        (value == '\'' && is_last_alpha) ||
        (value == ',' && is_last_digit) ||
        (value == '.' && (is_last_alpha || is_last_digit))
    ) {
        buffer_.push_back(value);
        return;
    }
    flush_buffer();
}

template<typename Invocable>
constexpr void tokenizer<Invocable>::clear_buffer() noexcept {
    buffer_.clear();
}

template<typename Invocable>
constexpr void tokenizer<Invocable>::flush_buffer() noexcept(
    std::is_nothrow_invocable_r_v<void, Invocable, std::wstring &>
) {
    if (buffer_.empty())
        return;
    const wchar_t last = buffer_.back();
    if (last == '\'' ||  last == ',' || last == '.')
        buffer_.pop_back();
    assert(!buffer_.empty() && iswalnum(buffer_.back()));
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
