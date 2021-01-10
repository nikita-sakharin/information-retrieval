#ifndef SEARCH_ENGINE_TOKENIZER_HPP
#define SEARCH_ENGINE_TOKENIZER_HPP

#include <cctype> // isalnum
#include <cstddef> // size_t

#include <span> // span
#include <type_traits> // is_invocable_r_v, is_nothrow_*_v

template<typename Invocable>
class tokenizer final {
public:
    constexpr tokenizer() noexcept(
        std::is_nothrow_default_constructible_v<Invocable>) = default;
    constexpr tokenizer(const Invocable &invocable) noexcept(
        std::is_nothrow_copy_constructible_v<Invocable>);
    constexpr tokenizer(const tokenizer &) noexcept(
        std::is_nothrow_copy_constructible_v<Invocable>) = default;
    constexpr tokenizer(tokenizer &&) noexcept(
        std::is_nothrow_move_constructible_v<Invocable>) = default;
    constexpr tokenizer &operator=(const tokenizer &) noexcept(
        std::is_nothrow_copy_assignable_v<Invocable>) = default;
    constexpr tokenizer &operator=(tokenizer &&) noexcept(
        std::is_nothrow_move_assignable_v<Invocable>) = default;
    constexpr ~tokenizer() noexcept(
        std::is_nothrow_destructible_v<Invocable>) = default;

    constexpr void operator()(wchar_t) noexcept(std::is_nothrow_invocable_r_v<
        void, Invocable, std::size_t, std::span<wchar_t>>);

    constexpr void reset();

private:
    static_assert(
        std::is_invocable_r_v<void, Invocable, std::size_t, std::span<wchar_t>>,
        "invocable must have signature void(size_t, span<wchar_t>)"
    );

    constexpr std::size_t get_end_index() noexcept;

    std::span<wchar_t> buffer;
    std::size_t index = 0, position = 0;
    Invocable invocable{};
};

template<typename Invocable>
constexpr void tokenizer::operator()(const wchar_t value) noexcept(
    std::is_nothrow_invocable_r_v<void, Invocable, std::size_t, std::span<wchar_t>>
) {
    buffer[index++] = value;
    if (const std::size_t end_index = get_end_index(); end_index <= index) {
        if (index > 1) {
            func(std::make_pair(position++, buffer.first(end_index)));
            std::move(buffer.cbegin() + , buffer.cbegin() + , buffer.begin());
        }
        index = 0;
    }
}

constexpr void reset() noexcept {
/*
    if (index)
        func(make_pair(position, buffer.first(index)));
*/
    index = 0;
    position = 0;
}

constexpr std::size_t tokenizer::get_end_index() noexcept {
    assert(index > 0);
    const wchar_t wc = buffer[index - 1];
    if (std::isalnum(wc) || wc)
        return;

    return std::numeric_limits<std::size_t>::max();
}

/*
если в тексте есть тире
инженер-механик co-education
или точки
C.A.T. U.S.A.
то они обрабатываются в токенизаторе, а не нормализаторе
*/
#endif
