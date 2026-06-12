#pragma once
#include <vector>
#include <string>
#include <unordered_map>
#include <cmath>
#include "document.hpp"

namespace hs {

class BM25 {
public:
    BM25(double k1 = 1.2, double b = 0.75) : k1_(k1), b_(b) {}
    
    void build_index(const std::vector<Document>& docs);
    std::vector<std::pair<size_t, double>> search(const std::vector<std::string>& query_tokens, size_t top_k = 10);

private:
    double k1_;
    double b_;
    double avg_dl_ = 0;
    size_t num_docs_ = 0;
    
    // term -> {doc_index -> frequency}
    std::unordered_map<std::string, std::unordered_map<size_t, size_t>> inverted_index_;
    // doc_index -> length
    std::vector<size_t> doc_lengths_;
    // term -> doc_count (for IDF)
    std::unordered_map<std::string, size_t> term_doc_counts_;

    double calculate_idf(const std::string& term);
};

} // namespace hs
