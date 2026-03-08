# Windows C++ Setup Guide - All Options

This document explains three ways to build the AzAI-CLI C++ project on Windows.

## Option 1: Automated PowerShell Script (Recommended)

**Easiest method** - Handles everything automatically.

### Prerequisites
- PowerShell 5.0+
- Administrator access (for CMake installation)

### Running the Script

```powershell
cd c:\Users\microai\AzAI-CLI\cpp-cli
Set-ExecutionPolicy -ExecutionPolicy Bypass -Scope Process
.\setup-windows.ps1
```

### What It Does
1. Checks/installs CMake
2. Sets up vcpkg environment
3. Installs C++ dependencies (nlohmann-json, cpr, fmt)
4. Builds the project with CMake
5. Shows you the executable location

### Troubleshooting
- If CMake download fails: Download manually from https://cmake.org/download/ then run:
  ```powershell
  .\setup-windows.ps1 -SkipCMake
  ```
- If build fails: Follow Option 2 (Visual Studio) instead

---

## Option 2: Visual Studio Direct Build

**Best if CMake isn't cooperating** - Build directly in Visual Studio IDE.

### Prerequisites
- Visual Studio 2019 or 2022 with C++ workload
- CMake plugin for Visual Studio (optional but helpful)

### Steps

1. **Generate Visual Studio Project Files:**
   ```powershell
   $env:VCPKG_ROOT = "C:\Users\microai\AzAI-CLI\vcpkg"
   cd C:\Users\microai\AzAI-CLI\cpp-cli
   mkdir build -Force
   cd build
   cmake .. -DCMAKE_TOOLCHAIN_FILE="$env:VCPKG_ROOT/scripts/buildsystems/cmake/vcpkg.cmake" -G "Visual Studio 17 2022" -A x64
   ```

2. **Open in Visual Studio:**
   - Open `C:\Users\microai\AzAI-CLI\cpp-cli\build\azai-cli.sln`
   - In Solution Explorer, right-click `azai-cli` project → "Set as Startup Project"
   - Build → Build Solution (Ctrl+Shift+B)
   - Wait for build to complete

3. **Run:**
   - If successful: executable is at `build\bin\Release\azai-cli.exe`
   - Or press F5 to debug

See [VISUAL-STUDIO.md](VISUAL-STUDIO.md) for more details.

---

## Option 3: Manual Command-Line Setup

**Most control** - Do each step manually if something goes wrong.

### Step 1: Install CMake
```powershell
# Using Chocolatey (requires admin)
choco install cmake -y

# OR download portable from:
# https://github.com/Kitware/CMake/releases
# Extract to C:\tools\cmake
# Add C:\tools\cmake\bin to PATH
```

### Step 2: Set Up Environment
```powershell
$env:VCPKG_ROOT = "C:\Users\microai\AzAI-CLI\vcpkg"
$env:PATH = "C:\tools\cmake\bin;$env:PATH"  # If using portable cmake
```

### Step 3: Install Dependencies
```powershell
cd $env:VCPKG_ROOT
.\vcpkg install nlohmann-json:x64-windows cpr:x64-windows fmt:x64-windows
```

**This can take 10-15 minutes** - Be patient!

### Step 4: Build
```powershell
cd C:\Users\microai\AzAI-CLI\cpp-cli
mkdir build
cd build
cmake .. -DCMAKE_TOOLCHAIN_FILE="$env:VCPKG_ROOT/scripts/buildsystems/cmake/vcpkg.cmake"
cmake --build . --config Release
```

See [WINDOWS-MANUAL-SETUP.md](WINDOWS-MANUAL-SETUP.md) for detailed troubleshooting.

---

## Comparing the Options

| Feature | Script | Visual Studio | Manual |
|---------|--------|---------------|--------|
| Ease of Use | ⭐⭐⭐⭐⭐ | ⭐⭐⭐⭐ | ⭐⭐⭐ |
| Control | ⭐⭐ | ⭐⭐⭐ | ⭐⭐⭐⭐⭐ |
| Requires Admin | Limited | Yes | Limited |
| GUI | No | Yes | No |
| Best For | Quick setup | Development/Debugging | Learning/Troubleshooting |

---

## After Build: Configuration

### 1. Copy Configuration File
```powershell
cd C:\Users\microai\AzAI-CLI\cpp-cli
Copy-Item config.json.example config.json
```

### 2. Add Your Azure Credentials
Edit `config.json` with:
- **endpoint**: Your Azure OpenAI endpoint URL
- **apiKey**: Your Azure OpenAI API key
- **deploymentName**: Your model deployment name (e.g., `gpt-35-turbo`)
- **searchEndpoint**: Your Azure AI Search endpoint
- **searchKey**: Your Search API key
- **searchIndex**: Your search index name

**Example:**
```json
{
  "endpoint": "https://my-resource.openai.azure.com/",
  "apiKey": "abc123def456...",
  "deploymentName": "gpt-35-turbo",
  "apiVersion": "2023-05-15",
  "searchEndpoint": "https://my-search.search.windows.net/",
  "searchKey": "xyz789...",
  "searchIndex": "my-documents"
}
```

### 3. Test the Application
```powershell
cd C:\Users\microai\AzAI-CLI\cpp-cli

# Test chat
.\build\bin\Release\azai-cli.exe chat --config config.json --prompt "What is AI?"

# Test search
.\build\bin\Release\azai-cli.exe search --query "machine learning" --type keyword
```

---

## Common Issues & Solutions

### "CMake not found"
- Download from https://cmake.org/download/
- Add `cmake` folder's `bin` directory to Windows PATH
- Restart PowerShell and try again

### "vcpkg install" times out
- vcpkg can take 10-30 minutes for first build
- Check internet connection
- Try installing one package at a time

### Build produces linking errors
- Ensure all vcpkg packages installed successfully
- Delete `build` folder and start over
- Make sure triplet is `x64-windows` 

### "VCPKG_ROOT not found"
Look in: `C:\Users\microai\AzAI-CLI\vcpkg\vcpkg.exe`

If it doesn't exist, re-run setup script from project root.

---

## Advanced: Using Different Generators

If you don't have Visual Studio 2022, change the CMake generator:

```powershell
# For Visual Studio 2019
cmake .. -G "Visual Studio 16 2019" -A x64

# For MinGW
cmake .. -G "Unix Makefiles"

# For Ninja
cmake .. -G "Ninja"

# For NMake (Visual C++)
cmake .. -G "NMake Makefiles"
```

---

## Getting Help

- **CMake issues?** → See [cmake.org/documentation](https://cmake.org/documentation/)
- **vcpkg issues?** → See [github.com/Microsoft/vcpkg](https://github.com/Microsoft/vcpkg)
- **Azure API issues?** → See [Azure docs](https://learn.microsoft.com/en-us/azure/ai-services/openai/)
- **Build details?** → See [BUILD.md](BUILD.md)
- **CLI usage?** → See [README.md](README.md)
