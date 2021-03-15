#ifndef SEARCH_ENGINE_TOKENIZER_HPP
#define SEARCH_ENGINE_TOKENIZER_HPP

#include <cstddef> // size_t
#include <cwctype> // iswalnum, iswspace

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

    constexpr void reset() noexcept;

private:
    static_assert(
        std::is_invocable_r_v<void, Invocable, std::size_t, std::wstring &>,
        "invocable must have signature void(size_t, wstring &)"
    );

    std::wstring buffer_;
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
    if (iswspace(value) || /*ispunct() */) {
        if (!buffer_.empty()) {
            invocable_(position++, buffer_);
            buffer_.clear();
        }
    }

    buffer_.push_back(value);
}

template<typename Invocable>
constexpr const Invocable &tokenizer<Invocable>::invocable() const noexcept {
    return invocable_;
}

template<typename Invocable>
constexpr Invocable &tokenizer<Invocable>::invocable() noexcept {
    return invocable_;
}

constexpr void reset() noexcept {
    if (!buffer_.empty())
        invocable_(position, buffer_); // TODO
    buffer_.clear();
    position = 0;
}

constexpr std::size_t tokenizer::get_end_index() noexcept {
    assert(index > 0);
    const wchar_t wc = buffer_[index - 1];
    if (std::isalnum(wc) || wc)
        return;

    return std::numeric_limits<std::size_t>::max();
}

#endif
