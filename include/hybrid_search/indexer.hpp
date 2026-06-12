#pragma once
#include <vector>
#include <string>
#include "document.hpp"

namespace hs {

class Indexer {
public:
    Indexer() = default;
    
    // Load documents from the PubMed 200k RTC format
    bool load_from_file(const std::string& filepath, size_t max_docs = 10000);
    
    const std::vector<Document>& get_documents() const { return documents_; }
    
private:
    std::vector<Document> documents_;
};

} // namespace hs
