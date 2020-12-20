#include <cassert>
#include <cstddef>
#include <cstdlib>
#include <cstring>

#include <exception>
#include <iostream>
#include <string>

#include <search_engine/header.hpp>
#include <search_engine/memmap.hpp>

/*
/cygdrive/c/Users/nikit/Desktop/Nikita/Learning/IR/webcrawler/texts.json
*/
int main(const int argc, char *argv[]) {
    using namespace std;

    if (argc != 2) {
        printf("argc != 2\n");
        std::exit(EXIT_FAILURE);
    }

    try {
        memmap p;
        p.open(argv[1]);
        memmap q(move(p));
        p = move(q);
        q.open(argv[1]);
        if (q.is_open())
            q.close();
        const char * const data = p.data();
        const size_t size = p.size();
        cout << "size = " << size << '\n';

        for (size_t i = 0; i < size; ++i)
            cout << data[i];
    } catch (const exception &except) {
        cerr << except.what() << endl;
    }

    return 0;
}
