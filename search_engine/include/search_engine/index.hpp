#ifndef SEARCH_ENGINE_INDEX_HPP
#define SEARCH_ENGINE_INDEX_HPP

#include <iostream>
#include <map>
#include <string_view>
#include <vector>

#include <search_engine/header.hpp>

class index final {
    std::map<std::string_view, uint> posting;
    std::vector<char> dictionary;

public:
    inline index() noexcept = default;
    inline index(const index &) noexcept = default;
    inline index(index &&) noexcept = default;
    inline index &operator=(const index &) noexcept = default;
    inline index &operator=(index &&) noexcept = default;
    inline ~index() noexcept = default;

    // void insert(uint, uint);
    friend std::istream &operator<<(std::istream &, index &);
    friend std::ostream &operator<<(std::ostream &, const index &);
};

#endif
