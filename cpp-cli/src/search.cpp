#include "search.hpp"
#include <cpr/cpr.h>
#include <fmt/core.h>
#include <iostream>
#include <iomanip>

std::vector<SearchResult> executeKeywordSearch(const UserProfile& profile, 
                                               const std::string& query, 
                                               int top) {
    std::string url = fmt::format(
        "{}/indexes/{}/docs/search?api-version=2023-11-01",
        profile.searchEndpoint,
        profile.searchIndex
    );

    json payload = {
        {"search", query},
        {"top", top},
        {"count", true}
    };

    cpr::Response r = cpr::Post(
        cpr::Url{url},
        cpr::Header{
            {"Content-Type", "application/json"},
            {"api-key", profile.searchKey}
        },
        cpr::Body{payload.dump()}
    );

    std::vector<SearchResult> results;
    if (r.status_code == 200) {
        try {
            auto responseJson = json::parse(r.text);
            auto values = responseJson["value"];
            for (const auto& doc : values) {
                results.emplace_back(
                    doc.value("id", ""),
                    doc.value("content", ""),
                    doc.value("@search.score", 0.0f)
                );
            }
        } catch (const std::exception& e) {
            std::cerr << fmt::format("Error parsing search response: {}", e.what()) << std::endl;
        }
    } else {
        std::cerr << fmt::format("Search Error: {} - {}", r.status_code, r.text) << std::endl;
    }

    return results;
}

std::vector<SearchResult> executeVectorSearch(const UserProfile& profile,
                                              const std::string& query,
                                              const std::string& vectorField,
                                              int top) {
    // Get embeddings for the query
    auto vector = getEmbeddings(query, profile);
    
    if (vector.empty()) {
        std::cerr << "Error: Could not generate embeddings for query" << std::endl;
        return std::vector<SearchResult>();
    }

    std::string url = fmt::format(
        "{}/indexes/{}/docs/search?api-version=2023-11-01",
        profile.searchEndpoint,
        profile.searchIndex
    );

    json payload = {
        {"vectorQueries", {{
            {"kind", "vector"},
            {"vector", vector},
            {"fields", vectorField},
            {"k", top}
        }}},
        {"select", "id,content"},
        {"count", true}
    };

    cpr::Response r = cpr::Post(
        cpr::Url{url},
        cpr::Header{
            {"Content-Type", "application/json"},
            {"api-key", profile.searchKey}
        },
        cpr::Body{payload.dump()}
    );

    std::vector<SearchResult> results;
    if (r.status_code == 200) {
        try {
            auto responseJson = json::parse(r.text);
            auto values = responseJson["value"];
            for (const auto& doc : values) {
                results.emplace_back(
                    doc.value("id", ""),
                    doc.value("content", ""),
                    doc.value("@search.score", 0.0f)
                );
            }
        } catch (const std::exception& e) {
            std::cerr << fmt::format("Error parsing vector search response: {}", e.what()) << std::endl;
        }
    } else {
        std::cerr << fmt::format("Vector Search Error: {} - {}", r.status_code, r.text) << std::endl;
    }

    return results;
}

std::vector<SearchResult> executeHybridSearch(const UserProfile& profile,
                                              const std::string& query,
                                              const std::string& vectorField,
                                              int top) {
    // Get embeddings for the query
    auto vector = getEmbeddings(query, profile);
    
    if (vector.empty()) {
        std::cerr << "Error: Could not generate embeddings for query" << std::endl;
        return std::vector<SearchResult>();
    }

    std::string url = fmt::format(
        "{}/indexes/{}/docs/search?api-version=2023-11-01",
        profile.searchEndpoint,
        profile.searchIndex
    );

    json payload = {
        {"search", query},
        {"vectorQueries", {{
            {"kind", "vector"},
            {"vector", vector},
            {"fields", vectorField},
            {"k", top}
        }}},
        {"select", "id,content"},
        {"top", top},
        {"count", true}
    };

    cpr::Response r = cpr::Post(
        cpr::Url{url},
        cpr::Header{
            {"Content-Type", "application/json"},
            {"api-key", profile.searchKey}
        },
        cpr::Body{payload.dump()}
    );

    std::vector<SearchResult> results;
    if (r.status_code == 200) {
        try {
            auto responseJson = json::parse(r.text);
            auto values = responseJson["value"];
            for (const auto& doc : values) {
                results.emplace_back(
                    doc.value("id", ""),
                    doc.value("content", ""),
                    doc.value("@search.score", 0.0f)
                );
            }
        } catch (const std::exception& e) {
            std::cerr << fmt::format("Error parsing hybrid search response: {}", e.what()) << std::endl;
        }
    } else {
        std::cerr << fmt::format("Hybrid Search Error: {} - {}", r.status_code, r.text) << std::endl;
    }

    return results;
}

void printSearchResults(const std::vector<SearchResult>& results) {
    if (results.empty()) {
        std::cout << "No results found." << std::endl;
        return;
    }

    std::cout << fmt::format("\nFound {} result(s):\n", results.size()) << std::endl;

    for (size_t i = 0; i < results.size(); ++i) {
        const auto& result = results[i];
        std::cout << fmt::format("--- Result {} ---", i + 1) << std::endl;
        std::cout << fmt::format("ID: {}", result.id) << std::endl;
        std::cout << fmt::format("Score: {:.4f}", result.score) << std::endl;
        std::cout << fmt::format("Content:\n{}\n", result.content) << std::endl;
    }
}
