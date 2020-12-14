#ifndef SEARCH_ENGINE_STEMMER_HPP
#define SEARCH_ENGINE_STEMMER_HPP

class stemmer final {
public:
    static inline constexpr char *stem(char *) noexcept;
    static inline constexpr char *tolower(char *, char *) noexcept;
};

#endif
