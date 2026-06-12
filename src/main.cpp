#include <iostream>
#include <string>
#include "hybrid_search/indexer.hpp"
#include "hybrid_search/hybrid_searcher.hpp"
#include "external/httplib.h"
#include "external/json.hpp"

using json = nlohmann::json;
using namespace httplib;

int main() {
    hs::Indexer indexer;
    std::string data_path = "200k_abstracts/train.txt";
    
    std::cout << "Loading dataset..." << std::endl;
    if (!indexer.load_from_file(data_path, 20000)) { // Limit to 20k for speed in this demo
        return 1;
    }

    std::cout << "Building hybrid index..." << std::endl;
    hs::HybridSearcher searcher;
    searcher.build_index(indexer.get_documents());

    Server svr;

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
