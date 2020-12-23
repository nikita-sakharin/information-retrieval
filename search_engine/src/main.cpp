#include <cstddef>
#include <cstdlib> // exit
#include <cstring> // strcmp

#include <exception> // exception
#include <iostream> // cerr

#include <search_engine/indexer.hpp>

int main(const int argc, char *argv[]) {
    using std::cerr, std::exception, std::exit;

    // --help?
    const char *text = nullptr, *index = nullptr;
    for (int i = 1; i < argc; ++i) {
        if (i + 1 >= argc)
            break;
        if (strcmp(argv[i], "--text"))
            text = argv[i + 1];
        else if (strcmp(argv[i], "--index"))
            index = argv[i + 1]
        else
            break;
    }

    if (text == nullptr || index == nullptr) {
        cerr << "Usage: " << argv[0] << " --text [FILE] --index [FILE]\n";
        exit(EXIT_FAILURE);
    }

    try {
        // indexer::index(argv[1], argv[2]);
    } catch (const exception &except) {
        cerr << except.what() << '\n';
        exit(EXIT_FAILURE);
    }

    return 0;
}
