#include <string>
#include <vector>

#include "document.h"
#include "request_queue.h"


RequestQueue::RequestQueue(const SearchServer& search_server) 
    : local_search_server_(search_server),
        empty_result_query_{0},
        current_minut_{0} {
}

std::vector<Document> RequestQueue::AddFindRequest(const std::string& raw_query, DocumentStatus status) {
    return AddFindRequest(
        raw_query, [status](int document_id, DocumentStatus document_status, int rating) {
            return document_status == status;
        });
}

std::vector<Document> RequestQueue::AddFindRequest(const std::string& raw_query) {
    return AddFindRequest(raw_query, DocumentStatus::ACTUAL);
}

int RequestQueue::GetNoResultRequests() const {
    // напишите реализацию
    return empty_result_query_;
}

void RequestQueue::AddResult (size_t size_of_request_result) {

    ++current_minut_;
    
    if(!requests_.empty() && (current_minut_ - requests_.front().minuts_cnt_ == min_in_day_)) {
        if(requests_.front().result_size_ == 0)
            --empty_result_query_;
        requests_.pop_front();
    }
    requests_.push_back({size_of_request_result, current_minut_});

    if(!size_of_request_result)
        ++empty_result_query_;
}