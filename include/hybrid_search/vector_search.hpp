#pragma once
#include <vector>
#include <string>
#include <unordered_map>
#include "document.hpp"

namespace hs {

class VectorSearch {
public:
    VectorSearch() = default;
    
    void build_index(const std::vector<Document>& docs);
    std::vector<std::pair<size_t, double>> search(const std::vector<std::string>& query_tokens, size_t top_k = 10);

private:
    struct SparseVector {
        std::unordered_map<size_t, double> values; // term_id -> weight
        double norm = 0.0;
    };

    std::vector<SparseVector> doc_vectors_;
    std::unordered_map<std::string, size_t> term_to_id_;
    std::vector<double> idf_;
    size_t num_docs_ = 0;

    double cosine_similarity(const SparseVector& v1, const SparseVector& v2);
};

} // namespace hs
