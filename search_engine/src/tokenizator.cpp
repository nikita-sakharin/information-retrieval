#include <cctype>

#include <search_engine/header.hpp>
#include <search_engine/tokenizator.hpp>

class tokenizator {
public:
    constexpr tokenizator() noexcept = delete;
    constexpr tokenizator(const tokenizator &) noexcept = delete;
    constexpr tokenizator(tokenizator &&) noexcept = delete;
    constexpr tokenizator &operator =(const tokenizator &) noexcept = delete;
    constexpr tokenizator &operator =(tokenizator &&) noexcept = delete;
    constexpr ~tokenizator() noexcept = delete;

    static void index_file(const char * const filename, index_ctor &index) {
        const memmap map(filename);
        const char * const data = map.data(), data_end = data + map.size();
        for (const char *ptr = data; ptr < data_end; ) {
            ;
        }
        // если в тексте есть тире
        // инженер-механик co-education
        // или точки
        // C.A.T. U.S.A.
        // то они обрабатываются в токенизаторе, а не нормализаторе
    }
}

wchar_t wchar_to_char(const char ch) noexcept {
    return std::tolower(ch);
}
