# AzAI-CLI: Azure AI C++ Command Line Tool

A high-performance C++ command-line interface for Azure OpenAI Chat and Azure AI Search integration.

## Features

- **Azure OpenAI Integration**: Chat with GPT models via Azure OpenAI API
- **Conversation History**: Maintain and manage conversation history
- **Azure AI Search**: Support for keyword, vector, and hybrid search
- **Vector Embeddings**: Automatic embedding generation for semantic search
- **Flexible CLI**: Easy-to-use command-line interface
- **JSON Configuration**: Simple JSON-based configuration management
- **Cross-Platform**: Builds on Windows, macOS, and Linux

## Prerequisites

- C++17 compatible compiler
- CMake 3.20 or higher
- vcpkg for dependency management
- Azure OpenAI and AI Search resources

## Dependencies

- **nlohmann/json**: Modern C++ JSON library
- **cpr**: C++ HTTP client library
- **fmt**: Modern C++ formatting library

## Building

### Option 1: Using vcpkg (Recommended)

```bash
# Clone the repository
cd cpp-cli

# Install dependencies with vcpkg
vcpkg install

# Create build directory
mkdir build
cd build

# Configure and build
cmake .. -DCMAKE_TOOLCHAIN_FILE=[vcpkg root]/scripts/buildsystems/cmake/vcpkg.cmake
cmake --build . --config Release
```

### Option 2: Manual Build (If vcpkg packages are already installed)

```bash
mkdir build
cd build
cmake ..
cmake --build . --config Release
```

### Windows (Development)

```powershell
mkdir build
cd build
cmake ..
cmake --build . --config Release
```

## Configuration

1. Copy the example configuration:
   ```bash
   cp config.json.example config.json
   ```

2. Edit `config.json` with your Azure credentials:
   ```json
   {
     "endpoint": "https://your-resource.openai.azure.com/",
     "apiKey": "your-api-key",
     "deploymentName": "gpt-35-turbo",
     "apiVersion": "2023-05-15",
     "searchEndpoint": "https://your-search.search.windows.net/",
     "searchKey": "your-search-key",
     "searchIndex": "your-index-name",
     "embeddingDeployment": "embedding",
     "embeddingModel": "text-embedding-ada-002"
   }
   ```

## Usage

### Chat Commands

**Simple chat:**
```bash
./azai-cli chat --config config.json --prompt "What is machine learning?"
```

**Chat with system prompt:**
```bash
./azai-cli chat --config config.json --prompt "Explain quantum computing" --system "You are an expert in physics"
```

**Chat with conversation history:**
```bash
./azai-cli chat --config config.json --history conversation.json
```

### Search Commands

**Keyword search:**
```bash
./azai-cli search --config config.json --query "machine learning" --type keyword
```

**Vector search (semantic):**
```bash
./azai-cli search --config config.json --query "artificial intelligence" --type vector
```

**Hybrid search (keyword + vector):**
```bash
./azai-cli search --config config.json --query "deep learning" --type hybrid
```

### Help

```bash
./azai-cli help
./azai-cli --help
./azai-cli chat --help
```

## API Reference

### Chat Functionality

#### `executeOpenAI()`
Executes a simple chat completion request.

```cpp
std::string executeOpenAI(const UserProfile& profile, 
                         const std::string& prompt, 
                         const std::string& systemPrompt = "");
```

#### `executeOpenAIWithHistory()`
Executes a chat completion with full conversation history.

```cpp
std::string executeOpenAIWithHistory(const UserProfile& profile, 
                                     const std::vector<ChatMessage>& messages);
```

### Search Functionality

#### `executeKeywordSearch()`
Performs keyword-based search.

```cpp
std::vector<SearchResult> executeKeywordSearch(const UserProfile& profile, 
                                               const std::string& query, 
                                               int top = 5);
```

#### `executeVectorSearch()`
Performs vector/semantic search.

```cpp
std::vector<SearchResult> executeVectorSearch(const UserProfile& profile,
                                              const std::string& query,
                                              const std::string& vectorField = "contentVector",
                                              int top = 5);
```

#### `executeHybridSearch()`
Performs combined keyword and vector search.

```cpp
std::vector<SearchResult> executeHybridSearch(const UserProfile& profile,
                                              const std::string& query,
                                              const std::string& vectorField = "contentVector",
                                              int top = 5);
```

### Utility Functions

#### `getEmbeddings()`
Generates vector embeddings for text.

```cpp
std::vector<float> getEmbeddings(const std::string& query, const UserProfile& profile);
```

## Data Structures

### UserProfile
Stores Azure service credentials and endpoints.

```cpp
struct UserProfile {
    std::string endpoint;              // Azure OpenAI endpoint
    std::string apiKey;                // Azure OpenAI API key
    std::string deploymentName;        // Model deployment name
    std::string apiVersion;            // API version
    std::string searchEndpoint;        // Azure AI Search endpoint
    std::string searchKey;             // Azure Search API key
    std::string searchIndex;           // Search index name
    std::string embeddingDeployment;   // Embedding model deployment
    std::string embeddingModel;        // Embedding model name
};
```

### ChatMessage
Represents a single chat message.

```cpp
struct ChatMessage {
    std::string role;      // "system", "user", or "assistant"
    std::string content;   // Message content
};
```

### SearchResult
Represents a search result.

```cpp
struct SearchResult {
    std::string id;       // Document ID
    std::string content;  // Document content
    float score;          // Relevance score
};
```

## File Structure

```
cpp-cli/
├── CMakeLists.txt
├── vcpkg.json
├── config.json.example
├── README.md
├── include/
│   ├── azai_common.hpp     # Common structures and utilities
│   ├── openai.hpp          # OpenAI chat functions
│   └── search.hpp          # Search functions
├── src/
│   ├── main.cpp            # CLI entry point
│   ├── azai_common.cpp     # Common implementations
│   ├── openai.cpp          # OpenAI chat implementations
│   └── search.cpp          # Search implementations
└── build/                  # Build output directory
```

## Examples

### Conversation Example

Create a conversation history file:
```json
[
  {"role": "user", "content": "What is an algorithm?"},
  {"role": "assistant", "content": "An algorithm is a step-by-step procedure..."},
  {"role": "user", "content": "Can you give me an example?"}
]
```

Continue the conversation:
```bash
./azai-cli chat --config config.json --history conversation.json
```

### Search Example

Perform a hybrid search for documents:
```bash
./azai-cli search --config config.json --query "neural networks" --type hybrid
```

## Troubleshooting

### Build Issues

**Missing dependencies:**
```bash
vcpkg install nlohmann-json cpr fmt
```

**CMake configuration errors:**
- Ensure vcpkg toolchain file is specified correctly
- Check CMake version is 3.20 or higher

### Runtime Errors

**Configuration not found:**
- Verify `config.json` exists in the correct directory
- Check all required fields are present

**API errors:**
- Verify Azure credentials are correct
- Check API key has appropriate permissions
- Ensure endpoints are accessible

**Search failures:**
- Verify search index exists
- Check index name matches configuration
- Ensure embedding deployment is configured for vector search

## Performance Considerations

- Chat responses are limited to 800 tokens by default
- Vector search k parameter is set to 5 results by default
- Use hybrid search for best balance of speed and relevance

## Security

- **Never commit** `config.json` with real credentials
- Use environment variables in production
- Rotate API keys regularly
- Use HTTPS for all API calls (enabled by default)

## Contributing

Contributions are welcome! Please:
1. Follow C++17 best practices
2. Add documentation for new features
3. Test on multiple platforms
4. Update README with usage examples

## License

[See LICENSE file in parent directory]

## Support

For issues related to:
- **Azure OpenAI**: Check [Azure OpenAI documentation](https://learn.microsoft.com/en-us/azure/ai-services/openai/)
- **Azure AI Search**: Check [Azure Search documentation](https://learn.microsoft.com/en-us/azure/search/)
- **This tool**: Check GitHub issues

## Version History

### 1.0.0
- Initial release
- Chat completion with Azure OpenAI
- Keyword, vector, and hybrid search
- Conversation history support
- JSON configuration management
