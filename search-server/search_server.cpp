#include <string>
#include <vector>
#include <set>
#include <map>
#include <tuple>
#include <iostream>
#include <numeric>
#include <cmath>

#include "search_server.h"

using namespace std::string_literals;

inline static constexpr int INVALID_DOCUMENT_ID = -1;

SearchServer::SearchServer(const std::string& stop_words_text)
    : SearchServer(
        SplitIntoWords(stop_words_text)) {
}

std::vector<Document> SearchServer::FindTopDocuments(const std::string& raw_query, DocumentStatus status) const {
    return FindTopDocuments(
        raw_query, [status](int document_id, DocumentStatus document_status, int rating) {
            return document_status == status;
        });
}

std::vector<Document> SearchServer::FindTopDocuments(const std::string& raw_query) const {
    return FindTopDocuments(raw_query, DocumentStatus::ACTUAL);
}

void SearchServer::AddDocument(int document_id, const std::string& document, DocumentStatus status,
                     const std::vector<int>& ratings) {

    if(document_id < 0)
        throw std::invalid_argument("попытка добавить документ с отрицательным id"s);
    else if (documents_.count(document_id))
        throw std::invalid_argument("попытка добавить документ c id ранее добавленного документа"s);

    const std::vector<std::string> words = SplitIntoWordsNoStop(document);

    const double inv_word_count = 1.0 / words.size();
    for (const std::string& word : words) {
        if(FindSpecSymb(word))
            throw std::invalid_argument("Наличие специальных символов в тексте добавляемого документа"s);
        word_to_document_freqs_[word][document_id] += inv_word_count;
    }
    documents_.emplace(document_id, DocumentData{ComputeAverageRating(ratings), status});
    ids_in_order_of_addition_.push_back(document_id);

}

int SearchServer::GetDocumentCount() const {
        return documents_.size();
}

std::tuple<std::vector<std::string>, DocumentStatus> SearchServer::MatchDocument(const std::string& raw_query,
                                                    int document_id) const { 

    const Query query = ParseQuery(raw_query);
    std::vector<std::string> matched_words;
    for (const std::string& word : query.plus_words) {
        if (word_to_document_freqs_.count(word) == 0) {
            continue;
        }
        if (word_to_document_freqs_.at(word).count(document_id)) {
            matched_words.push_back(word);
        }
    }
    for (const std::string& word : query.minus_words) {
        if (word_to_document_freqs_.count(word) == 0) {
            continue;
        }
        if (word_to_document_freqs_.at(word).count(document_id)) {
            matched_words.clear();
            break;
        }
    }
    return {matched_words, documents_.at(document_id).status};

}

int SearchServer::GetDocumentId(int index) const {
    
    int document_count = GetDocumentCount();
    if((index >= document_count) || (index < 0)) {
        throw std::out_of_range("индекс переданного документа выходит за пределы допустимого диапазона"s);
    } else {
        return ids_in_order_of_addition_.at(index);
    }
    return INVALID_DOCUMENT_ID;
}

bool SearchServer::IsStopWord(const std::string& word) const {
    return stop_words_.count(word) > 0;
}


std::vector<std::string> SearchServer::SplitIntoWordsNoStop(const std::string& text) const {

    std::vector<std::string> words;
    for (const std::string& word : SplitIntoWords(text)) {
        if (!IsStopWord(word)) {
            words.push_back(word);
        }
    }
    return words;
}

int SearchServer::ComputeAverageRating(const std::vector<int>& ratings) {

    if (ratings.empty()) {
        return 0;
    }
    
    int rating_sum = std::accumulate(ratings.begin(), ratings.end(), 0);
    return rating_sum / static_cast<int>(ratings.size());
}

SearchServer::QueryWord SearchServer::ParseQueryWord(std::string word) const {

    bool is_minus = false;

    if((word[0] == '-') && (word[1] == '-')){
        throw std::invalid_argument("наличие более чем одного минуса перед минус словом"s);
    } else if (word == "-"s) {
        throw std::invalid_argument("отсутствие текста после символа «минус» в поисковом запросе"s);
    } else if (FindSpecSymb(word)) {
        throw std::invalid_argument("в словах поискового запроса есть специальные символы"s);
    }
    
    if (word[0] == '-') {
        is_minus = true;
        word = word.substr(1);
    }
    return {word, is_minus, IsStopWord(word)};
}

SearchServer::Query SearchServer::ParseQuery(const std::string& text) const {
    
    Query query; 
    for (const std::string& word : SplitIntoWords(text)) {
        const QueryWord query_word = ParseQueryWord(word);
        if (!query_word.is_stop) {
            if (query_word.is_minus) {
                query.minus_words.insert(query_word.data);
            } else {
                query.plus_words.insert(query_word.data);
            }
        }
    }
    return query;
}

double SearchServer::ComputeWordInverseDocumentFreq(const std::string& word) const {
    return log(GetDocumentCount() * 1.0 / word_to_document_freqs_.at(word).size());
}