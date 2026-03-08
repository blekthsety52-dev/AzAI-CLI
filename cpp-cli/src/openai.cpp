#include "openai.hpp"
#include <cpr/cpr.h>
#include <fmt/core.h>
#include <iostream>

std::string executeOpenAI(const UserProfile& profile, const std::string& prompt, 
                         const std::string& systemPrompt) {
    std::string url = fmt::format(
        "{}/openai/deployments/{}/chat/completions?api-version={}",
        profile.endpoint,
        profile.deploymentName,
        profile.apiVersion
    );

    std::vector<ChatMessage> messages;
    if (!systemPrompt.empty()) {
        messages.push_back(ChatMessage("system", systemPrompt));
    }
    messages.push_back(ChatMessage("user", prompt));

    return executeOpenAIWithHistory(profile, messages);
}

std::string executeOpenAIWithHistory(const UserProfile& profile, const std::vector<ChatMessage>& messages) {
    std::string url = fmt::format(
        "{}/openai/deployments/{}/chat/completions?api-version={}",
        profile.endpoint,
        profile.deploymentName,
        profile.apiVersion
    );

    // Build messages array
    json messagesArray = json::array();
    for (const auto& msg : messages) {
        messagesArray.push_back(msg.toJson());
    }

    json payload = {
        {"messages", messagesArray},
        {"max_tokens", 800},
        {"temperature", 0.7},
        {"top_p", 0.95},
        {"frequency_penalty", 0},
        {"presence_penalty", 0}
    };

    cpr::Response r = cpr::Post(
        cpr::Url{url},
        cpr::Header{
            {"Content-Type", "application/json"},
            {"api-key", profile.apiKey}
        },
        cpr::Body{payload.dump()}
    );

    std::string result;
    if (r.status_code == 200) {
        try {
            auto responseJson = json::parse(r.text);
            result = responseJson["choices"][0]["message"]["content"].get<std::string>();
        } catch (const std::exception& e) {
            std::cerr << fmt::format("Error parsing OpenAI response: {}", e.what()) << std::endl;
        }
    } else {
        std::cerr << fmt::format("OpenAI Error: {} - {}", r.status_code, r.text) << std::endl;
    }

    return result;
}

bool streamOpenAI(const UserProfile& profile, const std::string& prompt) {
    std::string url = fmt::format(
        "{}/openai/deployments/{}/chat/completions?api-version={}",
        profile.endpoint,
        profile.deploymentName,
        profile.apiVersion
    );

    json payload = {
        {"messages", {{{"role", "user"}, {"content", prompt}}}},
        {"max_tokens", 800},
        {"temperature", 0.7},
        {"stream", true}
    };

    cpr::Response r = cpr::Post(
        cpr::Url{url},
        cpr::Header{
            {"Content-Type", "application/json"},
            {"api-key", profile.apiKey}
        },
        cpr::Body{payload.dump()}
    );

    if (r.status_code == 200) {
        // Process streaming response
        // Note: CPR doesn't have built-in streaming support, so we process the full response
        std::cout << r.text << std::endl;
        return true;
    } else {
        std::cerr << fmt::format("Stream Error: {} - {}", r.status_code, r.text) << std::endl;
        return false;
    }
}
