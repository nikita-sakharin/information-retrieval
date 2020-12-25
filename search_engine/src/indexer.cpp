#include <fstream> // ofstream
#include <iostream> // ios_base

#include <search_engine/index.hpp>
#include <search_engine/indexer.hpp>
#include <search_engine/memmap.hpp>

void indexer::make_index(
    const char * const texts_file,
    const char * const index_file
) {
    using std::ofstream, std::ios_base;

    const memmap map(texts_file);
    index text_index;

    ofstream stream(
        index_file,
        ios_base::binary | ios_base::out | ios_base::trunc
    );
    stream << text_index;
}
