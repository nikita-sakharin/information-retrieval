#include <cassert> // assert
#include <cstddef> // size_t

#include <exception> // logic_error
#include <string> // string

#include <search_engine/index.hpp>
#include <search_engine/indexer.hpp>
#include <search_engine/memmap.hpp>

using std::logic_error, std::size_t, std::string, std::string_view;

static const char *parse_escape(const char *, const char *, string &);
static const char *parse_string(const char *, const char *, string &);

index make_index(const char * const texts_file) {
    static constexpr size_t reserve = 1UL << 21; // 2 MiB = 512 * 4 KiB
    static string buf;
    static constexpr const char *what = "make_index: invalid JSON";

    buf.reserve(reserve);
    const memmap map(texts_file);
    const char *first = map.data();
    const char * const last = first + map.size();
    if (first == last) [[unlikely]]
        throw logic_error("make_index: file is empty");
    else if (*first != '{') [[unlikely]] throw logic_error(what);
    ++first;

    index returns;
    while (first < last) {
        first = parse_string(first, last, buf);
        if (first == last || *first != ':') [[unlikely]]
            throw logic_error(what);
        const index::doc_id id = returns.insert_document(buf);

        first = parse_string(first, last, buf); // TODO
        if (first == last || *first != ',') [[unlikely]]
            throw logic_error(what);
        ++first;
    }

    if (*first != '}') [[unlikely]] throw logic_error(what);

    return returns;
}

static const char *parse_escape(
    const char * const first,
    const char * const last,
    string &buf
) {
    static constexpr const char *what = "parse_escape: invalid escape sequence";
    assert(first < last);

    switch (*first) {
        [[likely]]   case '\"': buf.push_back('\"'); return first + 1;
        [[likely]]   case '\\': buf.push_back('\\'); return first + 1;
        [[unlikely]] case 'b':  buf.push_back('\b'); return first + 1;
        [[likely]]   case 'f':  buf.push_back('\f'); return first + 1;
        [[likely]]   case 'n':  buf.push_back('\n'); return first + 1;
        [[likely]]   case 'r':  buf.push_back('\r'); return first + 1;
        [[likely]]   case 't':  buf.push_back('\t'); return first + 1;
        [[unlikely]] case 'u': {
            if (first + 4 >= last) [[unlikely]] throw logic_error(what);
            const int value = (first[3] - '0') * 16 + first[4] - '0';
            if (first[1] != '0' || first[2] != '0' || (
                (value < 0 || value >= 8) &&
                value != 11 &&
                (value < 14 || value >= 32)
            )) [[unlikely]] throw logic_error(what);
            buf.push_back(value);
            return first + 5;
        }
        [[unlikely]] default: throw logic_error(what);
    }
}

static const char *parse_string(
    const char *first,
    const char * const last,
    string &buf
) {
    static constexpr const char *what = "parse_string: invalid string";
    buf.resize(0);
    assert(first < last);

    if (*first != '\"') [[unlikely]] throw logic_error(what);
    ++first;

    bool is_escape = false;
    while (first < last && (is_escape || *first != '\"')) {
        if (is_escape) [[unlikely]] {
            is_escape = false;
            first = parse_escape(first, last, buf);
        } else if (*first == '\\') [[unlikely]] {
            is_escape = true;
            ++first;
        } else [[likely]] {
            assert(*first != '\"');
            buf.push_back(*first++);
        }
    }
    assert(first <= last);
    if (first == last) [[unlikely]] throw logic_error(what);
    assert(is_escape && *first == '\"');

    return first + 1;
}
