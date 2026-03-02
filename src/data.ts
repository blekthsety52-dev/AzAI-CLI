export const projectData = {
  name: "AzAI-CLI",
  language: "C++ 20",
  type: "CLI Tool",
  description: "A high-performance, native command-line interface for interacting with Azure AI Services. AzAI-CLI allows developers and system administrators to integrate Azure's OpenAI, Speech, and AI Search capabilities directly into shell scripts and terminal workflows without the overhead of interpreted languages. It leverages the Azure SDK for C++ for authentication and provides a streamlined piping experience for JSON outputs.",
  keyFeatures: [
    {
      title: "Modular Command Structure",
      description: "Distinct subcommands for 'speech', 'openai', and 'search'.",
      icon: "Layers"
    },
    {
      title: "Speech-to-Text & Text-to-Speech",
      description: "Direct piping of audio files to Azure Speech Services and synthesis of text to .wav output.",
      icon: "Mic"
    },
    {
      title: "RAG-Ready Search",
      description: "Query Azure AI Search indexes with hybrid retrieval (vector + keyword) directly from the terminal.",
      icon: "Search"
    },
    {
      title: "OpenAI Chat Integration",
      description: "Interactive or one-shot prompt interface for deployed Azure OpenAI models.",
      icon: "MessageSquare"
    },
    {
      title: "Secure Configuration",
      description: "Encrypted local storage for API endpoints and keys using platform-specific keychains or secure config files.",
      icon: "Shield"
    },
    {
      title: "JSON-First Output",
      description: "All command outputs are formatted as minified or pretty-printed JSON for easy integration with tools like 'jq'.",
      icon: "Code"
    }
  ],
  performanceRequirements: "The tool must have a startup time under 50ms. Network operations must be asynchronous to handle streaming audio data efficiently. Memory usage should not exceed 50MB during standard text operations. Binary size should be minimized via static linking for portability.",
  testingStrategy: "Unit testing via Google Test for argument parsing, configuration management, and JSON serialization logic. Integration testing using mocked HTTP servers to simulate Azure API responses. End-to-End testing in a CI/CD pipeline against a sandbox Azure Resource Group.",
  deploymentPlan: "Code compilation via CMake. Dependency management using vcpkg. CI/CD pipelines (GitHub Actions) to generate static binaries for Linux (Alpine/Debian), macOS (Universal), and Windows (MSVC). Distribution via Homebrew tap and raw binary releases.",
  recommendedLibraries: [
    "CLI11 (Command Line Parser)",
    "nlohmann/json (JSON Serialization)",
    "Azure SDK for C++ (Identity, Core)",
    "CPR (C++ Requests - Wrapper around libcurl)",
    "spdlog (Fast Logging)",
    "fmt (String formatting)",
    "OpenSSL (Security)"
  ],
  architectureOverview: "The application follows a Clean Architecture approach. The 'Core' layer handles argument parsing and configuration loading. The 'Services' layer contains specific adapters for Azure OpenAI, Speech, and Search, implementing a common 'IService' interface. The 'Network' layer handles HTTP/2 connections, retries, and authentication (Bearer tokens/API Keys). The 'Presentation' layer ensures output is formatted strictly as JSON or raw binary (for audio) based on user flags.",
  folderStructure: `AzAI-CLI/
├── CMakeLists.txt
├── vcpkg.json
├── README.md
├── src/
│   ├── main.cpp
│   ├── config/
│   │   ├── ConfigManager.hpp
│   │   └── ConfigManager.cpp
│   ├── commands/
│   │   ├── CommandRegistry.hpp
│   │   ├── OpenAICommand.cpp
│   │   ├── SpeechCommand.cpp
│   │   └── SearchCommand.cpp
│   ├── services/
│   │   ├── AzureAuth.hpp
│   │   ├── AzureAuth.cpp
│   │   └── HttpClient.hpp
│   └── utils/
│       └── JsonUtils.hpp
├── include/
│   └── azai_common.hpp
└── tests/
    ├── unit/
    │   ├── TestConfig.cpp
    │   └── TestParser.cpp
    └── integration/
        └── TestAzureConnectivity.cpp`,
  codeSnippets: [
    {
      title: "Main CLI Entry Point & Configuration",
      language: "cpp",
      description: "Initializes the CLI11 application, defines global flags, and routes subcommands (openai, speech) to their respective handlers using lambda callbacks.",
      code: `#include <CLI/CLI.hpp>
#include <iostream>
#include "commands/CommandRegistry.hpp"
#include "config/ConfigManager.hpp"

int main(int argc, char** argv) {
    CLI::App app{"AzAI-CLI: Native Azure AI Interface"};
    app.require_subcommand(1);

    // Global flags
    bool verbose = false;
    app.add_flag("-v,--verbose", verbose, "Enable verbose logging");

    // Config Manager
    ConfigManager config;
    
    // Register Subcommands
    auto* openai = app.add_subcommand("openai", "Interact with Azure OpenAI");
    std::string prompt;
    openai->add_option("-p,--prompt", prompt, "The prompt to send")->required();
    openai->callback([&]() {
        CommandRegistry::executeOpenAI(config.getProfile(), prompt);
    });

    auto* speech = app.add_subcommand("speech", "Azure Speech Services");
    auto* tts = speech->add_subcommand("tts", "Text to Speech");
    std::string text_input, output_file;
    tts->add_option("-t,--text", text_input, "Text to synthesize")->required();
    tts->add_option("-o,--output", output_file, "Output .wav file path")->required();
    tts->callback([&]() {
        CommandRegistry::executeTTS(config.getProfile(), text_input, output_file);
    });

    try {
        app.parse(argc, argv);
    } catch (const CLI::ParseError &e) {
        return app.exit(e);
    }

    return 0;
}`
    },
    {
      title: "Azure OpenAI Service Handler",
      language: "cpp",
      description: "Demonstrates sending a chat completion request to Azure OpenAI using the CPR library for HTTP and nlohmann/json for payload formatting. It prints raw response text to stdout for piping.",
      code: `#include <cpr/cpr.h>
#include <nlohmann/json.hpp>
#include <fmt/core.h>
#include "azai_common.hpp"

using json = nlohmann::json;

void executeOpenAI(const UserProfile& profile, const std::string& prompt) {
    std::string endpoint = fmt::format("{}/openai/deployments/{}/chat/completions?api-version=2023-05-15", 
                                       profile.endpoint, profile.deploymentName);

    json payload = {
        {"messages", {{{"role", "user"}, {"content", prompt}}}},
        {"max_tokens", 800},
        {"temperature", 0.7}
    };

    cpr::Response r = cpr::Post(
        cpr::Url{endpoint},
        cpr::Header{
            {"Content-Type", "application/json"},
            {"api-key", profile.apiKey}
        },
        cpr::Body{payload.dump()}
    );

    if (r.status_code == 200) {
        auto responseJson = json::parse(r.text);
        // Output only the content for easy piping
        std::cout << responseJson["choices"][0]["message"]["content"].get<std::string>() << std::endl;
    } else {
        std::cerr << "Error: " << r.status_code << " - " << r.text << std::endl;
    }
}`
    },
    {
      title: "Azure AI Search Query Logic",
      language: "cpp",
      description: "Handles querying the Azure AI Search index. It demonstrates logic branching between standard full-text search and vector search, formatting the request body accordingly.",
      code: `#include <cpr/cpr.h>
#include <nlohmann/json.hpp>

void executeSearch(const UserProfile& profile, const std::string& query, bool vectorSearch) {
    // Setup URL for Azure AI Search
    std::string url = profile.searchEndpoint + "/indexes/" + profile.searchIndex + "/docs/search?api-version=2023-11-01";
    
    nlohmann::json body;
    if (vectorSearch) {
        // Assuming an embedding function exists to vectorize the query first
        std::vector<float> vector = getEmbeddings(query, profile);
        body = {
            {"vectorQueries", {{
                {"kind", "vector"},
                {"vector", vector},
                {"fields", "contentVector"},
                {"k", 5}
            }}}
        };
    } else {
        body = {{"search", query}, {"top", 5}};
    }

    cpr::Response r = cpr::Post(
        cpr::Url{url},
        cpr::Header{
            {"Content-Type", "application/json"},
            {"api-key", profile.searchKey}
        },
        cpr::Body{body.dump()}
    );

    if (r.status_code == 200) {
        std::cout << nlohmann::json::parse(r.text).dump(4) << std::endl;
    } else {
        std::cerr << "Search Failed: " << r.text << std::endl;
    }
}`
    }
  ]
};
