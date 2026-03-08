#pragma once

#include <string>
#include <vector>
#include <nlohmann/json.hpp>

using json = nlohmann::json;

/**
 * @struct UserProfile
 * @brief Stores Azure service credentials and endpoints
 */
struct UserProfile {
    // Azure OpenAI Configuration
    std::string endpoint;           ///< Azure OpenAI endpoint (e.g., https://your-resource.openai.azure.com/)
    std::string apiKey;             ///< Azure OpenAI API key
    std::string deploymentName;     ///< Deployment name for the model (e.g., "gpt-35-turbo")
    std::string apiVersion;         ///< API version (e.g., "2023-05-15")

    // Azure AI Search Configuration
    std::string searchEndpoint;     ///< Azure AI Search endpoint (e.g., https://your-search.search.windows.net/)
    std::string searchKey;          ///< Azure AI Search API key
    std::string searchIndex;        ///< Index name in Azure AI Search

    // Embedding Configuration
    std::string embeddingDeployment; ///< Deployment name for embeddings model
    std::string embeddingModel;     ///< Embedding model name

    /**
     * @brief Load profile from JSON configuration
     * @param configPath Path to JSON configuration file
     * @return True if successfully loaded, false otherwise
     */
    bool loadFromConfig(const std::string& configPath);

    /**
     * @brief Validate that all required fields are set
     * @return True if all required fields are set
     */
    bool validate() const;
};

/**
 * @struct SearchResult
 * @brief Represents a single search result from Azure AI Search
 */
struct SearchResult {
    std::string id;
    std::string content;
    float score;
    
    SearchResult(const std::string& id = "", const std::string& content = "", float score = 0.0f)
        : id(id), content(content), score(score) {}
};

/**
 * @struct ChatMessage
 * @brief Represents a chat message in conversation history
 */
struct ChatMessage {
    std::string role;       ///< "system", "user", or "assistant"
    std::string content;    ///< Message content

    ChatMessage(const std::string& role = "", const std::string& content = "")
        : role(role), content(content) {}

    json toJson() const {
        return json{{"role", role}, {"content", content}};
    }
};

/**
 * @brief Get embeddings for a query string using Azure OpenAI
 * @param query The text to embed
 * @param profile User profile with API credentials
 * @return Vector of floats representing the embedding
 */
std::vector<float> getEmbeddings(const std::string& query, const UserProfile& profile);

/**
 * @brief Utility function to print formatted output
 * @param message The message to print
 * @param isError If true, prints to stderr; otherwise stdout
 */
void printOutput(const std::string& message, bool isError = false);

/**
 * @brief Utility function to load JSON from file
 * @param filePath Path to the JSON file
 * @return Parsed JSON object
 */
json loadJsonFile(const std::string& filePath);

/**
 * @brief Utility function to save JSON to file
 * @param filePath Path to save the JSON file
 * @param data JSON data to save
 * @return True if successfully saved
 */
bool saveJsonFile(const std::string& filePath, const json& data);
