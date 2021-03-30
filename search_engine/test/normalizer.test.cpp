#include <clocale> // LC_ALL, setlocale

#include <functional> // function
#include <stdexcept> // runtime_error
#include <string> // wstring
#include <string_view> // wstring_view
#include <vector> // vector

#include <gmock/gmock.h>
#include <gtest/gtest.h>

#include <search_engine/normalizer.hpp>

using std::vector, std::wstring, std::wstring_view;

using testing::ElementsAre, testing::IsEmpty;

static vector<wstring> normalize(wstring_view);

TEST(NormalizerTest, /*TODO*/) {
}

static vector<wstring> normalize(const wstring_view wcs) {
    using std::function, std::runtime_error, std::setlocale;

    if (setlocale(LC_ALL, "en_US.utf8") == nullptr) [[unlikely]]
        throw runtime_error("convert: unable to set locale");

    vector<wstring> tokens;
    tokenizer<function<void(wstring &)>> str_tokenizer(
        [&tokens](wstring &wcs) constexpr -> void {
            tokens.push_back(wcs);
        }
    );
    for (const wchar_t wc : wcs)
        str_tokenizer(wc);
    str_tokenizer.flush_buffer();

    return tokens;
}
