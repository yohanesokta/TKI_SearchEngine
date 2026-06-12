cmake_minimum_required(VERSION 3.10)
project(HybridSearch)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Includes
include_directories(include)
include_directories(include/external)

# Find threads for httplib
find_package(Threads REQUIRED)

# Source files
set(SOURCES
    src/indexer.cpp
    src/bm25.cpp
    src/vector_search.cpp
    src/hybrid_searcher.cpp
)

# Main executable (Server)
add_executable(hybrid_server src/main.cpp ${SOURCES})
target_link_libraries(hybrid_server PRIVATE Threads::Threads)

# Add optimization flags
if(MSVC)
    target_compile_options(hybrid_server PRIVATE /W4 /O2)
else()
    target_compile_options(hybrid_server PRIVATE -Wall -Wextra -O3)
endif()
