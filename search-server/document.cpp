#include "document.h"


/* Операция << для Document */
std::ostream& operator<<(std::ostream& output, const Document& page_document) {
    /* Выводим содержимое объекта value в output */
    output << "{ "s
         << "document_id = "s << page_document.id << ", "s
         << "relevance = "s << page_document.relevance << ", "s
         << "rating = "s << page_document.rating << " }"s;
    return output;
}