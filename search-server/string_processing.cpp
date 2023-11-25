#include "string_processing.h"
#include <string>
#include <vector>

bool FindSpecSymb(const std::string& word) {
    for(const auto& symb : word) {
        if( static_cast<uint32_t>(symb) <= 31u)
            return true;
    }
    return false;
}

std::vector<std::string> SplitIntoWords(const std::string& text) {

    std::vector<std::string> words;
    std::string word;
    for (const char c : text) {
        if (c == ' ') {
            if (!word.empty()) {
                words.push_back(word);
                word.clear();
            }
        } else {
            word += c;
        }
    }
    
    if (!word.empty()) {
        words.push_back(word);
    }
    return words;
}