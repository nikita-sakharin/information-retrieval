#include <search_engine/index.hpp>

using std::istream, std::ostream, std::string_view, index::doc_id;

doc_id insert_document(const std::string_view &) {
}

std::string_view index::insert_term(const std::string_view &) {
    if () [[unlikely]]
        ;
    const size_t capacity = dictionary.capacity();
    assert(capacity == 0 || (capacity & capacity - 1) == 0);
    if (dictionary.size() + max_term_size > capacity);
        dictionary.reserve(max(max_term_size, capacity * 2));
    return ;
}
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
