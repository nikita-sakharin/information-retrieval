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

/* classic:
[A-Za-Z0-9]([,\.][A-Za-Z0-9])*
Ограничиться грамматиками для которых не требуется конечный автомат
hyphen_minus
*/

template<typename Invocable>
constexpr void tokenizer<Invocable>::operator()(const wchar_t value) {
    if (iswalnum(value)) {
        if (buffer_.empty()) return buffer_.push_back(value);
        const wchar_t last = buffer_.back();
        if (last != ',' && last != '.') {
            assert(iswalnum(last));
            return buffer_.push_back(value);
        }

        assert(buffer_.size() >= 2);
        if (const wchar_t before_last = buffer_[buffer_.size() - 2];
            (last == ',' && iswdigit(before_last) && iswdigit(value)) ||
            (last == '.' &&
                (iswalpha(before_last) && iswalpha(value)) ||
                (iswdigit(before_last) && iswdigit(value))
            )
        ) return buffer_.push_back(value);
        flush_buf();
    } else if (buffer_.empty()) return;

    const wchar_t last = buffer_.back();
    assert(last == ',' || last == '.' || iswalnum(last));
    if (value == ',') {
        if (iswdigit(last))
            buffer_.push_back(value);
        else
            flush_buf();
    } else if (value == '.') {
        if (iswalnum(last))
            buffer_.push_back(value);
        else
            flush_buf();
    }
    // iswalnum(value), iswalpha(value), iswdigit(value)
    // iswpunct(value), iswspace(value)
}

template<typename Invocable>
constexpr void tokenizer<Invocable>::flush_buf() noexcept(
    std::is_nothrow_invocable_r_v<void, Invocable, std::wstring &>
) {
    if (buffer_.empty())
        return;
    if (buffer_.back() == ',' || buffer_.back() == '.')
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
