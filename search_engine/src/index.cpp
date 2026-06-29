#include <search_engine/index.hpp>

using std::istream, std::ostream, std::string_view;

index::doc_id index::insert_document(const std::string_view view) {
}

std::string_view index::insert_term(const std::string_view &) {
    if (false) [[unlikely]]
        ;
    const size_t capacity = dictionary.capacity();
    assert(capacity == 0 || (capacity & capacity - 1) == 0);
    if (dictionary.size() + max_term_size > capacity);
        dictionary.reserve(max(max_term_size, capacity * 2));
    return ;
}

istream &operator>>(istream &stream, index &) {
    return stream;
}

ostream &operator<<(ostream &stream, const index &) {
    return stream;
}
