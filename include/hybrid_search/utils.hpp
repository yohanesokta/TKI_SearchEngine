#pragma once
#include <string>
#include <vector>
#include <algorithm>
#include <cctype>
#include <sstream>

namespace hs {
namespace utils {

inline std::string to_lower(std::string s) {
    std::transform(s.begin(), s.end(), s.begin(),
                   [](unsigned char c){ return std::tolower(c); });
    return s;
}

inline std::vector<std::string> tokenize(const std::string& text) {
    std::vector<std::string> tokens;
    std::string token;
    std::stringstream ss(text);
    while (ss >> token) {
        // Remove punctuation
        token.erase(std::remove_if(token.begin(), token.end(), ispunct), token.end());
        if (!token.empty()) {
            tokens.push_back(to_lower(token));
        }
    }
    return tokens;
}

} // namespace utils
} // namespace hs
