# Windows Container Runner

This document describes how to run NeoZorKDEXArb in a Windows container environment using Apple Container CLI.

## 🪟 Overview

The Windows Container Runner provides a Windows Server Core environment for building and running NeoZorKDEXArb with full MSVC compatibility. This is ideal for testing Windows-specific behavior and ensuring cross-platform compatibility.

## 🚀 Quick Start

```bash
# Make script executable
chmod +x scripts/container/run-windows-container.sh

# Run application in Windows container
./scripts/container/run-windows-container.sh

# Show help
./scripts/container/run-windows-container.sh --help

# Force rebuild
./scripts/container/run-windows-container.sh --build

# Clean build and rebuild
./scripts/container/run-windows-container.sh --clean --build
```

## 🔧 Features

### Windows Environment
- ✅ Windows Server Core LTSC 2022
- ✅ MSVC compiler support
- ✅ Windows-specific optimizations
- ✅ Full Windows API compatibility

### Build System
- ✅ CMake 3.28+ support
- ✅ Ninja build system
- ✅ vcpkg dependency management
- ✅ Visual Studio 2022 Build Tools

### Dependencies
- ✅ CURL for HTTP requests
- ✅ nlohmann/json for JSON parsing
- ✅ Threading support
- ✅ Windows-specific libraries

## 📋 Prerequisites

1. **Apple Container CLI** installed and available
2. **macOS** operating system
3. **Internet connection** for container images
4. **Sufficient disk space** for Windows builds (~2GB)
5. **Git** for vcpkg installation

## 🏗️ Build Process

### Automatic Build
The script automatically:
1. Downloads Windows Server Core container
2. Installs Chocolatey package manager
3. Installs CMake, Ninja, and Visual Studio Build Tools
4. Sets up vcpkg for dependency management
5. Builds the application with MSVC

### Build Directory
- **Location**: `build-windows/`
- **Executable**: `build-windows/bin/NeoZorKDEXArb.exe`
- **Build Type**: Release (optimized)

## 🎯 Use Cases

| Use Case | Description |
|----------|-------------|
| Windows Testing | Test Windows-specific behavior |
| Cross-platform | Ensure Windows compatibility |
| MSVC Testing | Test MSVC compiler compatibility |
| Production | Windows deployment testing |

## ⚙️ Configuration

### Environment Variables
- `VCPKG_ROOT`: vcpkg installation directory
- `CMAKE_TOOLCHAIN_FILE`: vcpkg toolchain file
- `CMAKE_BUILD_TYPE`: Build type (Release/Debug)

### CMake Options
- `-G "Ninja"`: Use Ninja build system
- `-DCMAKE_BUILD_TYPE=Release`: Release build
- `-DBUILD_TESTING=OFF`: Disable testing
- `-DVCPKG_TARGET_TRIPLET=x64-windows`: Windows x64 target

## 🐛 Troubleshooting

### Common Issues

#### Container Pull Failures
```bash
# Check internet connection
ping mcr.microsoft.com

# Verify Apple Container CLI
container --version
```

#### Build Failures
```bash
# Clean and rebuild
./scripts/container/run-windows-container.sh --clean --build

# Check build logs
tail -f build-windows/CMakeFiles/CMakeOutput.log
```

#### Dependency Issues
```bash
# Force vcpkg reinstall
rm -rf build-windows
./scripts/container/run-windows-container.sh --build
```

### Performance Tips

1. **Use SSD storage** for faster builds
2. **Increase container memory** if available
3. **Cache vcpkg packages** between runs
4. **Use --clean sparingly** to avoid rebuilds

## 📊 Performance Comparison

| Platform | Build Time | Runtime | Memory Usage |
|----------|------------|---------|--------------|
| Alpine | ~2-3 min | Fast | ~50MB |
| Ubuntu | ~5-7 min | Medium | ~100MB |
| Windows | ~8-12 min | Medium | ~150MB |

## 🔄 Maintenance

### Updating Scripts
```bash
# Make executable
chmod +x scripts/container/run-windows-container.sh

# Check Apple Container CLI version
container --version
```

### Rebuilding Applications
```bash
# Force rebuild
./scripts/container/run-windows-container.sh --build

# Clean rebuild
./scripts/container/run-windows-container.sh --clean --build
```

## 📚 Related Documentation

- [Alpine Container Runner](./ALPINE_CONTAINER_RUNNER.md)
- [Ubuntu Container Runner](./UBUNTU_CONTAINER_RUNNER.md)
- [Getting Started](./README.md)
- [Build and Usage](../BUILD_AND_USAGE.md)

## 🆘 Support

For issues with Windows container runner:

1. Check this documentation
2. Review build logs in `build-windows/`
3. Verify Apple Container CLI installation
4. Check system resources and disk space

---

**Created**: 26.02.2025  
**Author**: Rostyslav S.  
**Version**: 1.0.7  
**Platform**: Windows Server Core LTSC 2022
