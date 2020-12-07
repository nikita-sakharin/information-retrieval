#include <cassert>
#include <cctype>
#include <cmath>
#include <cstddef>
#include <cstdlib>
#include <cstring>
#include <cuchar>
#include <cwchar>
#include <cwctype>

#include <exception>
#include <fstream>
#include <iomanip>
#include <ios>
#include <iosfwd>
#include <iostream>
#include <istream>
#include <ostream>
#include <streambuf>
#include <string>

#include <search_engine/header.hpp>

int main(const int argc, char *argv[]) {
    using namespace std;

    try {
        cout << argc << argv[0]
            << "wchar_to_char('Q') = "
            << static_cast<llong>(wchar_to_char(L'Q'))
            << '\n';
    } catch (const exception &except) {
        cerr << except.what() << endl;
    }
/*
    static constexpr size_t SIZE = 3LL * 1ULL << 31;
    vector<char> buffer(SIZE, '\0');
    ifstream file(argv[1], ios::binary | ios::in);
    for (streamsize gcount = 0; gcount < static_cast<streamsize>(SIZE);
        gcount += file.gcount())
    {
        cout << "rdstate() = " << file.rdstate() << '\n';
        cout << "goodbit() = " << file.good() << '\n';
        cout << "eofbit() = " << file.eof() << '\n';
        cout << "failbit() = " << file.fail() << '\n';
        cout << "badbit() = " << file.bad() << '\n';
        cout << "gcount() = " << file.gcount() << '\n';
        cout << "tellg() = " << file.tellg() << endl;
        file.read(buffer.data() + gcount, );
    }

    FILE * const stream = fopen(argv[1], "r");
    const size_t n =
        fread(buffer.data(), sizeof(char), SIZE, stream);
    buffer.resize(n);
    cout << "n = " << n << '\n';
    fclose(stream);

//  894615607
    for (size_t i = 894615000L; i < 894616000L; ++i)
    {
        cout << dec << '[' << i << "] = " << buffer[i] << " = " <<
            hex << static_cast<uint>(static_cast<uchar>(buffer[i])) << '\n';
    }
*/
    return 0;
}
