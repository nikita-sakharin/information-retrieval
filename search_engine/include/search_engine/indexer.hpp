#ifndef SEARCH_ENGINE_INDEXER_HPP
#define SEARCH_ENGINE_INDEXER_HPP

#include <search_engine/index.hpp>

template<bool StopWords = false, bool Stem = false>
index make_index(const char *);

#endif
