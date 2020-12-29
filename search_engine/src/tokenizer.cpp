#include <cctype>

#include <search_engine/tokenizer.hpp>

void index_file() {
}

// если в тексте есть тире
// инженер-механик co-education
// или точки
// C.A.T. U.S.A.
// то они обрабатываются в токенизаторе, а не нормализаторе
