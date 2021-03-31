#include <cassert> // assert
#include <clocale> // LC_ALL, setlocale
#include <cstddef> // size_t

#include <stdexcept> // logic_error, runtime_error
#include <string_view> // string_view

#include <search_engine/index.hpp>
#include <search_engine/indexer.hpp>
#include <search_engine/memmap.hpp>

index make_index(const char * const texts_file) {
    using std::logic_error, std::runtime_error, std::setlocale, std::size_t,
        std::string_view;
    static constexpr const char
        *invalid = "make_index: invalid JSON",
        *empty = "make_index: empty string";

    if (setlocale(LC_ALL, "en_US.utf8") == nullptr) [[unlikely]]
        throw runtime_error("make_index: unable to set locale");

    const memmap map(texts_file);
    const char *first = map.data();
    const char * const last = first + map.size();
    if (first == last) [[unlikely]]
        throw logic_error("make_index: file is empty");
    else if (*first != '{') [[unlikely]] throw logic_error(invalid);
    ++first;

    index returns;
    while (first < last && *first != '}') {
        first = str_parser::parse(first, last, );
        if (first == last || *first != ':') [[unlikely]]
            throw logic_error(invalid);
        else if (buf.empty()) [[unlikely]] throw logic_error(empty);
        const index::doc_id id = returns.insert_document(buf);
        ++first;

        first = parse_string(first, last, buf); // TODO
        // lambda insert to index
        if (first == last || *first != ',') [[unlikely]]
            throw logic_error(invalid);
        else if (buf.empty()) [[unlikely]] throw logic_error(empty);
        ++first;
    }
    assert(first <= last);
    if (first == last) [[unlikely]] throw logic_error(invalid);
    assert(*first == '}');

    return returns;
}
