#include <cctype>

#include <search_engine/tokenizator.hpp>

static void index_file(const char * const filename, index_ctor &index) {
    const memmap map(filename);
    const char * const data = map.data(), data_end = data + map.size();
    for (const char *ptr = data; ptr < data_end; ) {
        ;
    }
        // если в тексте есть тире
        // инженер-механик co-education
        // или точки
        // C.A.T. U.S.A.
        // то они обрабатываются в токенизаторе, а не нормализаторе
}
