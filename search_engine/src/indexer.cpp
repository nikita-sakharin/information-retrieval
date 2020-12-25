#include <search_engine/indexer.hpp>
#include <search_engine/memmap.hpp>

index indexer::make_index(
    const char * const texts_file
) {

    const memmap map(texts_file);
    return index();
}
