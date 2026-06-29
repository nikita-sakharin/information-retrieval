#include <cassert> // assert
#include <clocale> // LC_ALL, setlocale
#include <cstddef> // size_t

#include <stdexcept> // logic_error, runtime_error
#include <string> // string, wstring

#include <search_engine/char_encoder.hpp>
// #include <search_engine/index.hpp>
#include <search_engine/indexer.hpp>
#include <search_engine/memmap.hpp>
#include <search_engine/normalizer.hpp>
#include <search_engine/stemmer.hpp>
#include <search_engine/str_encoder.hpp>
#include <search_engine/str_parser.hpp>
#include <search_engine/tokenizer.hpp>

template<bool StopWords = false, bool Stem = false>
const char *make_index(const char * const texts_file) {
    using std::logic_error, std::runtime_error, std::setlocale, std::size_t,
        std::string, std::wstring;
    static constexpr const char
        *invalid = "make_index: invalid JSON"; //, *empty = "make_index: empty string";

    if (setlocale(LC_ALL, "en_US.utf8") == nullptr) [[unlikely]]
        throw runtime_error("make_index: unable to set locale");

    const memmap map(texts_file);
    const char *first = map.data();
    const char * const last = first + map.size();
    if (first == last) [[unlikely]]
        throw logic_error("make_index: file is empty");
    else if (*first != '{') [[unlikely]]
        throw logic_error(invalid);
    ++first;

    // size_t pos;
    string title;
    str_parser title_parser(bind(&string::push_back, &title, std::placeholders::_1));
/*
    str_encoder<wchar_t, char>(
        [pos, &counts](const string &str) constexpr noexcept -> void {
            ++counts[std::hash<string>()(str) % counts.size()];
        }
    );
    str_parser(char_encoder(tokenizer(normalizer(
        [&pos](const size_t position, wstring &wcs) constexpr noexcept -> void {
            pos = position;
            text_stemmer(wcs);
        }
    ))));
    while (first < last && *first != '}') {
        title_parser(first, last);
        if (first == last || *first != ':') [[unlikely]]
            throw logic_error(invalid);
        else if (buf.empty()) [[unlikely]] throw logic_error(empty);
        ++first;
        // const index::doc_id id = returns.insert_document(buf);

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
*/
    return first;
}
