#include "hybrid_search/vector_search.hpp"
#include <cmath>
#include <algorithm>
#include <iostream>
#include <unordered_set>

namespace hs {

void VectorSearch::build_index(const std::vector<Document>& docs) {
    num_docs_ = docs.size();
    if (num_docs_ == 0) return;

    // 1. Map terms to IDs and count doc frequencies
    std::unordered_map<std::string, size_t> term_doc_freq;
    for (const auto& doc : docs) {
        std::unordered_set<std::string> unique_terms(doc.tokens.begin(), doc.tokens.end());
        for (const auto& term : unique_terms) {
            term_doc_freq[term]++;
        }
    }

    size_t next_id = 0;
    for (const auto& [term, freq] : term_doc_freq) {
        term_to_id_[term] = next_id++;
        idf_.push_back(std::log(static_cast<double>(num_docs_) / (freq + 1)) + 1.0);
    }

    // 2. Build sparse TF-IDF vectors for documents
    doc_vectors_.resize(num_docs_);
    for (size_t i = 0; i < num_docs_; ++i) {
        std::unordered_map<size_t, size_t> term_counts;
        for (const auto& token : docs[i].tokens) {
            auto it = term_to_id_.find(token);
            if (it != term_to_id_.end()) {
                term_counts[it->second]++;
            }
        }

        double sum_sq = 0;
        for (const auto& [term_id, count] : term_counts) {
            double tf = static_cast<double>(count); // Simple TF
            double tfidf = tf * idf_[term_id];
            doc_vectors_[i].values[term_id] = tfidf;
            sum_sq += tfidf * tfidf;
        }
        doc_vectors_[i].norm = std::sqrt(sum_sq);
    }
    
    std::cout << "Vector search index built." << std::endl;
}

double VectorSearch::cosine_similarity(const SparseVector& v1, const SparseVector& v2) {
    if (v1.norm == 0 || v2.norm == 0) return 0.0;
    
    double dot_product = 0;
    // Iterate over the smaller vector
    const auto& small = (v1.values.size() < v2.values.size()) ? v1.values : v2.values;
    const auto& large = (v1.values.size() < v2.values.size()) ? v2.values : v1.values;
    
    for (const auto& [id, val] : small) {
        auto it = large.find(id);
        if (it != large.end()) {
            dot_product += val * it->second;
        }
    }
    
    return dot_product / (v1.norm * v2.norm);
}

std::vector<std::pair<size_t, double>> VectorSearch::search(const std::vector<std::string>& query_tokens, size_t top_k) {
    SparseVector query_vec;
    std::unordered_map<size_t, size_t> term_counts;
    for (const auto& token : query_tokens) {
        auto it = term_to_id_.find(token);
        if (it != term_to_id_.end()) {
            term_counts[it->second]++;
        }
    }

    double sum_sq = 0;
    for (const auto& [term_id, count] : term_counts) {
        double tf = static_cast<double>(count);
        double tfidf = tf * idf_[term_id];
        query_vec.values[term_id] = tfidf;
        sum_sq += tfidf * tfidf;
    }
    query_vec.norm = std::sqrt(sum_sq);

    std::vector<std::pair<size_t, double>> scores;
    for (size_t i = 0; i < num_docs_; ++i) {
        double sim = cosine_similarity(query_vec, doc_vectors_[i]);
        if (sim > 0) {
            scores.push_back({i, sim});
        }
    }

    std::sort(scores.begin(), scores.end(), [](const auto& a, const auto& b) {
        return a.second > b.second;
    });

    if (scores.size() > top_k) {
        scores.resize(top_k);
    }
    
    return scores;
}

} // namespace hs
