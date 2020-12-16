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
#include <string>

#include <search_engine/header.hpp>

/*
/cygdrive/c/Users/nikit/Desktop/Nikita/Learning/IR/webcrawler/texts.json
*/
int main(const int argc, char *argv[]) {
    using namespace std;
    static_cast<void>(argc);
    static_cast<void>(argv);

    try {
        cout << argc << argv[0];
    } catch (const exception &except) {
        cerr << except.what() << endl;
    }

    return 0;
}
