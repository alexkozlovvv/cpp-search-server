#pragma once

#include <vector>
#include <set>
#include <string>


std::vector<std::string> SplitIntoWords(const std::string& text);

bool FindSpecSymb(const std::string& word);

template <typename StringContainer>
std::set<std::string> MakeUniqueNonEmptyStrings(const StringContainer& strings) {

    std::set<std::string> non_empty_strings;
    for (const std::string& str : strings) {
        if (!str.empty()) {
            non_empty_strings.insert(str);
        }
    }
    return non_empty_strings;
}