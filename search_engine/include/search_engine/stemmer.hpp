#ifndef SEARCH_ENGINE_STEMMER_HPP
#define SEARCH_ENGINE_STEMMER_HPP

class stemmer final {
public:
    static inline constexpr char *stem(char *) noexcept;
    static char *tolower(char *, const char *, size_t) noexcept;
};

#endif
