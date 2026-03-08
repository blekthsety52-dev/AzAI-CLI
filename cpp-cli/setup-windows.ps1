# AzAI-CLI Windows Setup Script
# This script automates the entire setup including CMake, vcpkg, and building

param(
    [switch]$SkipCMake,
    [switch]$SkipBuild,
    [switch]$SkipVcpkg
)

$ErrorActionPreference = "Stop"
$scriptRoot = Split-Path -Parent $MyInvocation.MyCommand.Path
$projectRoot = Split-Path -Parent $scriptRoot
$vcpkgRoot = Join-Path $projectRoot "vcpkg"
$cppCliRoot = Join-Path $projectRoot "cpp-cli"
$buildDir = Join-Path $cppCliRoot "build"

Write-Host "=====================================" -ForegroundColor Cyan
Write-Host "AzAI-CLI Windows Setup Script" -ForegroundColor Cyan
Write-Host "=====================================" -ForegroundColor Cyan
Write-Host ""

# Step 1: Check and install CMake
if (-not $SkipCMake) {
    Write-Host "[1/4] Checking CMake installation..." -ForegroundColor Yellow
    
    $cmakePath = $null
    try {
        $cmakePath = $(where.exe cmake 2>$null)
    } catch {
        $cmakePath = $null
    }
    
    if ($cmakePath) {
        Write-Host "[OK] CMake already installed at: $cmakePath" -ForegroundColor Green
        & cmake --version | Select-Object -First 1
    } else {
        Write-Host "[INFO] CMake not found. Installing CMake..." -ForegroundColor Red
        
        $cmakeVersion = "3.31.10"
        $cmakeUrl = "https://github.com/Kitware/CMake/releases/download/v$cmakeVersion/cmake-$cmakeVersion-windows-x86_64.msi"
        $cmakeInstaller = "$env:TEMP\cmake-installer.msi"
        
        Write-Host "Downloading CMake $cmakeVersion from GitHub..." -ForegroundColor Cyan
        try {
            Invoke-WebRequest -Uri $cmakeUrl -OutFile $cmakeInstaller -UseBasicParsing
            Write-Host "Installing CMake..." -ForegroundColor Cyan
            Start-Process msiexec.exe -ArgumentList "/i", $cmakeInstaller, "/qn" -Wait
            Remove-Item $cmakeInstaller
            
            $pathMachine = [System.Environment]::GetEnvironmentVariable("Path","Machine")
            $pathUser = [System.Environment]::GetEnvironmentVariable("Path","User")
            $env:Path = $pathMachine + ";" + $pathUser
            
            Write-Host "[OK] CMake installed successfully" -ForegroundColor Green
            & cmake --version | Select-Object -First 1
        } catch {
            Write-Host "[ERROR] Failed to download CMake automatically" -ForegroundColor Red
            Write-Host "Please download manually from: $cmakeUrl" -ForegroundColor Yellow
            Write-Host "Then run this script again with -SkipCMake flag" -ForegroundColor Yellow
            exit 1
        }
    }
} else {
    Write-Host "[1/4] Skipping CMake check" -ForegroundColor Yellow
}

Write-Host ""

# Step 2: Setup vcpkg environment
Write-Host "[2/4] Setting up vcpkg..." -ForegroundColor Yellow

$env:VCPKG_ROOT = $vcpkgRoot

$vcpkgExe = Join-Path $vcpkgRoot "vcpkg.exe"
if (-not (Test-Path $vcpkgExe)) {
    Write-Host "[ERROR] vcpkg.exe not found at: $vcpkgExe" -ForegroundColor Red
    exit 1
}

Write-Host "[OK] vcpkg found at: $vcpkgExe" -ForegroundColor Green
& $vcpkgExe --version

Write-Host "Cleaning up partial downloads..." -ForegroundColor Cyan
$downloadDir = Join-Path $vcpkgRoot "downloads\tools"
if (Test-Path $downloadDir) {
    Get-ChildItem $downloadDir -Filter "*.partial.*" -Recurse -ErrorAction SilentlyContinue | Remove-Item -Recurse -Force -ErrorAction SilentlyContinue
}

Write-Host ""

# Step 3: Install dependencies with vcpkg
if (-not $SkipVcpkg) {
    Write-Host "[3/4] Installing dependencies with vcpkg..." -ForegroundColor Yellow
    Write-Host "This may take several minutes..." -ForegroundColor Cyan
    
    Push-Location $vcpkgRoot
    try {
        & .\vcpkg.exe install nlohmann-json:x64-windows cpr:x64-windows fmt:x64-windows --recurse
        
        if ($LASTEXITCODE -ne 0) {
            Write-Host "[ERROR] vcpkg install failed" -ForegroundColor Red
            exit 1
        }
        Write-Host "[OK] Dependencies installed successfully" -ForegroundColor Green
    } finally {
        Pop-Location
    }
} else {
    Write-Host "[3/4] Skipping vcpkg dependency installation" -ForegroundColor Yellow
}

Write-Host ""

# Step 4: Build the project
if (-not $SkipBuild) {
    Write-Host "[4/4] Building C++ project..." -ForegroundColor Yellow
    
    if (-not (Test-Path $buildDir)) {
        New-Item -ItemType Directory -Path $buildDir -Force | Out-Null
    }
    
    Push-Location $buildDir
    try {
        Write-Host "Configuring CMake..." -ForegroundColor Cyan
        & cmake.exe .. -DCMAKE_TOOLCHAIN_FILE="$vcpkgRoot/scripts/buildsystems/cmake/vcpkg.cmake" -G "Visual Studio 17 2022" -A x64
        
        if ($LASTEXITCODE -ne 0) {
            Write-Host "[ERROR] CMake configuration failed" -ForegroundColor Red
            exit 1
        }
        
        Write-Host "Building project..." -ForegroundColor Cyan
        & cmake.exe --build . --config Release
        
        if ($LASTEXITCODE -ne 0) {
            Write-Host "[ERROR] Build failed" -ForegroundColor Red
            exit 1
        }
        
        Write-Host "[OK] Build completed successfully" -ForegroundColor Green
        Write-Host ""
        Write-Host "Executable location:" -ForegroundColor Cyan
        Write-Host "$buildDir\bin\Release\azai-cli.exe" -ForegroundColor Green
    } finally {
        Pop-Location
    }
} else {
    Write-Host "[4/4] Skipping build" -ForegroundColor Yellow
}

Write-Host ""
Write-Host "=====================================" -ForegroundColor Cyan
Write-Host "Setup Complete!" -ForegroundColor Green
Write-Host "=====================================" -ForegroundColor Cyan
Write-Host ""
Write-Host "Next Steps:" -ForegroundColor Yellow
Write-Host "1. Edit config.json with your Azure credentials:" -ForegroundColor White
Write-Host "   $(Join-Path $cppCliRoot 'config.json')" -ForegroundColor Green
Write-Host ""
Write-Host "2. Run the CLI tool:" -ForegroundColor White
Write-Host "   $(Join-Path $buildDir 'bin\Release\azai-cli.exe') chat --config config.json --prompt 'Hello!'" -ForegroundColor Green
Write-Host ""
Write-Host "3. For more info, see README.md:" -ForegroundColor White
Write-Host "   $(Join-Path $cppCliRoot 'README.md')" -ForegroundColor Green
Write-Host ""
