#pragma once
#include <string>
#include <vector>
#include <map>

namespace hs {

struct Document {
    std::string id;
    std::string text;
    std::string label; // BACKGROUND, METHODS, etc.
    std::vector<std::string> tokens;
};

struct SearchResult {
    std::string doc_id;
    std::string text;
    double score;
};

} // namespace hs
