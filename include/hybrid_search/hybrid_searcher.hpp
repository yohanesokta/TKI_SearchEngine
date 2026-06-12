#pragma once
#include "bm25.hpp"
#include "vector_search.hpp"
#include <memory>

namespace hs {

class HybridSearcher {
public:
    HybridSearcher();
    
    void build_index(const std::vector<Document>& docs);
    std::vector<SearchResult> search(const std::string& query, size_t top_k = 10, double alpha = 0.5);

private:
    std::unique_ptr<BM25> bm25_;
    std::unique_ptr<VectorSearch> vector_search_;
    std::vector<Document> docs_;
    
    // Reciprocal Rank Fusion
    std::vector<SearchResult> rrf(const std::vector<std::pair<size_t, double>>& bm25_results,
                                 const std::vector<std::pair<size_t, double>>& vector_results,
                                 size_t top_k);
};

} // namespace hs
