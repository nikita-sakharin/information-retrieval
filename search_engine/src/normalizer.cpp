#include <clocale>

#include <search_engine/header.hpp>
#include <search_engine/normalizer.hpp>

using std::setlocale;

template<bool Diacritic>
char *normalizer::normalize(
    const char *first,
    const char * const last,
    char * const result
) noexcept {
    // assert(*last == '\0');
    assert(first != nullptr && first < last && result != nullptr);

    const char * const name = setlocale(LC_ALL, "en_US.utf8");
    assert(name != nullptr);

    static constexpr size_t SIZE = 1ULL << 16;
    static wchar_t buffer[SIZE];
    const size_t size = last - first;
    assert(size <= SIZE);
    mbstate_t mbstate;

    const size_t converted = mbsrtowcs(buffer, &first, size, &mbstate);
    for ()
        ;

    return result + size;
}
