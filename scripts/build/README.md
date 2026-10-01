# Build Scripts Directory

This directory contains comprehensive build scripts for NeoZorKDEXArb, supporting multiple platforms and build methods.

## 🚀 Universal Build Script (Recommended)

**`build-universal.sh`** - The main build script that supports all platforms and build methods through a single, user-friendly interface.

### Features
- **Multi-platform**: macOS, Linux, Alpine, Windows
- **Multiple methods**: Native, Container, Cross-platform, Wine
- **Interactive mode**: Menu-driven interface for beginners
- **Non-interactive mode**: Command-line options for automation
- **Automatic dependency detection**: Checks and reports missing requirements
- **vcpkg integration**: Automatic dependency management
- **Package creation**: Generates distribution packages
- **Test integration**: Optional test execution after build

### Quick Start
```bash
# Make executable
chmod +x build-universal.sh

# Interactive build
./build-universal.sh

# Quick build for specific platform
./build-universal.sh --platform macos --build-type Release

# Full build with all features
./build-universal.sh --clean --test --package --verbose
```

**📖 [Complete Documentation](../docs/getting-started/UNIVERSAL_BUILD_SCRIPT.md)**

## 🔧 Specialized Build Scripts

### Platform-Specific Scripts

#### `build-apple-container.sh`
- **Purpose**: macOS builds with Apple Container support
- **Features**: ARM64/Intel support, vcpkg integration
- **Usage**: `./build-apple-container.sh [--local] [--package]`

#### `build-multi-platform.sh`
- **Purpose**: Multi-platform builds for distribution
- **Features**: Cross-compilation, multiple targets
- **Usage**: `./build-multi-platform.sh [platform]`

#### `build-alpine-arm64.sh`
- **Purpose**: Alpine Linux ARM64 builds
- **Features**: Lightweight container builds
- **Usage**: `./build-alpine-arm64.sh`

### Build and Deploy Scripts

#### `build-and-deploy.sh`
- **Purpose**: Automated build and deployment
- **Features**: Git integration, automatic commits
- **Usage**: `./build-and-deploy.sh "commit message" [options]`

#### `build-modern.sh`
- **Purpose**: Modern build system with vcpkg
- **Features**: Dependency management, optimized builds
- **Usage**: `./build-modern.sh [--clean] [--package]`

### Utility Scripts

#### `cmake.sh`
- **Purpose**: Basic CMake build wrapper
- **Features**: Simple build process
- **Usage**: `./cmake.sh`

## 📋 Script Comparison

| Script | Platform | Method | Interactive | Dependencies | Best For |
|--------|----------|--------|-------------|--------------|----------|
| `build-universal.sh` | All | Multiple | ✅ Yes | Auto-detect | **General use** |
| `build-apple-container.sh` | macOS | Container | ❌ No | Apple Container | macOS development |
| `build-multi-platform.sh` | All | Cross-compile | ❌ No | Manual setup | Distribution |
| `build-and-deploy.sh` | All | Native | ❌ No | Git | CI/CD |
| `build-modern.sh` | All | vcpkg | ❌ No | vcpkg | Modern builds |
| `cmake.sh` | All | CMake | ❌ No | CMake | Simple builds |

## 🎯 When to Use Each Script

### For New Users
- **Start with**: `build-universal.sh`
- **Why**: Interactive, automatic dependency detection, works everywhere

### For Developers
- **Daily builds**: `build-universal.sh --platform macos --clean`
- **Debug builds**: `build-universal.sh --build-type Debug --test`
- **Container builds**: `build-universal.sh` (choose container option)

### For CI/CD
- **Automated builds**: `build-universal.sh --platform linux --clean --test --package`
- **Deployment**: `build-and-deploy.sh "Release v1.0.8"`
- **Multi-platform**: `build-multi-platform.sh`

### For Distribution
- **Package creation**: `build-universal.sh --package`
- **Cross-platform**: `build-multi-platform.sh`
- **Windows builds**: `build-universal.sh --platform windows`

## 🔍 Script Features

### Universal Build Script
- ✅ **Platform detection**: Automatic OS detection
- ✅ **Dependency checking**: Reports missing requirements
- ✅ **Multiple build methods**: Native, Container, Wine
- ✅ **Interactive mode**: User-friendly menus
- ✅ **Command-line options**: Automation friendly
- ✅ **vcpkg integration**: Automatic dependency management
- ✅ **Package creation**: Distribution packages
- ✅ **Test integration**: Optional test execution
- ✅ **Color output**: Clear visual feedback
- ✅ **Error handling**: Comprehensive error reporting

### Other Scripts
- 🔄 **Platform-specific**: Optimized for specific platforms
- 🔄 **Specialized methods**: Focused on specific build types
- 🔄 **Automation**: Designed for CI/CD workflows
- 🔄 **Legacy support**: Maintains compatibility

## 🚀 Getting Started

### 1. Choose Your Script
```bash
# For most users (recommended)
./build-universal.sh

# For macOS development
./build-apple-container.sh

# For automated builds
./build-and-deploy.sh "Build message"
```

### 2. Make Scripts Executable
```bash
chmod +x *.sh
```

### 3. Run Your First Build
```bash
# Interactive build
./build-universal.sh

# Quick build
./build-universal.sh --platform macos --build-type Release
```

## 📚 Documentation

- **[Universal Build Script](../docs/getting-started/UNIVERSAL_BUILD_SCRIPT.md)** - Complete guide
- **[Build Instructions](../docs/getting-started/BUILD_AND_USAGE.md)** - Traditional builds
- **[Platform Setup](../docs/getting-started/platforms/README.md)** - Platform-specific setup

## 🆘 Troubleshooting

### Common Issues

#### Script Not Executable
```bash
chmod +x build-universal.sh
```

#### Dependencies Missing
```bash
# macOS
brew install cmake git
xcode-select --install

# Ubuntu
sudo apt install build-essential cmake git

# Alpine
apk add --no-cache build-base cmake git
```

#### Platform Detection Issues
```bash
# Force platform
./build-universal.sh --platform macos
```

### Debug Mode
```bash
# Verbose output
./build-universal.sh --verbose

# Check script syntax
bash -n build-universal.sh

# Run with debug logging
bash -x build-universal.sh
```

## 🔮 Future Enhancements

- **Cloud build support**: Remote compilation
- **Binary distribution**: Pre-built packages
- **Plugin system**: Extensible build methods
- **Performance profiling**: Build time optimization
- **Dependency caching**: Faster subsequent builds

## 📞 Support

For issues and questions:

1. **Check troubleshooting section** above
2. **Review script help**: `./build-universal.sh --help`
3. **Enable verbose mode**: `--verbose` flag
4. **Check dependencies**: Script reports missing requirements
5. **Review logs**: Color-coded output for debugging

---

**Ready to build? Start with `./build-universal.sh` for the best experience!** 🚀
