#ifndef SEARCH_ENGINE_TOKENIZER_HPP
#define SEARCH_ENGINE_TOKENIZER_HPP

#include <cstddef> // size_t

#include <span> // span
#include <utility> // pair

class tokenizator final {
public:
    constexpr tokenizator() noexcept = default;
    constexpr tokenizator(const tokenizator &) noexcept = default;
    constexpr tokenizator(tokenizator &&) noexcept = default;
    constexpr tokenizator &operator=(const tokenizator &) noexcept = default;
    constexpr tokenizator &operator=(tokenizator &&) noexcept = default;
    constexpr ~tokenizator() noexcept = default;

    constexpr void operator()(char) noexcept;

private:
    static constexpr void char_mapping(wchar_t) noexcept;

    constexpr std::size_t get_end_index() noexcept; // is_token_end

    std::span<char> buffer;
    std::size_t index;
    const std::function<void(std::pair<std::size_t, std::span<char>>)> &func;
}

constexpr void tokenizator::operator()(const char value) noexcept {
    buffer[index++] = value;
    if (const std::size_t end_index = get_end_index(); end_index < index) { // TODO
        if (index > 1)
            func(make_pair(0 /* TODO */, buffer.first(index - 1)));
        index = 0;
    }
}

constexpr std::size_t tokenizator::get_end_index() noexcept {
    return ;
}

#endif
