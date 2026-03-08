#pragma once

#include <string>
#include <vector>
#include "azai_common.hpp"

/**
 * @brief Execute a chat completion request to Azure OpenAI
 * @param profile User profile with Azure OpenAI credentials
 * @param prompt User prompt/question
 * @param systemPrompt Optional system message to set context
 * @return The response content from the model
 */
std::string executeOpenAI(const UserProfile& profile, const std::string& prompt, 
                         const std::string& systemPrompt = "");

/**
 * @brief Execute a chat completion with conversation history
 * @param profile User profile with Azure OpenAI credentials
 * @param messages Vector of ChatMessage objects representing conversation history
 * @return The response content from the model
 */
std::string executeOpenAIWithHistory(const UserProfile& profile, const std::vector<ChatMessage>& messages);

/**
 * @brief Stream a chat completion request (prints output as received)
 * @param profile User profile with Azure OpenAI credentials
 * @param prompt User prompt/question
 * @return True if successful, false otherwise
 */
bool streamOpenAI(const UserProfile& profile, const std::string& prompt);
