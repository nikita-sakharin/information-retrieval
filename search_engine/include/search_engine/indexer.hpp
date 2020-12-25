#ifndef SEARCH_ENGINE_INDEXER_HPP
#define SEARCH_ENGINE_INDEXER_HPP

#include <search_engine/index.hpp>

class indexer final {
public:
    static index make_index(const char *);
};

#endif
