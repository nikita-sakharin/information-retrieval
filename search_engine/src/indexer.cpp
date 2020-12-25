#include <fstream>
#include <iostream>

#include <search_engine/index.hpp>
#include <search_engine/indexer.hpp>
#include <search_engine/memmap.hpp>

void indexer::make_index(const char * const text, const char * const file) {
    using std::ofstream, std::ios_base;

    const memmap map(text);
    index text_index;
    ofstream stream(file, ios_base::binary | ios_base::out | ios_base::trunc);
    stream << text_index;
}
