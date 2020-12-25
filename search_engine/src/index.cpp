#include <search_engine/index.hpp>

using std::istream, std::ostream;
/*
void index::insert(
    const char * const first,
    const char * const last,
    const uint doc_id
) {
    dictionary.insert();
}
*/
istream &operator<<(istream &stream, index &) {
    return stream;
}

ostream &operator<<(ostream &stream, const index &) {
    return stream;
}
