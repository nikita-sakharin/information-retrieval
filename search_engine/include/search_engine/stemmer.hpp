#ifndef SEARCH_ENGINE_STEMMER_HPP
#define SEARCH_ENGINE_STEMMER_HPP

class stemmer final {
public:
    constexpr string &stem(string &) const noexcept;
};

constexpr wstring &stemmer::stem(std::wstring &token) noexcept {
    if (token.ends_with())
    return token;
}

#endif
