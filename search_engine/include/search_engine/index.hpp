#ifndef SEARCH_ENGINE_INDEX_HPP
#define SEARCH_ENGINE_INDEX_HPP

#include <cstdint> // uint32_t

#include <iostream> // istream, ostream
#include <map> // map
#include <string_view> // string_view
#include <vector> // vector

class index final {
public:
    using doc_id = std::uint32_t;

    inline index() noexcept = default;
    inline index(const index &) noexcept = default;
    inline index(index &&) noexcept = default;
    inline index &operator=(const index &) noexcept = default;
    inline index &operator=(index &&) noexcept = default;
    inline ~index() noexcept = default;

    doc_id insert_document(std::string_view);

    // void insert_term(doc_id, std::string_view, size_t);
    friend std::istream &operator>>(std::istream &, index &);
    friend std::ostream &operator<<(std::ostream &, const index &);

private:
    std::map<std::string_view, std::vector<doc_id>> posting;
    std::vector<char> dictionary;
};

#endif
