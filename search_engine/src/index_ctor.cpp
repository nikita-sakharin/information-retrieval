#include <cctype>
#include <cstring>

#include <map>

#include <search_engine/header.hpp>

class index_ctor {
    class compare {
    public:
        inline constexpr bool operator()(const char *, const char *) noexcept;
    };

    std::map<const char *, uint, compare> posting;
    vector<char> dictionary;

public:
    void insert(uint, uint);
    void write(std::ostream &);
};

inline constexpr bool index_ctor::compare::operator()(
    const char * const str1,
    const char * const str2
) noexcept { return strcmp(str1, str2); }

void index_ctor::insert(const char * const term_first, const char * const term_last,
    const uint doc_id) {
    dictionary.insert();
    ;
}
