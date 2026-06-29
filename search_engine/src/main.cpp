#include <cassert> // assert
#include <cstdlib> // EXIT_FAILURE, EXIT_SUCCESS, exit
#include <cstring> // strcmp

#include <exception> // exception
#include <fstream> // ofstream
#include <iostream> // cerr, cin, cout, ios_base

#include <unistd.h> // getopt

#include <search_engine/indexer.hpp>

int main(const int argc, char ** const argv) {
    using std::cerr, std::cin, std::cout, std::exception, std::exit,
        std::ios_base, std::ofstream, std::strcmp;
    ios_base::sync_with_stdio(false);
    cerr.tie(nullptr);
    cin.tie(nullptr);

    if (argc < 2) {
        cerr << argv[0] << ": not enough arguments\n";
        exit(EXIT_FAILURE);
    } else if (strcmp(argv[1], "--help") == 0) {
        cout << "Usage:\n"
            << "  " << argv[0] << " -i -f FILE -t FILE\n"
            << "  " << argv[0] << " -s -f FILE\n";
        exit(EXIT_SUCCESS);
    }

    int command = 0;
    const char *index_file = nullptr, *texts_file = nullptr;
    for (int opt; opt = getopt(argc, argv, "f:ist:"), opt != -1; ) {
        switch (opt) {
            case ':':
                command = -1;
                cerr << argv[0] << ": unknown option -- "
                    << static_cast<char>(optopt) << '\n';
                break;
            case '?':
                command = -1;
                break;
            case 'f':
                index_file = optarg;
                break;
            case 'i':
            case 's':
                if (command != 0) {
                    command = -1;
                    cerr << argv[0] << ": You may not specify more than one "
                        "'-i' or '-s' option\n";
                } else
                    command = opt;
                break;
            case 't':
                texts_file = optarg;
                break;
            default:
                assert(false);
        }
        if (command == -1)
            break;
    }

    if (command == 0)
        cerr << argv[0] << ": missing command\n";
    else if (command != -1 && !index_file)
        cerr << argv[0] << ": option requires an argument -- f\n";
    else if (command == 'i' && !texts_file)
        cerr << argv[0] << ": option requires an argument -- t\n";
    if (command <= 0 || !index_file || (command == 'i' && !texts_file)) {
        cerr << "Try '" << argv[0] << " --help' for more information.\n";
        exit(EXIT_FAILURE);
    }

    try {
        switch (command) {
            case 'i':
                ofstream(
                    index_file,
                    ios_base::binary | ios_base::out | ios_base::trunc
                ) << make_index(texts_file);
                break;
            case 's':
                break;
            default:
                assert(false);
        }
    } catch (const exception &except) {
        cerr << except.what() << '\n';
        exit(EXIT_FAILURE);
    }

    return 0;
}
