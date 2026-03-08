cmake_minimum_required(VERSION 3.20)
project(azai-cli VERSION 1.0.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Set output directories
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

# Find required packages
find_package(nlohmann_json 3.2.0 REQUIRED)
find_package(cpr REQUIRED)
find_package(fmt REQUIRED)

# Main executable
add_executable(azai-cli
    src/main.cpp
    src/azai_common.cpp
    src/openai.cpp
    src/search.cpp
)

target_include_directories(azai-cli PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/include
)

target_link_libraries(azai-cli PRIVATE
    nlohmann_json::nlohmann_json
    cpr::cpr
    fmt::fmt
)

# Optional: Set compiler flags for better warnings and optimizations
if(MSVC)
    target_compile_options(azai-cli PRIVATE /W4)
else()
    target_compile_options(azai-cli PRIVATE -Wall -Wextra -Wpedantic)
endif()

# Installation
install(TARGETS azai-cli DESTINATION bin)
install(FILES ${CMAKE_CURRENT_SOURCE_DIR}/include/azai_common.hpp 
               ${CMAKE_CURRENT_SOURCE_DIR}/include/openai.hpp 
               ${CMAKE_CURRENT_SOURCE_DIR}/include/search.hpp 
        DESTINATION include)
