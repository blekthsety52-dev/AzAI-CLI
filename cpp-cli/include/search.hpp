#pragma once

#include <string>
#include <vector>
#include "azai_common.hpp"

/**
 * @brief Execute a keyword search in Azure AI Search
 * @param profile User profile with Azure AI Search credentials
 * @param query Search query text
 * @param top Maximum number of results to return
 * @return Vector of SearchResult objects
 */
std::vector<SearchResult> executeKeywordSearch(const UserProfile& profile, 
                                               const std::string& query, 
                                               int top = 5);

/**
 * @brief Execute a vector search in Azure AI Search
 * @param profile User profile with Azure AI Search credentials
 * @param query Search query text (will be vectorized automatically)
 * @param vectorField Name of the vector field in the index
 * @param top Maximum number of results to return
 * @return Vector of SearchResult objects
 */
std::vector<SearchResult> executeVectorSearch(const UserProfile& profile,
                                              const std::string& query,
                                              const std::string& vectorField = "contentVector",
                                              int top = 5);

/**
 * @brief Execute a hybrid search (keyword + vector)
 * @param profile User profile with Azure AI Search credentials
 * @param query Search query text
 * @param vectorField Name of the vector field in the index
 * @param top Maximum number of results to return
 * @return Vector of SearchResult objects
 */
std::vector<SearchResult> executeHybridSearch(const UserProfile& profile,
                                              const std::string& query,
                                              const std::string& vectorField = "contentVector",
                                              int top = 5);

/**
 * @brief Print search results in a formatted manner
 * @param results Vector of SearchResult objects
 */
void printSearchResults(const std::vector<SearchResult>& results);
