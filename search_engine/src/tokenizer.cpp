#include <cctype>

#include <search_engine/tokenizer.hpp>

using std::size_t, std::span, std::string_view;

explicit tokenizator::tokenizator(string_view view) noexcept : view_(view) {}

pair<size_t, string_type> tokenizer::get_string(std::span<char> buffer) {
    if (view_.empty()) [[unlikely]]
        return ;
    if (view_.empty()) [[unlikely]]
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

static const char *tokenizator::parse_escape(
    const string_view view,
    const span<char, > buf
) {
    static constexpr const char *what =
        "tokenizator::parse_escape: invalid escape sequence";
    assert(!view.empty());

    switch (*view.data()) {
        [[likely]]   case '\"': buf.push_back('\"'); return first + 1;
        [[likely]]   case '\\': buf.push_back('\\'); return first + 1;
        [[unlikely]] case 'b':  buf.push_back('\b'); return first + 1;
        [[likely]]   case 'f':  buf.push_back('\f'); return first + 1;
        [[likely]]   case 'n':  buf.push_back('\n'); return first + 1;
        [[likely]]   case 'r':  buf.push_back('\r'); return first + 1;
        [[likely]]   case 't':  buf.push_back('\t'); return first + 1;
        [[unlikely]] case 'u': {
            if (view.size() ) [[unlikely]] throw logic_error(what);
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

// если в тексте есть тире
// инженер-механик co-education
// или точки
// C.A.T. U.S.A.
// то они обрабатываются в токенизаторе, а не нормализаторе
