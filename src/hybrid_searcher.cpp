#include "hybrid_search/hybrid_searcher.hpp"
#include "hybrid_search/utils.hpp"
#include <map>

namespace hs {

HybridSearcher::HybridSearcher() {
    bm25_ = std::make_unique<BM25>();
    vector_search_ = std::make_unique<VectorSearch>();
}

void HybridSearcher::build_index(const std::vector<Document>& docs) {
    docs_ = docs;
    bm25_->build_index(docs);
    vector_search_->build_index(docs);
}

std::vector<SearchResult> HybridSearcher::search(const std::string& query, size_t top_k, double alpha) {
    auto query_tokens = utils::tokenize(query);
    
    // Get top 100 from each to have a good pool for fusion
    auto bm25_results = bm25_->search(query_tokens, 100);
    auto vector_results = vector_search_->search(query_tokens, 100);
    
    return rrf(bm25_results, vector_results, top_k);
}

std::vector<SearchResult> HybridSearcher::rrf(const std::vector<std::pair<size_t, double>>& bm25_results,
                                             const std::vector<std::pair<size_t, double>>& vector_results,
                                             size_t top_k) {
    const double k = 60.0; // RRF constant
    std::unordered_map<size_t, double> rrf_scores;

    // Fusion logic
    for (size_t i = 0; i < bm25_results.size(); ++i) {
        rrf_scores[bm25_results[i].first] += 1.0 / (k + i + 1);
    }

    for (size_t i = 0; i < vector_results.size(); ++i) {
        rrf_scores[vector_results[i].first] += 1.0 / (k + i + 1);
    }

    std::vector<std::pair<size_t, double>> sorted_rrf(rrf_scores.begin(), rrf_scores.end());
    std::sort(sorted_rrf.begin(), sorted_rrf.end(), [](const auto& a, const auto& b) {
        return a.second > b.second;
    });

    std::vector<SearchResult> results;
    for (size_t i = 0; i < std::min(top_k, sorted_rrf.size()); ++i) {
        size_t doc_idx = sorted_rrf[i].first;
        results.push_back({docs_[doc_idx].id, docs_[doc_idx].text, sorted_rrf[i].second});
    }

    return results;
}

} // namespace hs
