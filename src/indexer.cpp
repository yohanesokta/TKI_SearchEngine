#include "hybrid_search/indexer.hpp"
#include "hybrid_search/utils.hpp"
#include <fstream>
#include <iostream>

namespace hs {

bool Indexer::load_from_file(const std::string& filepath, size_t max_docs) {
    std::ifstream file(filepath);
    if (!file.is_open()) {
        std::cerr << "Failed to open file: " << filepath << std::endl;
        return false;
    }

    std::string line;
    std::string current_id;
    while (std::getline(file, line) && documents_.size() < max_docs) {
        if (line.empty()) continue;

        if (line.substr(0, 3) == "###") {
            current_id = line.substr(3);
        } else {
            size_t tab_pos = line.find('\t');
            if (tab_pos != std::string::npos) {
                std::string label = line.substr(0, tab_pos);
                std::string text = line.substr(tab_pos + 1);
                
                Document doc;
                doc.id = current_id;
                doc.label = label;
                doc.text = text;
                doc.tokens = utils::tokenize(text);
                documents_.push_back(std::move(doc));
            }
        }
    }
    
    std::cout << "Loaded " << documents_.size() << " sentences." << std::endl;
    return true;
}

} // namespace hs
