#include <clocale> // setlocale
#include <cwctype> // towlower

#include <system_error> // generic_category, system_error

#include <search_engine/header.hpp>
#include <search_engine/normalizer.hpp>

using std::generic_category, std::setlocale, std::system_error;

template<bool Diacritic>
char *normalizer::normalize(
    const char *first,
    const char * const last,
    char * const result
) {
    assert(
        first != nullptr &&
        first < last &&
        *last == '\0' &&
        result != nullptr
    );

    const char * const name = setlocale(LC_ALL, "en_US.utf8");
    assert(name != nullptr);

    static constexpr size_t SIZE = 1ULL << 16;
    static wchar_t buffer[SIZE];
    const size_t size = last - first;
    assert(size <= SIZE);
    mbstate_t mbstate;

    const size_t converted = mbsrtowcs(buffer, &first, size, &mbstate);
    if (converted != size) [[unlikely]]
        throw system_error(errno, generic_category(), "normalizer::normalize");

    for (size_t i = 0; i < )
        buffer[i] = towlower(buffer);

    return result + size;
}

normalizer::diacritic(
    const char *first,
    const char * const last,
    char * const result
) noexcept {
}
