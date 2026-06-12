# Hybrid Search in C++

A modular and professional C++ implementation of a Hybrid Search engine for the PubMed 200k Abstracts dataset.

## Features
- **BM25 Search:** Efficient keyword-based retrieval.
- **Vector Search:** TF-IDF based cosine similarity for semantic-like retrieval.
- **Reciprocal Rank Fusion (RRF):** Professionally combines results from multiple search channels.
- **Modular Design:** Clear separation between indexing, search algorithms, and the web interface.
- **HTTP API:** Single-header based HTTP server for easy integration.

## Architecture
- `hs::Indexer`: Parses the PubMed dataset format.
- `hs::BM25`: Implements the BM25 scoring algorithm.
- `hs::VectorSearch`: Implements sparse vector search with TF-IDF weighting.
- `hs::HybridSearcher`: Orchestrates the search process and performs result fusion.

## How to Build
```bash
mkdir build && cd build
cmake ..
make
```

## How to Run
```bash
./build/hybrid_server
```

## API Usage
Search for documents:
```bash
curl "http://localhost:8080/search?q=query+string&k=10"
```

Health check:
```bash
curl "http://localhost:8080/health"
```
