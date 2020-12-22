#include <cassert>
#include <cstddef>
#include <cstdlib>
#include <cstring>

#include <codecvt>
#include <exception>
#include <iostream>
#include <locale>
#include <string>

#include <search_engine/header.hpp>
#include <search_engine/memmap.hpp>
/*
/cygdrive/c/Users/nikit/Desktop/Nikita/Learning/IR/webcrawler/texts.json
*/
/*
    using namespace std;
    const uint i = 7 * 256 * 256 + 3 * 256 + 10;
    const uchar * const ptr = reinterpret_cast<const uchar *>(&i);
    cout << int(ptr[0]) << ' ' << int(ptr[1]) << ' ' << int(ptr[2]) << '\n';
    const string str = "\u0449";
    cout << str.size() << '\n';

    wstring_convert<codecvt_utf8<char32_t>, char32_t> cvt;
    u32string utf32 = cvt.from_bytes(str);
    cout << utf32.size() << '\n';
    cout << uint(utf32[0]) << '\n';
*/
using namespace std;

int main() {
    constexpr const char *name = "en_US.UTF-8";
    assert(setlocale(LC_ALL, name) != nullptr);
/*
    for (wint_t c = 0; c < 0X110000; ++c) {
        const wint_t lower = towlower(c);
        assert(lower != WEOF);
        if (c != lower)
            cout << c << " != " << lower << '\n';
    }
    cout << "SUCCESS" << endl;
*/
    memmap m("/cygdrive/c/Users/nikit/Desktop/Nikita/Learning/IR/webcrawler/texts.json");
    const uchar * const data = reinterpret_cast<const uchar *>(m.data());
    const size_t size = m.size();
    size_t total = 0;
    for (size_t i = 0; i < size; ++i) {
        if ((i & 0X1FFFFF) == 0)
            cout << i << '\n';
        ptrdiff_t count = 0;
        if (data[i] >= 240 && data[i] < 248)
            count = 3;
        else if (data[i] >= 224 && data[i] < 240)
            count = 2;
        else if (data[i] >= 192 && data[i] < 224)
            count = 1;
        else if (data[i] >= 128) {
            cerr << "data[" << i << "] = " << static_cast<int>(data[i]) << '\n';
            continue;
        } else if (data[i] < 128)
            continue;
        ++i;
        if (count == 3)
            cerr << "count = 3 for i = " << i << '\n';
        for (ptrdiff_t j = 0; j < count; ++j)
            if (data[i + j] < 128)
                cerr << "data[" << i << "] = " << static_cast<int>(data[i]) << '\n';
        i += count - 1;
    }
    cout << total << '\n';

    return 0;
}
