#include "azai_common.hpp"
#include <iostream>
#include <fstream>
#include <cpr/cpr.h>
#include <fmt/core.h>

bool UserProfile::loadFromConfig(const std::string& configPath) {
    try {
        std::ifstream file(configPath);
        if (!file.is_open()) {
            std::cerr << fmt::format("Error: Could not open config file: {}", configPath) << std::endl;
            return false;
        }

        json config;
        file >> config;

        // Load Azure OpenAI configuration
        endpoint = config.value("endpoint", "");
        apiKey = config.value("apiKey", "");
        deploymentName = config.value("deploymentName", "");
        apiVersion = config.value("apiVersion", "2023-05-15");

        // Load Azure AI Search configuration
        searchEndpoint = config.value("searchEndpoint", "");
        searchKey = config.value("searchKey", "");
        searchIndex = config.value("searchIndex", "");

        // Load Embedding configuration
        embeddingDeployment = config.value("embeddingDeployment", "");
        embeddingModel = config.value("embeddingModel", "text-embedding-ada-002");

        return validate();
    } catch (const std::exception& e) {
        std::cerr << fmt::format("Error loading config: {}", e.what()) << std::endl;
        return false;
    }
}

bool UserProfile::validate() const {
    if (endpoint.empty() || apiKey.empty() || deploymentName.empty()) {
        std::cerr << "Error: Missing required Azure OpenAI configuration" << std::endl;
        return false;
    }
    if (searchEndpoint.empty() || searchKey.empty() || searchIndex.empty()) {
        std::cerr << "Error: Missing required Azure AI Search configuration" << std::endl;
        return false;
    }
    return true;
}

std::vector<float> getEmbeddings(const std::string& query, const UserProfile& profile) {
    std::string url = fmt::format(
        "{}/openai/deployments/{}/embeddings?api-version={}",
        profile.endpoint,
        profile.embeddingDeployment,
        profile.apiVersion
    );

    json payload = {
        {"input", query},
        {"model", profile.embeddingModel}
    };

    cpr::Response r = cpr::Post(
        cpr::Url{url},
        cpr::Header{
            {"Content-Type", "application/json"},
            {"api-key", profile.apiKey}
        },
        cpr::Body{payload.dump()}
    );

    std::vector<float> embedding;
    if (r.status_code == 200) {
        auto responseJson = json::parse(r.text);
        auto data = responseJson["data"];
        if (!data.empty()) {
            embedding = data[0]["embedding"].get<std::vector<float>>();
        }
    } else {
        std::cerr << fmt::format("Error getting embeddings: {} - {}", r.status_code, r.text) << std::endl;
    }
    return embedding;
}

void printOutput(const std::string& message, bool isError) {
    if (isError) {
        std::cerr << message << std::endl;
    } else {
        std::cout << message << std::endl;
    }
}

json loadJsonFile(const std::string& filePath) {
    try {
        std::ifstream file(filePath);
        if (!file.is_open()) {
            throw std::runtime_error(fmt::format("Could not open file: {}", filePath));
        }
        json data;
        file >> data;
        return data;
    } catch (const std::exception& e) {
        std::cerr << fmt::format("Error loading JSON file: {}", e.what()) << std::endl;
        return json::object();
    }
}

bool saveJsonFile(const std::string& filePath, const json& data) {
    try {
        std::ofstream file(filePath);
        if (!file.is_open()) {
            throw std::runtime_error(fmt::format("Could not open file for writing: {}", filePath));
        }
        file << data.dump(4);
        return true;
    } catch (const std::exception& e) {
        std::cerr << fmt::format("Error saving JSON file: {}", e.what()) << std::endl;
        return false;
    }
}
