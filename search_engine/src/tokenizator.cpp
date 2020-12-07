#include <cctype>

#include <sys/mman.h>

#include <search_engine/header.hpp>
// #include <search_engine/tokenizator.hpp> ???
/*
class tokenizator {
    static constexpr size_t len = ;

    const void *addr;
    off_t off;
public:
    constexpr tokenizator() noexcept = delete;
    constexpr tokenizator(const tokenizator &) noexcept = delete;
    constexpr tokenizator(tokenizator &&) noexcept = default;
    tokenizator &operator =(const tokenizator &) noexcept = default;
    tokenizator &operator =(tokenizator &&) noexcept = default;
    ~tokenizator() noexcept = default;

    tokenizator(const char *) noexcept = delete;
    constexpr tokenizator(tokenizator) noexcept = delete;
}
*/
wchar_t wchar_to_char(const char ch) noexcept {
    return std::tolower(ch);
}
