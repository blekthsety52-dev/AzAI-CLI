# Windows Setup Instructions - Manual Steps

If the automated setup script encounters issues, follow these manual steps:

## Step 1: Download and Install CMake

**Option A: Using Chocolatey (Easiest)**
```powershell
# Install Chocolatey first if not installed
iex ((New-Object System.Net.WebClient).DownloadString('https://community.chocolatey.org/install.ps1'))

# Then install CMake
choco install cmake -y
```

**Option B: Download Portable CMake**
1. Download from: https://github.com/Kitware/CMake/releases/download/v3.29.6/cmake-3.29.6-windows-x86_64.zip
2. Extract to `C:\tools\cmake`
3. Add `C:\tools\cmake\bin` to your system PATH (or just to the terminal session)

**Option C: Download MSI Installer**
1. Go to https://cmake.org/download/
2. Download "Windows x86_64 Installer" (latest)
3. Run the installer
4. Choose "Add CMake to the system PATH" during installation

**Verify CMake is installed:**
```powershell
cmake --version
```

## Step 2: Set Environment Variables

```powershell
# Set vcpkg root
$env:VCPKG_ROOT = "C:\Users\microai\AzAI-CLI\vcpkg"

# If using portable CMake from Option B, add to PATH
$env:PATH = "C:\tools\cmake\bin;$env:PATH"

# Use system binaries to avoid vcpkg trying to download tools
$env:VCPKG_FORCE_SYSTEM_BINARIES = 1
```

## Step 3: Install Dependencies with vcpkg

```powershell
cd $env:VCPKG_ROOT

# Clean up previous failed attempts
Remove-Item .\downloads\tools -Recurse -Force -ErrorAction SilentlyContinue

# Install packages
.\vcpkg install nlohmann-json:x64-windows cpr:x64-windows fmt:x64-windows
```

If you get errors about `x64-windows` triplet, try:
```powershell
.\vcpkg install nlohmann-json cpr fmt
```

## Step 4: Build the Project

```powershell
cd C:\Users\microai\AzAI-CLI\cpp-cli

# Create build directory
mkdir build -Force
cd build

# Configure CMake
cmake .. -DCMAKE_TOOLCHAIN_FILE="$env:VCPKG_ROOT/scripts/buildsystems/cmake/vcpkg.cmake" -G "Visual Studio 17 2022" -A x64

# Build
cmake --build . --config Release
```

If you don't have Visual Studio 2022, use one of these generators instead:
- `"Visual Studio 16 2019"` (for Visual Studio 2019)
- `"Unix Makefiles"` (if using MinGW/MSYS2)  
- `"Ninja"` (if Ninja is installed)

## Step 5: Run the Application

```powershell
# Executable location
C:\Users\microai\AzAI-CLI\cpp-cli\build\bin\Release\azai-cli.exe

# Test it
.\build\bin\Release\azai-cli.exe --help
```

## Troubleshooting

### "cmake: command not found"
- CMake is not installed or not in PATH
- Check it's properly installed: `cmake --version`
- Add the CMake bin directory to PATH if present

### "vcpkg: command not found"
- Use full path: `C:\Users\microai\AzAI-CLI\vcpkg\vcpkg.exe install ...`
- Or set VCPKG_ROOT: `$env:VCPKG_ROOT = "C:\Users\microai\AzAI-CLI\vcpkg"`

### "Library not found" errors
- vcpkg dependencies weren't installed
- Run: `.\vcpkg install nlohmann-json cpr fmt --triplet x64-windows`
- Wait for it to complete (may take 10+ minutes for cpr)

### "Cannot open include file"
- Ensure CMakeLists.txt references correct vcpkg toolchain
- Check: `-DCMAKE_TOOLCHAIN_FILE="$env:VCPKG_ROOT/scripts/buildsystems/cmake/vcpkg.cmake"`

### Build still fails?
Try Visual Studio direct build (see VISUAL-STUDIO.md for steps)

## Next Steps After Successful Build

1. **Edit configuration:**
   ```powershell
   # Copy example to actual config
   Copy-Item config.json.example config.json
   
   # Edit with your credentials
   notepad config.json
   ```

2. **Test basic chat:**
   ```powershell
   .\build\bin\Release\azai-cli.exe chat --config config.json --prompt "Hello, who are you?"
   ```

3. **Test search:**
   ```powershell
   .\build\bin\Release\azai-cli.exe search --config config.json --query "test" --type keyword
   ```
