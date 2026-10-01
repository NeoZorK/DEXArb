# Multi-Platform Build Guide

Comprehensive guide for building DEXArb across multiple platforms including Linux, macOS, and Windows.

## 🎯 Overview

This guide covers building DEXArb on all supported platforms, ensuring consistent build processes and cross-platform compatibility.

## 🏗️ Build System Architecture

### CMake-Based Build System
```
┌─────────────────────────────────────────────────────────┐
│                 CMake Build System                      │
├─────────────────────────────────────────────────────────┤
│  ┌─────────────┐  ┌─────────────┐  ┌─────────────┐    │
│  │  Platform   │  │   Compiler  │  │   Generator │    │
│  │ Detection   │  │ Selection   │  │ Selection   │    │
│  └─────────────┘  └─────────────┘  └─────────────┘    │
│  ┌─────────────┐  ┌─────────────┐  ┌─────────────┐    │
│  │ Dependency  │  │   Build     │  │   Install   │    │
│  │ Management  │  │  Process    │  │   Process   │    │
│  └─────────────┘  └─────────────┘  └─────────────┘    │
└─────────────────────────────────────────────────────────┘
```

### Supported Platforms
- **Linux**: Ubuntu, Debian, CentOS, RHEL, Alpine
- **macOS**: 10.15+ (Catalina), Apple Silicon support
- **Windows**: 10+ via Wine, native builds planned
- **Containers**: Docker-based builds for all platforms

## 🐧 Linux Builds

### Ubuntu/Debian
```bash
# Install dependencies
sudo apt update
sudo apt install -y build-essential cmake git \
    libssl-dev libcurl4-openssl-dev nlohmann-json3-dev \
    pkg-config

# Build process
mkdir build && cd build
cmake .. -DCMAKE_BUILD_TYPE=Release
make -j$(nproc)

# Install (optional)
sudo make install
```

### Alpine Linux
```bash
# Install dependencies
apk add --no-cache build-base cmake git \
    openssl-dev curl-dev nlohmann-json-dev \
    pkgconfig

# Build process
mkdir build && cd build
cmake .. -DCMAKE_BUILD_TYPE=Release
make -j$(nproc)
```

### CentOS/RHEL
```bash
# Install dependencies
sudo yum groupinstall -y "Development Tools"
sudo yum install -y cmake git openssl-devel curl-devel

# Install nlohmann-json (may need to build from source)
git clone https://github.com/nlohmann/json.git
cd json && mkdir build && cd build
cmake .. && make install

# Build DEXArb
cd /path/to/DEXArb
mkdir build && cd build
cmake .. -DCMAKE_BUILD_TYPE=Release
make -j$(nproc)
```

## 🍎 macOS Build

### Using Homebrew
```bash
# Install dependencies
brew install cmake git openssl nlohmann-json

# Set environment variables
export OPENSSL_ROOT_DIR=$(brew --prefix openssl)
export CMAKE_PREFIX_PATH=$(brew --prefix)

# Build process
mkdir build && cd build
cmake .. -DCMAKE_BUILD_TYPE=Release
make -j$(sysctl -n hw.ncpu)
```

### Using MacPorts
```bash
# Install dependencies
sudo port install cmake git openssl nlohmann-json

# Build process
mkdir build && cd build
cmake .. -DCMAKE_BUILD_TYPE=Release
make -j$(sysctl -n hw.ncpu)
```

### Apple Silicon (ARM64) Support
```bash
# Check architecture
uname -m

# For ARM64 builds
mkdir build && cd build
cmake .. -DCMAKE_BUILD_TYPE=Release \
    -DCMAKE_OSX_ARCHITECTURES=arm64

# Universal binary (both architectures)
cmake .. -DCMAKE_BUILD_TYPE=Release \
    -DCMAKE_OSX_ARCHITECTURES="x86_64;arm64"
```

## 🪟 Windows Build

### Wine-Based Build (Linux/macOS Host)
```bash
# Install Wine and MinGW
sudo apt install wine64 wine32 mingw-w64  # Ubuntu
brew install --cask wine-stable           # macOS

# Set environment variables
export CC=x86_64-w64-mingw32-gcc
export CXX=x86_64-w64-mingw32-g++
export CMAKE_GENERATOR="MinGW Makefiles"

# Build process
mkdir build && cd build
cmake .. -G "MinGW Makefiles" \
    -DCMAKE_C_COMPILER=x86_64-w64-mingw32-gcc \
    -DCMAKE_CXX_COMPILER=x86_64-w64-mingw32-g++ \
    -DCMAKE_BUILD_TYPE=Release

mingw32-make -j$(nproc)
```

### Native Windows Build (Windows Host)
```bash
# Install Visual Studio 2019/2022 with C++ workload
# Install CMake from cmake.org

# Build process
mkdir build && cd build
cmake .. -G "Visual Studio 16 2019" -A x64
cmake --build . --config Release

# Or using MinGW on Windows
cmake .. -G "MinGW Makefiles"
mingw32-make
```

## 🐳 Container-Based Builds

### Docker Build Environment
```dockerfile
# Dockerfile.build
FROM ubuntu:20.04

ENV DEBIAN_FRONTEND=noninteractive
ENV TZ=UTC

RUN apt update && apt install -y \
    build-essential \
    cmake \
    git \
    libssl-dev \
    libcurl4-openssl-dev \
    nlohmann-json3-dev \
    pkg-config \
    && rm -rf /var/lib/apt/lists/*

WORKDIR /workspace
CMD ["/bin/bash"]
```

### Multi-Platform Container Build
```bash
# Build for multiple architectures
docker buildx create --use
docker buildx build --platform linux/amd64,linux/arm64 \
    -f Dockerfile.build -t dexarb-builder:latest .

# Run build in container
docker run --rm -v $(pwd):/workspace dexarb-builder:latest \
    bash -c "cd /workspace && mkdir build && cd build && \
    cmake .. && make -j$(nproc)"
```

### Cross-Compilation Container
```bash
# Windows cross-compilation container
docker run --rm -v $(pwd):/workspace dexarb-wine:latest \
    bash -c "cd /workspace && mkdir build && cd build && \
    cmake .. -G 'MinGW Makefiles' && mingw32-make"
```

## ⚙️ Build Configuration

### CMake Configuration Options
```bash
# Basic configuration
cmake .. -DCMAKE_BUILD_TYPE=Release

# With specific options
cmake .. \
    -DCMAKE_BUILD_TYPE=Release \
    -DENABLE_TESTS=ON \
    -DENABLE_DOCS=ON \
    -DENABLE_EXAMPLES=ON \
    -DCMAKE_INSTALL_PREFIX=/usr/local

# Platform-specific options
cmake .. \
    -DCMAKE_BUILD_TYPE=Release \
    -DPLATFORM_SPECIFIC_OPTION=ON
```

### Environment Variables
```bash
# Compiler selection
export CC=gcc-9
export CXX=g++-9

# Build flags
export CFLAGS="-O3 -march=native"
export CXXFLAGS="-O3 -march=native"

# CMake variables
export CMAKE_BUILD_TYPE=Release
export CMAKE_INSTALL_PREFIX=/usr/local
```

### Build Variants
```bash
# Debug build
cmake .. -DCMAKE_BUILD_TYPE=Debug

# Release build
cmake .. -DCMAKE_BUILD_TYPE=Release

# Release with debug info
cmake .. -DCMAKE_BUILD_TYPE=RelWithDebInfo

# Minimal size release
cmake .. -DCMAKE_BUILD_TYPE=MinSizeRel
```

## 🔧 Dependency Management

### System Package Managers
```bash
# Ubuntu/Debian
sudo apt install libssl-dev libcurl4-openssl-dev nlohmann-json3-dev

# Alpine
apk add openssl-dev curl-dev nlohmann-json-dev

# CentOS/RHEL
sudo yum install openssl-devel curl-devel

# macOS
brew install openssl nlohmann-json
```

### vcpkg Integration
```bash
# Install vcpkg
git clone https://github.com/Microsoft/vcpkg.git
cd vcpkg
./bootstrap-vcpkg.sh

# Install dependencies
./vcpkg install nlohmann-json cpr

# Configure CMake with vcpkg
cmake .. -DCMAKE_TOOLCHAIN_FILE=[path to vcpkg]/scripts/buildsystems/vcpkg.cmake
```

### Bundled Dependencies
```bash
# Build with bundled dependencies
cmake .. -DUSE_BUNDLED_DEPS=ON

# Specific dependency options
cmake .. \
    -DUSE_BUNDLED_OPENSSL=ON \
    -DUSE_BUNDLED_NLOHMANN_JSON=ON \
    -DUSE_BUNDLED_CURL=ON
```

## 🧪 Testing and Validation

### Build Verification
```bash
# Run tests after build
cd build
ctest --output-on-failure

# Run specific test suite
./tests/cpp/test_modern_utils
./tests/cpp/test_arbitrage

# Generate coverage report
make coverage
```

### Cross-Platform Testing
```bash
# Test on different platforms
# Linux
docker run --rm -v $(pwd):/workspace ubuntu:20.04 bash -c \
    "cd /workspace && mkdir build && cd build && cmake .. && make && ctest"

# macOS (if available)
mkdir build && cd build
cmake .. && make && ctest

# Windows (Wine)
docker run --rm -v $(pwd):/workspace dexarb-wine:latest bash -c \
    "cd /workspace && mkdir build && cd build && \
    cmake .. -G 'MinGW Makefiles' && mingw32-make && ctest"
```

## 📊 Performance Optimization

### Compiler Optimizations
```bash
# GCC optimizations
cmake .. -DCMAKE_CXX_FLAGS="-O3 -march=native -mtune=native"

# Clang optimizations
cmake .. -DCMAKE_CXX_FLAGS="-O3 -march=native"

# MSVC optimizations
cmake .. -DCMAKE_CXX_FLAGS="/O2 /arch:AVX2"
```

### Build Performance
```bash
# Parallel builds
make -j$(nproc)           # Linux
make -j$(sysctl -n hw.ncpu)  # macOS
mingw32-make -j$(nproc)   # Windows

# Distributed builds
make -j$(nproc) -l$(nproc)
```

### Memory Optimization
```bash
# Limit memory usage
cmake .. -DCMAKE_CXX_FLAGS="-O2 -fno-omit-frame-pointer"

# Use gold linker (Linux)
cmake .. -DCMAKE_EXE_LINKER_FLAGS="-fuse-ld=gold"
```

## 🔍 Troubleshooting

### Common Build Issues

#### Compiler Not Found
```bash
# Check available compilers
which gcc g++ clang clang++
gcc --version
g++ --version

# Set specific compiler
cmake -DCMAKE_C_COMPILER=gcc-9 -DCMAKE_CXX_COMPILER=g++-9 ..
```

#### Missing Dependencies
```bash
# Check installed packages
pkg-config --list-all

# Install missing dependencies
sudo apt install libssl-dev libcurl4-openssl-dev nlohmann-json3-dev

# Check library paths
echo $LD_LIBRARY_PATH
ldconfig -p | grep ssl
```

#### CMake Configuration Issues
```bash
# Clean build directory
rm -rf build && mkdir build

# Debug CMake configuration
cmake .. -DCMAKE_VERBOSE_MAKEFILE=ON

# Check CMake version
cmake --version
```

### Platform-Specific Issues

#### Linux Issues
```bash
# Permission issues
sudo chown -R $USER:$USER .

# Library path issues
export LD_LIBRARY_PATH=/usr/local/lib:$LD_LIBRARY_PATH
sudo ldconfig
```

#### macOS Issues
```bash
# OpenSSL issues
export OPENSSL_ROOT_DIR=$(brew --prefix openssl)
export CMAKE_PREFIX_PATH=$(brew --prefix)

# Xcode command line tools
xcode-select --install
```

#### Windows Issues (Wine)
```bash
# Wine configuration
winecfg
# Set Windows version to Windows 10
# Enable CSMT for better performance

# MinGW issues
sudo apt install mingw-w64
export CC=x86_64-w64-mingw32-gcc
export CXX=x86_64-w64-mingw32-g++
```

## 🚀 Automation

### Build Scripts
```bash
#!/bin/bash
# build-all-platforms.sh

set -e

echo "Building DEXArb for all platforms..."

# Linux build
echo "Building for Linux..."
mkdir -p build-linux && cd build-linux
cmake .. -DCMAKE_BUILD_TYPE=Release
make -j$(nproc)
cd ..

# macOS build (if available)
if [[ "$OSTYPE" == "darwin"* ]]; then
    echo "Building for macOS..."
    mkdir -p build-macos && cd build-macos
    cmake .. -DCMAKE_BUILD_TYPE=Release
    make -j$(sysctl -n hw.ncpu)
    cd ..
fi

# Windows build (Wine)
echo "Building for Windows (Wine)..."
docker run --rm -v $(pwd):/workspace dexarb-wine:latest \
    bash -c "cd /workspace && mkdir build-windows && cd build-windows && \
    cmake .. -G 'MinGW Makefiles' && mingw32-make"

echo "All builds completed successfully!"
```

### CI/CD Integration
```yaml
# .github/workflows/build.yml
name: Multi-Platform Build

on: [push, pull_request]

jobs:
  build:
    strategy:
      matrix:
        platform: [ubuntu-latest, macos-latest, windows-latest]
    
    runs-on: ${{ matrix.platform }}
    
    steps:
      - uses: actions/checkout@v3
      
      - name: Install dependencies
        run: |
          # Platform-specific dependency installation
          
      - name: Build
        run: |
          mkdir build && cd build
          cmake .. -DCMAKE_BUILD_TYPE=Release
          cmake --build . --config Release
          
      - name: Test
        run: |
          cd build
          ctest --output-on-failure
```

## 📚 Next Steps

After successful multi-platform build:
- [Configure the application](../../getting-started/configuration.md)
- [Run your first scan](../../getting-started/first-steps.md)
- [Deploy to production](../README.md)
- [Learn about testing](../../testing/README.md)

---

*Multi-platform build successful? Let's deploy DEXArb everywhere!*
