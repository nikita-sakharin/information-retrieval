#ifndef SEARCH_ENGINE_NORMALIZER_HPP
#define SEARCH_ENGINE_NORMALIZER_HPP

#include <cassert>

static_assert(__STDC_ISO_10646__ >= 201103L); // Unicode revision
static_assert(sizeof(wint_t) == 4U); // TODO

class normalizer final {
public:
    static char *normalize(const char *, const char *, char *) noexcept;
};

#endif
