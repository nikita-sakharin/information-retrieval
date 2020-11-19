#include <cassert>
#include <cctype>
#include <cmath>
#include <cstddef>
#include <cstdlib>
#include <cstring>
#include <cuchar>
#include <cwchar>
#include <cwctype>

#include <fstream>
#include <iomanip>
#include <ios>
#include <iosfwd>
#include <iostream>
#include <istream>
#include <ostream>
#include <streambuf>
#include <string>
/*
#include "../algorithm.hpp"
#include "../header.hpp"
#include "../set.hpp"
*/
int main()
{
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
}
