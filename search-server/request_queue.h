#pragma once

#include <string>
#include <vector>
#include <deque>

#include "search_server.h"


class RequestQueue {
public:

    explicit RequestQueue(const SearchServer& search_server);

    template <typename DocumentPredicate>
    std::vector<Document> AddFindRequest(const std::string& raw_query, DocumentPredicate document_predicate);

    std::vector<Document> AddFindRequest(const std::string& raw_query, DocumentStatus status);

    std::vector<Document> AddFindRequest(const std::string& raw_query);

    int GetNoResultRequests() const;

private:
    struct QueryResult {
        size_t result_size_;
        uint32_t minuts_cnt_;
    };
    std::deque<QueryResult> requests_;
    const static int min_in_day_ = 1440;
    const SearchServer& local_search_server_;
    int empty_result_query_;
    uint32_t current_minut_;
    
    void AddResult (size_t size_of_request_result);

};

template <typename DocumentPredicate>
std::vector<Document> RequestQueue::AddFindRequest(const std::string& raw_query, DocumentPredicate document_predicate) {
    auto query_result = local_search_server_.FindTopDocuments(raw_query, document_predicate);        
    AddResult(query_result.size());
    return query_result;
}

