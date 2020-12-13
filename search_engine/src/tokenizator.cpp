#include <cctype>

#include <fcntl.h>
#include <sys/mman.h>
#include <sys/stat.h>

#include <search_engine/header.hpp>
// #include <search_engine/tokenizator.hpp> ???

class tokenizator {
public:
    constexpr tokenizator() noexcept = delete;
    constexpr tokenizator(const tokenizator &) noexcept = delete;
    constexpr tokenizator(tokenizator &&) noexcept = delete;
    tokenizator &operator =(const tokenizator &) noexcept = delete;
    tokenizator &operator =(tokenizator &&) noexcept = delete;
    ~tokenizator() noexcept = delete;

    static void index_file(const char * const filename, index_ctor &index) {

        if (const int errnum = close(fildes); errnum != 0)
            throw system_error(errnum);
    }
}

wchar_t wchar_to_char(const char ch) noexcept {
    return std::tolower(ch);
}
