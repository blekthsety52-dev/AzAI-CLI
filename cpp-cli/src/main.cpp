#include <iostream>
#include <string>
#include <vector>
#include <cstring>
#include <fmt/core.h>
#include "azai_common.hpp"
#include "openai.hpp"
#include "search.hpp"

void printUsage(const char* programName) {
    std::cout << fmt::format(R"(
Usage: {} [COMMAND] [OPTIONS]

Commands:
  chat          Chat with Azure OpenAI
  search        Search Azure AI Search
  help          Show this help message

Chat Options:
  {} chat [--config <path>] [--prompt <text>] [--system <text>]
  {} chat [--config <path>] --history <file>

Search Options:
  {} search [--config <path>] [--query <text>] [--type <keyword|vector|hybrid>]

Global Options:
  --config <path>    Path to configuration file (default: config.json)
  --prompt <text>    User prompt/query
  --system <text>    System prompt (for chat)
  --query <text>     Search query text
  --type <type>      Search type: keyword, vector, or hybrid (default: hybrid)
  --history <file>   Chat history file (JSON format)
  --help             Show help message

Examples:
  {} chat --config my-config.json --prompt "What is AI?"
  {} search --config my-config.json --query "machine learning" --type vector
  {} chat --config config.json --history conversation.json

Configuration File Format (config.json):
{{
  "endpoint": "https://your-resource.openai.azure.com/",
  "apiKey": "your-api-key",
  "deploymentName": "gpt-35-turbo",
  "apiVersion": "2023-05-15",
  "searchEndpoint": "https://your-search.search.windows.net/",
  "searchKey": "your-search-key",
  "searchIndex": "your-index-name",
  "embeddingDeployment": "embedding",
  "embeddingModel": "text-embedding-ada-002"
}}
)",
    programName, programName, programName, programName, programName, programName, programName) << std::endl;
}

int main(int argc, char* argv[]) {
    if (argc < 2) {
        printUsage(argv[0]);
        return 1;
    }

    std::string command = argv[1];
    std::string configPath = "config.json";
    std::string prompt;
    std::string systemPrompt;
    std::string query;
    std::string searchType = "hybrid";
    std::string historyFile;

    // Parse command line arguments
    for (int i = 2; i < argc; ++i) {
        std::string arg = argv[i];
        
        if (arg == "--config" && i + 1 < argc) {
            configPath = argv[++i];
        } else if (arg == "--prompt" && i + 1 < argc) {
            prompt = argv[++i];
        } else if (arg == "--system" && i + 1 < argc) {
            systemPrompt = argv[++i];
        } else if (arg == "--query" && i + 1 < argc) {
            query = argv[++i];
        } else if (arg == "--type" && i + 1 < argc) {
            searchType = argv[++i];
        } else if (arg == "--history" && i + 1 < argc) {
            historyFile = argv[++i];
        } else if (arg == "--help" || arg == "-h") {
            printUsage(argv[0]);
            return 0;
        }
    }

    // Load configuration
    UserProfile profile;
    if (!profile.loadFromConfig(configPath)) {
        std::cerr << "Failed to load configuration from " << configPath << std::endl;
        return 1;
    }

    // Execute command
    if (command == "chat") {
        if (!historyFile.empty()) {
            // Load conversation history
            auto historyJson = loadJsonFile(historyFile);
            std::vector<ChatMessage> messages;
            
            if (historyJson.is_array()) {
                for (const auto& msg : historyJson) {
                    messages.emplace_back(
                        msg.value("role", ""),
                        msg.value("content", "")
                    );
                }
            }

            if (messages.empty()) {
                std::cerr << "Error: No messages found in history file" << std::endl;
                return 1;
            }

            std::string response = executeOpenAIWithHistory(profile, messages);
            std::cout << response << std::endl;

            // Save updated history
            messages.emplace_back("assistant", response);
            json updatedHistory = json::array();
            for (const auto& msg : messages) {
                updatedHistory.push_back(msg.toJson());
            }
            saveJsonFile(historyFile, updatedHistory);
        } else {
            if (prompt.empty()) {
                std::cerr << "Error: Please provide a prompt with --prompt or --history" << std::endl;
                return 1;
            }

            std::string response = executeOpenAI(profile, prompt, systemPrompt);
            if (!response.empty()) {
                std::cout << response << std::endl;
            }
        }
    } else if (command == "search") {
        if (query.empty()) {
            std::cerr << "Error: Please provide a search query with --query" << std::endl;
            return 1;
        }

        std::vector<SearchResult> results;
        std::string searchMethod;

        if (searchType == "keyword") {
            results = executeKeywordSearch(profile, query);
            searchMethod = "Keyword Search";
        } else if (searchType == "vector") {
            results = executeVectorSearch(profile, query);
            searchMethod = "Vector Search";
        } else if (searchType == "hybrid") {
            results = executeHybridSearch(profile, query);
            searchMethod = "Hybrid Search";
        } else {
            std::cerr << fmt::format("Error: Unknown search type '{}'", searchType) << std::endl;
            return 1;
        }

        std::cout << fmt::format("{} Results for: {}\n", searchMethod, query) << std::endl;
        printSearchResults(results);
    } else if (command == "help" || command == "--help" || command == "-h") {
        printUsage(argv[0]);
        return 0;
    } else {
        std::cerr << fmt::format("Error: Unknown command '{}'", command) << std::endl;
        printUsage(argv[0]);
        return 1;
    }

    return 0;
}
