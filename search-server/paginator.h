#pragma once

#include <iostream>
#include <vector>

#include "document.h"

using namespace std::string_literals;


template <typename Iterator>
class IteratorRange {
public: 

    IteratorRange(Iterator start_page_it, Iterator end_page_it, size_t page_size) :
        start_page_it_(start_page_it), end_page_it_(end_page_it), page_size_(page_size) {
    }

    auto begin() const {
        return start_page_it_;
    }

    auto end() const {
        return end_page_it_;
    }

    size_t size() {
        return page_size_;
    }

private:

    Iterator start_page_it_;
    Iterator end_page_it_;
    size_t page_size_;

};


template <typename Iterator>
class Paginator {
public:

    Paginator(Iterator start_it, Iterator end_it, size_t page_size);
    
    auto begin() const {
        return pages_vector_.begin();
    }

    auto end() const {
        return pages_vector_.end();
    }

    size_t size() const {
        return pages_vector_.size();
    }

private:

    std::vector<IteratorRange<Iterator>> pages_vector_;

};

template <typename Iterator>
Paginator<Iterator>::Paginator(Iterator start_it, Iterator end_it, size_t page_size)
{ 
    for(int temp_cnt_doc = distance(start_it, end_it); temp_cnt_doc > 0;) {
        if(temp_cnt_doc >= page_size) {
            Iterator temp_end_it = next(start_it, page_size);
            pages_vector_.push_back({start_it, temp_end_it, static_cast<size_t>(distance(start_it, temp_end_it))}); 
            start_it = temp_end_it;
            temp_cnt_doc -= page_size;
            continue;
        }
        pages_vector_.push_back({start_it, end_it, static_cast<size_t>(distance(start_it, end_it))});
        break;
    }
}

/* Операция << для класса IteratorRange*/
template <typename Iterator>
std::ostream& operator<<(std::ostream& out, const IteratorRange<Iterator>& page) {
    /* Выводим содержимое объекта value в output */
    for( auto it = page.begin(); it != page.end(); it++)
        out << *it;
    return out;
}

template <typename Container>
auto Paginate(const Container& c, size_t page_size) {
    return Paginator(begin(c), end(c), page_size);
}

