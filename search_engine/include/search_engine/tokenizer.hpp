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
    constexpr tokenizer();
    constexpr tokenizer(const Invocable &);
    constexpr tokenizer(const tokenizer &) = default;
    constexpr tokenizer(tokenizer &&) noexcept(
        std::is_nothrow_move_constructible_v<Invocable>) = default;
    constexpr tokenizer &operator=(const tokenizer &) = default;
    constexpr tokenizer &operator=(tokenizer &&) noexcept(
        std::is_nothrow_move_assignable_v<Invocable>) = default;
    constexpr ~tokenizer() noexcept(
        std::is_nothrow_destructible_v<Invocable>) = default;

    constexpr void operator()(wchar_t) noexcept(std::is_nothrow_invocable_r_v<
        void, Invocable, std::size_t, std::wstring &>);

    constexpr const Invocable &invocable() const noexcept;
    constexpr Invocable &invocable() noexcept;

    constexpr void reserve(std::size_t);

    constexpr void reset() noexcept(std::is_nothrow_invocable_r_v<
        void, Invocable, std::size_t, std::wstring &>);

private:
    static_assert(
        std::is_invocable_r_v<void, Invocable, std::size_t, std::wstring &>,
        "Invocable must have signature void(size_t, wstring &)"
    );

    std::wstring buffer_{};
    std::size_t position_ = 0;
    Invocable invocable_{};
};

template<typename Invocable>
constexpr tokenizer<Invocable>::tokenizer(
    const Invocable &invocable
) : invocable_(invocable) {}

template<typename Invocable>
constexpr void tokenizer<Invocable>::operator()(const wchar_t value) noexcept(
    std::is_nothrow_invocable_r_v<void, Invocable, std::size_t, std::wstring &>
) {
    // iswspace(value), iswpunct(value)
    if (iswalnum(value))
        buffer_.push_back(value);
    else if (!buffer_.empty()) {
        invocable_(position_++, buffer_);
        buffer_.clear();
    }
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
) noexcept(
    std::is_nothrow_invocable_r_v<void, Invocable, std::size_t, std::wstring &>
) {
    buffer_.reserve(capacity);
}

template<typename Invocable>
constexpr void tokenizer<Invocable>::reset() noexcept {
    if (!buffer_.empty())
        invocable_(position_, buffer_); // TODO
    buffer_.clear();
    position_ = 0;
}

#endif
