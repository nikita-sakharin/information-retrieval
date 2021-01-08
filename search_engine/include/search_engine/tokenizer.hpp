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

    constexpr void reset();

private:
    constexpr std::size_t get_end_index() noexcept;

    std::span<char> buffer;
    std::size_t index = 0, position = 0;
    std::function<void(std::pair<std::size_t, std::span<char>>)> func;
}

constexpr void tokenizator::operator()(const char value) noexcept {
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

constexpr std::size_t tokenizator::get_end_index() noexcept {
    if ()
        return;

    return std::numeric_limits<std::size_t>::max();
}

#endif
