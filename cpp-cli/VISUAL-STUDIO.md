# Visual Studio Direct Build Instructions

If you want to build directly using Visual Studio without CMake, follow these steps:

## Prerequisites

- Visual Studio 2019 or 2022 with C++ workload
- VCPKG installed and bootstrapped

## Step 1: Generate Visual Studio Project Files

```powershell
$env:VCPKG_ROOT = "c:\Users\microai\AzAI-CLI\vcpkg"
cd c:\Users\microai\AzAI-CLI\cpp-cli

mkdir build
cd build

# Generate Visual Studio solution
cmake .. `
  -DCMAKE_TOOLCHAIN_FILE="$env:VCPKG_ROOT/scripts/buildsystems/cmake/vcpkg.cmake" `
  -G "Visual Studio 17 2022" `
  -A x64
```

## Step 2: Open in Visual Studio

1. Navigate to `cpp-cli/build` folder
2. Open `azai-cli.sln` in Visual Studio
3. In Solution Explorer, right-click the `azai-cli` project
4. Select "Set as Startup Project"
5. Build → Build Solution (or press Ctrl+Shift+B)
6. Wait for compilation to complete

## Step 3: Configure Dependencies

If you get linking errors about missing libraries:

1. In Visual Studio: Project → Properties
2. Go to: C/C++ → General → Additional Include Directories
3. Add:
   ```
   $env:VCPKG_ROOT\installed\x64-windows\include
   ```

4. Go to: Linker → General → Additional Library Directories
5. Add:
   ```
   $env:VCPKG_ROOT\installed\x64-windows\lib
   ```

## Step 4: Run the Executable

After successful build, the executable will be at:
```
cpp-cli/build/bin/Release/azai-cli.exe
```

Run from command line:
```powershell
.\bin\Release\azai-cli.exe --help
.\bin\Release\azai-cli.exe chat --config config.json --prompt "Hello!"
```

## Troubleshooting

### "Cannot find CMAKE_TOOLCHAIN_FILE"
- Ensure `$env:VCPKG_ROOT` is set correctly
- Verify the path to vcpkg.cmake exists

### "Unable to open include file"
- vcpkg dependencies may not be installed
- Run from vcpkg folder:
  ```
  .\vcpkg.exe install nlohmann-json cpr fmt --triplet x64-windows
  ```

### Linker errors
- Check that x64-windows triplet matches Visual Studio (should be x64)
- You may need to rebuild vcpkg packages with static linking:
  ```
  .\vcpkg.exe install nlohmann-json cpr fmt --triplet x64-windows-static
  ```
  Then update CMakeLists.txt to link static libraries
