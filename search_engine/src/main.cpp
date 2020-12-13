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

/*
/cygdrive/c/Users/nikit/Desktop/Nikita/Learning/IR/webcrawler/texts.json
*/

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

    return 0;
}
