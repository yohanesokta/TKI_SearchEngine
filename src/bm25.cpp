#include "hybrid_search/bm25.hpp"
#include <algorithm>
#include <iostream>

namespace hs {

void BM25::build_index(const std::vector<Document>& docs) {
    num_docs_ = docs.size();
    doc_lengths_.resize(num_docs_);
    size_t total_length = 0;

    for (size_t i = 0; i < num_docs_; ++i) {
        doc_lengths_[i] = docs[i].tokens.size();
        total_length += doc_lengths_[i];
        
        std::unordered_map<std::string, size_t> term_freqs;
        for (const auto& token : docs[i].tokens) {
            term_freqs[token]++;
        }
        
        for (const auto& [term, freq] : term_freqs) {
            inverted_index_[term][i] = freq;
            term_doc_counts_[term]++;
        }
    }
    
    avg_dl_ = static_cast<double>(total_length) / num_docs_;
    std::cout << "BM25 index built with " << inverted_index_.size() << " unique terms." << std::endl;
}

double BM25::calculate_idf(const std::string& term) {
    auto it = term_doc_counts_.find(term);
    if (it == term_doc_counts_.end()) return 0.0;
    
    size_t n_q = it->second;
    return std::log((num_docs_ - n_q + 0.5) / (n_q + 0.5) + 1.0);
}

std::vector<std::pair<size_t, double>> BM25::search(const std::vector<std::string>& query_tokens, size_t top_k) {
    std::unordered_map<size_t, double> scores;
    
    for (const auto& token : query_tokens) {
        double idf = calculate_idf(token);
        if (idf <= 0) continue;
        
        auto it = inverted_index_.find(token);
        if (it != inverted_index_.end()) {
            for (const auto& [doc_idx, freq] : it->second) {
                double numerator = freq * (k1_ + 1);
                double denominator = freq + k1_ * (1 - b_ + b_ * (doc_lengths_[doc_idx] / avg_dl_));
                scores[doc_idx] += idf * (numerator / denominator);
            }
        }
    }
    
    std::vector<std::pair<size_t, double>> sorted_results(scores.begin(), scores.end());
    std::sort(sorted_results.begin(), sorted_results.end(), 
              [](const auto& a, const auto& b) { return a.second > b.second; });
    
    if (sorted_results.size() > top_k) {
        sorted_results.resize(top_k);
    }
    
    return sorted_results;
}

} // namespace hs
