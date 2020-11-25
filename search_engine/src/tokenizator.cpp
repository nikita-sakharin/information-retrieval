#include <cctype>

#include <search_engine/header.hpp>

wchar_t wchar_to_char(const char ch) noexcept {
    return std::tolower(ch);
}
