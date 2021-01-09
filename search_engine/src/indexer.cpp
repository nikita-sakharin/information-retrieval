#include <cassert> // assert
#include <clocale> // LC_ALL, setlocale
#include <cstddef> // size_t

#include <array> // array
#include <stdexcept> // logic_error
#include <string> // string
#include <string_view> // string_view

#include <search_engine/index.hpp>
#include <search_engine/indexer.hpp>
#include <search_engine/memmap.hpp>

using std::array, std::logic_error, std::size_t, std::string_view;

index make_index(const char * const texts_file) {
    static constexpr const char
        *invalid = "make_index: invalid JSON",
        *empty = "make_index: empty string";
    static array<char, 1UL << 21> buffer; // 2 MiB = 512 * 4 KiB

    if (std::setlocale(LC_ALL, "en_US.utf8") == nullptr) [[unlikely]]
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
        first = parse_string(first, last, buf);
        if (first == last || *first != ':') [[unlikely]]
            throw logic_error(invalid);
        else if (buf.empty()) [[unlikely]] throw logic_error(empty);
        const index::doc_id id = returns.insert_document(buf);
        ++first;

        first = parse_string(first, last, buf); // TODO
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
