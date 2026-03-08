# AzAI-CLI C++ Build Guide

Complete guide for building the AzAI-CLI C++ application on different platforms.

## Table of Contents

1. [Prerequisites](#prerequisites)
2. [Setup](#setup)
3. [Windows Instructions](#windows-instructions)
4. [macOS Instructions](#macos-instructions)
5. [Linux Instructions](#linux-instructions)
6. [Troubleshooting](#troubleshooting)

## Prerequisites

### Required
- C++17 compatible compiler
- CMake 3.20 or higher
- Git
- vcpkg (for dependency management)

### Optional
- Visual Studio Code with C++ extension
- Visual Studio 2019+ (Windows)
- Xcode Command Line Tools (macOS)

## Setup

### 1. Clone vcpkg (if not already installed)

```bash
git clone https://github.com/Microsoft/vcpkg.git
cd vcpkg
./bootstrap-vcpkg.bat    # Windows
./bootstrap-vcpkg.sh     # macOS/Linux
cd ..
```

### 2. Set vcpkg environment variables

**Windows (PowerShell):**
```powershell
$env:VCPKG_ROOT = "C:\path\to\vcpkg"
```

**Windows (Command Prompt):**
```cmd
set VCPKG_ROOT=C:\path\to\vcpkg
```

**macOS/Linux (bash):**
```bash
export VCPKG_ROOT=/path/to/vcpkg
```

## Windows Instructions

### Using Visual Studio 2022

```bash
# Navigate to cpp-cli directory
cd cpp-cli

# Install dependencies
vcpkg install nlohmann-json cpr fmt --triplet x64-windows

# Create build directory
mkdir build
cd build

# Configure CMake
cmake .. -DCMAKE_TOOLCHAIN_FILE=$env:VCPKG_ROOT/scripts/buildsystems/cmake/vcpkg.cmake -G "Visual Studio 17 2022"

# Build
cmake --build . --config Release

# Executable will be in: build/bin/Release/azai-cli.exe
```

### Using MinGW

```bash
cd cpp-cli

# Install dependencies for MinGW
vcpkg install nlohmann-json cpr fmt --triplet x64-mingw-dynamic

# Create build directory
mkdir build
cd build

# Configure CMake
cmake .. -DCMAKE_TOOLCHAIN_FILE=$env:VCPKG_ROOT/scripts/buildsystems/cmake/vcpkg.cmake -G "MinGW Makefiles"

# Build
cmake --build . --config Release

# Executable: build/bin/azai-cli.exe
```

### Using CMake GUI

1. Open CMake GUI
2. Set source code path to `cpp-cli`
3. Set build path to `cpp-cli/build`
4. Configure with appropriate generator
5. Set `CMAKE_TOOLCHAIN_FILE` to vcpkg.cmake
6. Generate and build in Visual Studio

## macOS Instructions

### Using Clang

```bash
# Navigate to cpp-cli directory
cd cpp-cli

# Install dependencies
vcpkg install nlohmann-json cpr fmt --triplet x64-osx

# Create build directory
mkdir build
cd build

# Configure CMake
cmake .. \
  -DCMAKE_TOOLCHAIN_FILE=$VCPKG_ROOT/scripts/buildsystems/cmake/vcpkg.cmake \
  -DCMAKE_BUILD_TYPE=Release \
  -G "Unix Makefiles"

# Build
cmake --build . --config Release

# Run the executable
./bin/azai-cli --help
```

### Using Homebrew

```bash
# Install dependencies via Homebrew
brew install nlohmann-json cpr fmt cmake

# Create build directory
mkdir build
cd build

# Configure and build
cmake .. -DCMAKE_BUILD_TYPE=Release
cmake --build . --config Release

# Run
./bin/azai-cli --help
```

## Linux Instructions

### Ubuntu/Debian

```bash
# Install system dependencies
sudo apt-get update
sudo apt-get install build-essential cmake git curl

# Navigate to cpp-cli
cd cpp-cli

# Install C++ libraries via vcpkg
vcpkg install nlohmann-json cpr fmt --triplet x64-linux

# Create build directory
mkdir build
cd build

# Configure CMake
cmake .. \
  -DCMAKE_TOOLCHAIN_FILE=$VCPKG_ROOT/scripts/buildsystems/cmake/vcpkg.cmake \
  -DCMAKE_BUILD_TYPE=Release

# Build
cmake --build .

# Run
./bin/azai-cli --help
```

### Fedora/CentOS/RHEL

```bash
# Install system dependencies
sudo dnf groupinstall "Development Tools"
sudo dnf install cmake git curl

# Follow Ubuntu instructions from step 3 onwards
```

### Alpine Linux

```bash
# Install system dependencies
apk add build-base cmake git curl

# Proceed with vcpkg installation
```

## Building with Custom Compiler

### GCC

```bash
cd cpp-cli
mkdir build
cd build

cmake .. \
  -DCMAKE_CXX_COMPILER=g++ \
  -DCMAKE_C_COMPILER=gcc \
  -DCMAKE_TOOLCHAIN_FILE=$VCPKG_ROOT/scripts/buildsystems/cmake/vcpkg.cmake \
  -DCMAKE_BUILD_TYPE=Release

cmake --build .
```

### Clang

```bash
cd cpp-cli
mkdir build
cd build

cmake .. \
  -DCMAKE_CXX_COMPILER=clang++ \
  -DCMAKE_C_COMPILER=clang \
  -DCMAKE_TOOLCHAIN_FILE=$VCPKG_ROOT/scripts/buildsystems/cmake/vcpkg.cmake \
  -DCMAKE_BUILD_TYPE=Release

cmake --build .
```

## Installation

### Install Locally

```bash
cd build
cmake --install . --prefix ~/azai-cli
```

### Install System-wide (Linux/macOS)

```bash
cd build
sudo cmake --install . --prefix /usr/local
```

Then use from anywhere:
```bash
azai-cli --help
```

## Quick Start After Build

1. **Copy example config:**
   ```bash
   cp config.json.example config.json
   ```

2. **Edit config.json** with your Azure credentials

3. **Test the CLI:**
   ```bash
   ./bin/azai-cli chat --config config.json --prompt "Hello!"
   ```

4. **Test search:**
   ```bash
   ./bin/azai-cli search --config config.json --query "test" --type keyword
   ```

## Troubleshooting

### CMake not found
```bash
# macOS
brew install cmake

# Ubuntu/Debian
sudo apt-get install cmake

# Windows - Download from https://cmake.org/download/
```

### vcpkg not found
Set the toolchain file explicitly:
```bash
cmake .. -DCMAKE_TOOLCHAIN_FILE=/path/to/vcpkg/scripts/buildsystems/cmake/vcpkg.cmake
```

### Compiler not found
```bash
# macOS
xcode-select --install

# Ubuntu/Debian
sudo apt-get install build-essential

# Fedora
sudo dnf groupinstall "Development Tools"
```

### Dependencies not installing
```bash
# Force reinstall
vcpkg install nlohmann-json cpr fmt --force

# Or use pre-built binaries
vcpkg install nlohmann-json cpr fmt --binary
```

### Build fails with linking errors
- Ensure all dependencies are installed
- Verify vcpkg triplet matches your platform
- Check that CMAKE_TOOLCHAIN_FILE is set correctly

### DLL/Shared library errors (Windows)
Use static linking in vcpkg:
```bash
vcpkg install nlohmann-json cpr fmt --triplet x64-windows-static
```

## Performance Optimization

For optimized builds:

```bash
cmake .. \
  -DCMAKE_BUILD_TYPE=Release \
  -DCMAKE_CXX_FLAGS="-O3 -march=native"

cmake --build . --config Release
```

## Development Build

For faster compilation during development:

```bash
cmake .. -DCMAKE_BUILD_TYPE=Debug

cmake --build . --config Debug

# Run with debug symbols
gdb ./bin/azai-cli
```

## CI/CD Integration

### GitHub Actions Example

```yaml
name: Build

on: [push, pull_request]

jobs:
  build:
    runs-on: ${{ matrix.os }}
    strategy:
      matrix:
        os: [ubuntu-latest, macos-latest, windows-latest]
    
    steps:
    - uses: actions/checkout@v2
    
    - name: Install dependencies
      run: |
        # Ubuntu
        if [[ "$RUNNER_OS" == "Linux" ]]; then
          sudo apt-get update
          sudo apt-get install -y build-essential cmake
        fi
        # macOS
        if [[ "$RUNNER_OS" == "macOS" ]]; then
          brew install cmake
        fi
    
    - name: Setup vcpkg
      run: |
        git clone https://github.com/Microsoft/vcpkg.git
        cd vcpkg
        ./bootstrap-vcpkg.sh || ./bootstrap-vcpkg.bat
    
    - name: Build
      run: |
        cd cpp-cli
        mkdir build && cd build
        cmake .. -DCMAKE_TOOLCHAIN_FILE=../../vcpkg/scripts/buildsystems/cmake/vcpkg.cmake
        cmake --build . --config Release
```

## Additional Resources

- [CMake Documentation](https://cmake.org/documentation/)
- [vcpkg Documentation](https://github.com/Microsoft/vcpkg/blob/master/README.md)
- [Azure OpenAI Documentation](https://learn.microsoft.com/en-us/azure/ai-services/openai/)
- [Azure AI Search Documentation](https://learn.microsoft.com/en-us/azure/search/)
