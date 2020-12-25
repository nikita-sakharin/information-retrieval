#ifndef SEARCH_ENGINE_TOKENIZATOR_HPP
#define SEARCH_ENGINE_TOKENIZATOR_HPP

#include <search_engine/memmap.hpp>

class tokenizator final {
public:
    static void split(const memmap &, index &);
    // если в тексте есть тире
    // инженер-механик co-education
    // или точки
    // C.A.T. U.S.A.
    // то они обрабатываются в токенизаторе, а не нормализаторе
}

#endif
