#include <iostream>
#include <string>
#include <fstream>
#include <sstream>
#include "hybrid_search/indexer.hpp"
#include "hybrid_search/hybrid_searcher.hpp"
#include "external/httplib.h"
#include "external/json.hpp"

using json = nlohmann::json;
using namespace httplib;

// Helper to read file content
std::string read_file(const std::string& path) {
    std::ifstream file(path);
    if (!file.is_open()) return "";
    std::stringstream ss;
    ss << file.rdbuf();
    return ss.str();
}

int main() {
    hs::Indexer indexer;
    std::string data_path = "200k_abstracts/train.txt";
    
    std::cout << "Loading dataset..." << std::endl;
    if (!indexer.load_from_file(data_path, 20000)) { 
        return 1;
    }

    std::cout << "Building hybrid index..." << std::endl;
    hs::HybridSearcher searcher;
    searcher.build_index(indexer.get_documents());

    Server svr;

    // Serve static files
    svr.Get("/", [](const Request&, Response& res) {
        std::string html = read_file("templates/index.html");
        res.set_content(html, "text/html");
    });

    svr.Get("/css/style.css", [](const Request&, Response& res) {
        std::string css = read_file("templates/css/style.css");
        res.set_content(css, "text/css");
    });

    svr.Get("/js/main.js", [](const Request&, Response& res) {
        std::string js = read_file("templates/js/main.js");
        res.set_content(js, "application/javascript");
    });

    // API Endpoint
    svr.Get("/search", [&](const Request& req, Response& res) {
        if (!req.has_param("q")) {
            res.status = 400;
            res.set_content("Missing 'q' parameter", "text/plain");
            return;
        }

        std::string query = req.get_param_value("q");
        int top_k = 10;
        if (req.has_param("k")) {
            try {
                top_k = std::stoi(req.get_param_value("k"));
            } catch (...) {}
        }

        auto results = searcher.search(query, top_k);

        json j_results = json::array();
        for (const auto& r : results) {
            j_results.push_back({
                {"doc_id", r.doc_id},
                {"text", r.text},
                {"score", r.score}
            });
        }

        res.set_content(j_results.dump(4), "application/json");
    });

    svr.Get("/health", [](const Request&, Response& res) {
        res.set_content("{\"status\": \"ok\"}", "application/json");
    });

    std::cout << "Server starting at http://localhost:8080" << std::endl;
    svr.listen("0.0.0.0", 8080);

    return 0;
}
