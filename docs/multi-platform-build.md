# Multi-Platform Build Guide for NeoZorKDEXArb

This guide explains how to build NeoZorKDEXArb for multiple platforms using Apple Container CLI without Docker or Python.

## 🎯 Supported Platforms

- **macOS** (native Apple Silicon/Intel)
- **Linux Alpine** (musl libc)
- **Linux Ubuntu** (glibc)
- **Windows** (cross-compilation)

## 🚀 Prerequisites

### Required Software
- Apple Container CLI (`container` command)
- CMake 3.28 or higher
- Git
- Xcode Command Line Tools (for macOS builds)

### Install Apple Container CLI
```bash
# Install Docker Desktop (includes Apple Container CLI)
brew install --cask docker

# Verify installation
container version
```

## 📋 Quick Start

### 1. Build for All Platforms
```bash
# Build, test, and package for all platforms
./scripts/build-multi-platform.sh --all
```

### 2. Build for Specific Platform
```bash
# Build only for macOS
./scripts/build-multi-platform.sh macos

# Build for Alpine and Ubuntu
./scripts/build-multi-platform.sh alpine ubuntu

# Build for Windows only
./scripts/build-multi-platform.sh windows
```

### 3. Test All Platforms
```bash
# Test functionality across all built platforms
./scripts/test-all-platforms.sh
```

## 🔧 Build Options

### Command Line Options
```bash
./scripts/build-multi-platform.sh [OPTIONS] [PLATFORMS...]

Options:
  --help, -h          Show help message
  --clean             Clean build directories before building
  --test              Run tests after building
  --package           Create distribution packages after building
  --all               Build all platforms, test, and package

Platforms:
  macos               Build for macOS (native)
  alpine              Build for Linux Alpine (musl)
  ubuntu              Build for Linux Ubuntu (glibc)
  windows             Build for Windows (cross-compilation)
  all                 Build for all platforms
```

### Examples
```bash
# Clean build with tests
./scripts/build-multi-platform.sh --clean --test macos

# Build all platforms and create packages
./scripts/build-multi-platform.sh --all

# Build specific platforms with tests
./scripts/build-multi-platform.sh --test alpine ubuntu
```

## 🏗️ Build Process Details

### macOS Build
- **Method**: Native compilation
- **Compiler**: clang++ (Apple)
- **Dependencies**: vcpkg with x64-osx triplet
- **Output**: `build-macos/bin/NeoZorKDEXArb`

### Alpine Linux Build
- **Method**: Apple Container with Alpine image
- **Compiler**: g++ (GNU)
- **Dependencies**: Alpine packages (alpine-sdk, cmake, g++)
- **Output**: `build-alpine/bin/NeoZorKDEXArb`

### Ubuntu Linux Build
- **Method**: Apple Container with Ubuntu image
- **Compiler**: g++ (GNU)
- **Dependencies**: Ubuntu packages (build-essential, cmake, g++)
- **Output**: `build-ubuntu/bin/NeoZorKDEXArb`

### Windows Build
- **Method**: Cross-compilation in Ubuntu container
- **Compiler**: x86_64-w64-mingw32-g++ (MinGW)
- **Dependencies**: MinGW cross-compiler toolchain
- **Output**: `build-windows/bin/NeoZorKDEXArb.exe`

## 🧪 Testing

### Run Tests for Specific Platform
```bash
# Test macOS build
./scripts/build-multi-platform.sh --test macos

# Test Alpine build
./scripts/build-multi-platform.sh --test alpine
```

### Comprehensive Testing
```bash
# Test all platforms
./scripts/test-all-platforms.sh
```

### Test Types
- **Platform Compatibility**: Basic functionality tests
- **Cross-Platform Consistency**: Version and behavior consistency
- **Network Functionality**: Network operations (when implemented)
- **File Operations**: File system operations

## 📦 Distribution Packages

### Create Packages
```bash
# Build and package all platforms
./scripts/build-multi-platform.sh --all
```

### Package Structure
```
dist/
├── macos/
│   └── NeoZorKDEXArb
├── alpine/
│   └── NeoZorKDEXArb
├── ubuntu/
│   └── NeoZorKDEXArb
├── windows/
│   └── NeoZorKDEXArb.exe
└── README.txt
```

## 🔍 Troubleshooting

### Common Issues

#### Apple Container CLI Not Found
```bash
# Install Docker Desktop
brew install --cask docker

# Start Docker Desktop
open /Applications/Docker.app

# Verify container command
container version
```

#### Build Failures
```bash
# Clean and rebuild
./scripts/build-multi-platform.sh --clean --all

# Check specific platform
./scripts/build-multi-platform.sh --clean alpine
```

#### Test Failures
```bash
# Run tests individually
./scripts/test-all-platforms.sh

# Check test reports
ls -la test-report-*.txt
```

### Platform-Specific Issues

#### Alpine Build Issues
- Ensure Alpine image is available: `container pull alpine:latest`
- Check for sufficient disk space
- Verify network connectivity for package downloads

#### Ubuntu Build Issues
- Ensure Ubuntu image is available: `container pull ubuntu:22.04`
- Check for package repository access
- Verify build dependencies installation

#### Windows Build Issues
- Ensure MinGW toolchain is properly installed
- Check for cross-compilation dependencies
- Verify Windows-specific libraries

## 📊 Build Performance

### Expected Build Times
- **macOS**: 2-5 minutes
- **Alpine**: 5-10 minutes
- **Ubuntu**: 5-10 minutes
- **Windows**: 8-15 minutes

### Optimization Tips
```bash
# Use parallel builds
export MAKEFLAGS="-j$(nproc)"

# Clean builds for faster iteration
./scripts/build-multi-platform.sh --clean alpine

# Build only needed platforms
./scripts/build-multi-platform.sh macos alpine
```

## 🔄 Continuous Integration

### GitHub Actions Example
```yaml
name: Multi-Platform Build

on: [push, pull_request]

jobs:
  build:
    runs-on: macos-latest
    
    steps:
    - uses: actions/checkout@v3
    
    - name: Build all platforms
      run: |
        chmod +x scripts/build-multi-platform.sh
        ./scripts/build-multi-platform.sh --all
    
    - name: Test all platforms
      run: |
        chmod +x scripts/test-all-platforms.sh
        ./scripts/test-all-platforms.sh
```

## 📚 Additional Resources

### Scripts
- `scripts/build-multi-platform.sh` - Main build script
- `scripts/test-all-platforms.sh` - Testing script
- `scripts/build-apple-container.sh` - Legacy macOS script

### Configuration Files
- `vcpkg/triplets/` - Platform-specific triplets
- `vcpkg/scripts/toolchains/` - Compiler toolchains
- `CMakeLists.txt` - Build configuration

### Documentation
- `docs/` - Project documentation
- `include/utils/platform.h` - Platform compatibility layer
- `tests/cpp/test_platform_compatibility.cpp` - Platform tests

## 🤝 Contributing

### Adding New Platforms
1. Create new triplet in `vcpkg/triplets/`
2. Create new toolchain in `vcpkg/scripts/toolchains/`
3. Add platform detection in `include/utils/platform.h`
4. Update build scripts
5. Add platform-specific tests

### Testing New Platforms
```bash
# Test new platform
./scripts/build-multi-platform.sh new_platform

# Run compatibility tests
./scripts/test-all-platforms.sh
```

## 📞 Support

For issues and questions:
- Check troubleshooting section above
- Review test reports and build logs
- Verify platform requirements
- Ensure Apple Container CLI is working

---

**Note**: This build system uses Apple Container CLI for cross-platform compilation without requiring Docker or Python, providing a native macOS experience while supporting multiple target platforms.
