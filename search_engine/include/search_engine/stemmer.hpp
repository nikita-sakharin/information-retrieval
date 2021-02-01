#ifndef SEARCH_ENGINE_STEMMER_HPP
#define SEARCH_ENGINE_STEMMER_HPP

#include <algorithm> // is_sorted

class stemmer final {
public:
    constexpr string &stem(string &) const noexcept;
private:
    std::array<endings, > = {
        "ое",
        "ые",
        "ыми",
        "ой",
        "ый",
        "ом",
        "ым",
        "ого",
        "ому",
        "ых",
        "ою",
        "ую",
        "ая"
    }
    static_assert(std::is_sorted(endings.cbegin(), endings.cend()),
        "endings must be sorted");
};

constexpr wstring &stemmer::stem(std::wstring &token) noexcept {
    return token;
}

#endif
