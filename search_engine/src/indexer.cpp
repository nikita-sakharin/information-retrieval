#include <cstddef> // size_t

#include <search_engine/index.hpp>
#include <search_engine/indexer.hpp>
#include <search_engine/memmap.hpp>

using std::size_t, std::string_view, index::doc_id;

static const char *parse_escape(const char *, const char *, char *);
static const char *parse_title(const char *, const char *, char *, size_t);

index make_index(const char * const texts_file) {
    static constexpr size_t max_size = 1UL << 21; // 2 MiB
    static char buffer[max_size];
    static constexpr const char *what = "make_index: invalid JSON";

    const memmap map(texts_file);
    const char *data = map.data();
    const char * const data_end = data + map.size();
    if (data == data_end) [[unlikely]]
        throw logic_error("make_index: file is empty");
    else if (*data != '{') [[unlikely]] throw logic_error(what);
    ++data;

    index returns;
    while (data < data_end) {
        data = parse_title(data, data_end, buffer, max_size);
        if (*data != ':') [[unlikely]] throw logic_error(what);
        const doc_id id = index.insert_document(string_view(buffer));
        if (*data != ',') [[unlikely]] throw logic_error(what);
        ++data;
    }

    if (*data != '}') [[unlikely]] throw logic_error(what);

    return returns;
}

static const char *parse_escape(
    const char * const data,
    const char * const data_end,
    char * const buf
) {
    static constexpr const char *what = "parse_escape: invalid escape sequence";
    switch (*data) {
        [[likely]]   case '\"': *buf = '\"'; return data + 1;
        [[likely]]   case '\\': *buf = '\\'; return data + 1;
        [[unlikely]] case 'b':  *buf = '\b'; return data + 1;
        [[likely]]   case 'f':  *buf = '\f'; return data + 1;
        [[likely]]   case 'n':  *buf = '\n'; return data + 1;
        [[likely]]   case 'r':  *buf = '\r'; return data + 1;
        [[likely]]   case 't':  *buf = '\t'; return data + 1;
        [[unlikely]] case 'u':
            if (data + 4 >= data_end) [[unlikely]] throw logic_error(what);
            const int value = (data[3] - '0') * 16 + data[4] - '0';
            if (data[1] != '0' || data[2] != '0' || (
                (value < 0 || value >= 8) &&
                value != 11 &&
                (value < 14 || value >= 32)
            )) [[unlikely]] throw logic_error(what);
            *buf = value;
            return data + 5;
        [[unlikely]] default: throw logic_error(what);
    }
}

static const char *parse_title(
    const char *data,
    const char * const data_end,
    char *buf,
    const size_t size
) {
    static constexpr const char *what = "parse_title: invalid JSON";

    if (*data != '\"') [[unlikely]] throw logic_error(what);
    ++data;

    bool is_escape = false;
    while (data < data_end && (is_escape || *data != '\"')) {
        // TODO check size: buffer overflow
        if (is_escape) [[unlikely]] {
            is_escape = false;
            data = parse_escape(data, data_end, buf);
        } else if (*data == '\\') [[unlikely]] {
            is_escape = true;
            ++data;
        } else [[likely]]
            *buf++ = *data++;
    }
    if (data == data_end) // TODO

    if () [[unlikely]] // TODO
        throw logic_error("make_index: JSON is valid but title is empty");
    *buf = '\0';
}
