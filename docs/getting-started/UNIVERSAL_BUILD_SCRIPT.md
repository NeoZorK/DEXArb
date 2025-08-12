# Universal Interactive Build Script

## Overview

The **Universal Interactive Build Script** (`build-universal.sh`) is a comprehensive, cross-platform build solution for NeoZorKDEXArb that supports multiple build methods and platforms through a single, user-friendly interface.

## 🚀 Features

- **Multi-Platform Support**: macOS, Linux, Alpine, Windows
- **Multiple Build Methods**: Native, Container, Cross-platform, Wine
- **Interactive Mode**: User-friendly menu-driven interface
- **Non-Interactive Mode**: Command-line options for automation
- **Automatic Dependency Detection**: Checks and reports missing requirements
- **vcpkg Integration**: Automatic dependency management
- **Parallel Builds**: Optimized for multi-core systems
- **Package Creation**: Generates distribution packages
- **Test Integration**: Optional test execution after build
- **Color-coded Output**: Clear visual feedback

## 📋 Prerequisites

### Required Dependencies
- **Git** - Version control
- **CMake** 3.28+ - Build system
- **Make** - Build tool
- **C++ Compiler** - g++ or clang++

### Optional Dependencies
- **Docker** - For container builds
- **Wine** - For Windows builds
- **MinGW** - For Windows cross-compilation

### Installation by Platform

#### macOS
```bash
# Install Homebrew if not already installed
/bin/bash -c "$(curl -fsSL https://raw.githubusercontent.com/Homebrew/install/HEAD/install.sh)"

# Install dependencies
brew install cmake git
xcode-select --install
```

#### Ubuntu/Debian
```bash
sudo apt update
sudo apt install build-essential cmake git
```

#### CentOS/RHEL
```bash
sudo yum groupinstall 'Development Tools'
sudo yum install cmake git
```

#### Alpine
```bash
apk add --no-cache build-base cmake git
```

## 🎯 Usage

### Interactive Mode (Recommended)

```bash
# Make script executable
chmod +x scripts/build/build-universal.sh

# Run interactive build
./scripts/build/build-universal.sh
```

The script will:
1. Detect your platform automatically
2. Check dependencies
3. Present build options
4. Guide you through configuration
5. Execute the build
6. Run tests (optional)
7. Create packages (optional)

### Non-Interactive Mode

```bash
# Quick build with defaults
./scripts/build/build-universal.sh --platform macos --build-type Release

# Full build with all features
./scripts/build/build-universal.sh --clean --test --package --verbose

# Windows build
./scripts/build/build-universal.sh --platform windows --build-type Release

# Alpine container build
./scripts/build/build-universal.sh --platform alpine --clean
```

## ⚙️ Build Options

### 1. Native Build (Recommended)
- **Description**: Builds directly on your system
- **Best for**: Development, testing, production
- **Performance**: Fastest build times
- **Dependencies**: System packages

### 2. Container Build (Docker)
- **Description**: Builds in isolated containers
- **Best for**: Reproducible builds, CI/CD
- **Performance**: Slightly slower due to container overhead
- **Dependencies**: Docker

### 3. Cross-Platform Build
- **Description**: Builds for different architectures
- **Best for**: Distribution, deployment
- **Performance**: Varies by target
- **Dependencies**: Cross-compilation tools

### 4. Windows Build (Wine)
- **Description**: Builds Windows executables on Linux/macOS
- **Best for**: Windows deployment
- **Performance**: Slower due to Wine emulation
- **Dependencies**: Wine, MinGW

### 5. Alpine Container Build
- **Description**: Lightweight Alpine Linux container
- **Best for**: Minimal footprint, security
- **Performance**: Good
- **Dependencies**: Docker

### 6. Ubuntu Container Build
- **Description**: Full Ubuntu container
- **Best for**: Maximum compatibility
- **Performance**: Good
- **Dependencies**: Docker

## 🔧 Build Configuration

### Build Types
- **Release**: Optimized for production (default)
- **Debug**: With debug symbols for development
- **RelWithDebInfo**: Release with debug information

### Options
- **Clean Build**: Removes existing build directory
- **Run Tests**: Executes C++ tests after build
- **Create Package**: Generates distribution package
- **Verbose Output**: Detailed build information

## 📁 Output Structure

### Build Directories
```
DEXArb/
├── build-macos/           # macOS native build
├── build-linux/           # Linux native build
├── build-alpine/          # Alpine container build
├── build-ubuntu/          # Ubuntu container build
├── build-windows/         # Windows build
└── dist-{platform}/       # Distribution packages
```

### Executable Locations
- **Native builds**: `build-{platform}/bin/NeoZorKDEXArb`
- **Container builds**: `build-{container}/NeoZorKDEXArb`
- **Windows builds**: `build-windows/NeoZorKDEXArb.exe`

### Distribution Packages
- **Location**: `dist-{platform}/`
- **Contents**: Executable + README.txt
- **Format**: Platform-specific binary

## 🎮 Command Line Options

| Option | Description | Example |
|--------|-------------|---------|
| `--help, -h` | Show help message | `./build-universal.sh -h` |
| `--platform PLATFORM` | Specify platform | `--platform macos` |
| `--build-type TYPE` | Specify build type | `--build-type Debug` |
| `--clean` | Clean build directory | `--clean` |
| `--test` | Run tests after build | `--test` |
| `--package` | Create distribution package | `--package` |
| `--verbose` | Enable verbose output | `--verbose` |

### Platform Values
- `macos` - macOS (x86_64/ARM64)
- `linux` - Linux distributions
- `alpine` - Alpine Linux
- `windows` - Windows (via Wine)

### Build Type Values
- `Release` - Production optimized
- `Debug` - Development with symbols
- `RelWithDebInfo` - Release with debug info

## 🔍 Examples

### Basic Usage
```bash
# Interactive build
./scripts/build/build-universal.sh

# Quick macOS build
./scripts/build/build-universal.sh --platform macos

# Debug build with tests
./scripts/build/build-universal.sh --build-type Debug --test
```

### Advanced Usage
```bash
# Clean Alpine container build
./scripts/build/build-universal.sh --platform alpine --clean --package

# Windows build with verbose output
./scripts/build/build-universal.sh --platform windows --verbose

# Full production build
./scripts/build/build-universal.sh --clean --test --package --verbose
```

### CI/CD Integration
```bash
# Automated build for CI
./scripts/build/build-universal.sh \
    --platform linux \
    --build-type Release \
    --clean \
    --test \
    --package
```

## 🐛 Troubleshooting

### Common Issues

#### 1. Permission Denied
```bash
# Make script executable
chmod +x scripts/build/build-universal.sh
```

#### 2. Dependencies Missing
```bash
# Install missing dependencies
brew install cmake git          # macOS
sudo apt install cmake git      # Ubuntu
sudo yum install cmake git      # CentOS
```

#### 3. CMake Version Too Old
```bash
# Update CMake
brew upgrade cmake              # macOS
sudo apt upgrade cmake          # Ubuntu
```

#### 4. Docker Not Found
```bash
# Install Docker
brew install --cask docker      # macOS
sudo apt install docker.io      # Ubuntu
```

#### 5. Wine Not Found
```bash
# Install Wine
brew install --cask wine-stable # macOS
sudo apt install wine           # Ubuntu
```

### Debug Mode
```bash
# Enable verbose output
./scripts/build/build-universal.sh --verbose

# Check script syntax
bash -n scripts/build/build-universal.sh

# Run with debug logging
bash -x scripts/build/build-universal.sh
```

## 📊 Performance Tips

### Build Optimization
- **Use Release mode** for production builds
- **Enable parallel builds** (automatic)
- **Clean builds** when switching configurations
- **Use native builds** for fastest compilation

### Resource Management
- **Monitor CPU usage** during builds
- **Ensure sufficient RAM** (4GB+ recommended)
- **Use SSD storage** for faster I/O
- **Close unnecessary applications** during builds

## 🔒 Security Considerations

- **Read-only operations** - No system modifications
- **Isolated builds** - Container builds are sandboxed
- **Dependency verification** - Uses trusted package sources
- **No network access** - Builds are offline-capable

## 🧪 Testing

The script includes comprehensive testing:

```bash
# Run tests after build
./scripts/build/build-universal.sh --test

# Test script functionality
./scripts/build/build-universal.sh --help
./scripts/build/build-universal.sh --version
```

## 📚 Integration

### With Existing Build System
- **CMake integration** - Uses existing CMakeLists.txt
- **vcpkg support** - Automatic dependency management
- **Test framework** - Integrates with Google Test
- **CI/CD ready** - Command-line interface for automation

### With Development Workflow
- **Git integration** - Automatic repository detection
- **Version tracking** - Build version information
- **Package management** - Distribution package creation
- **Cross-platform** - Single script for all platforms

## 🔮 Future Enhancements

- **Cloud build support** - Remote compilation
- **Binary distribution** - Pre-built packages
- **Plugin system** - Extensible build methods
- **Performance profiling** - Build time optimization
- **Dependency caching** - Faster subsequent builds

## 📞 Support

For issues and questions:

1. **Check troubleshooting section** above
2. **Review script help**: `./build-universal.sh --help`
3. **Enable verbose mode**: `--verbose` flag
4. **Check dependencies**: Script reports missing requirements
5. **Review logs**: Color-coded output for debugging

---

**Ready to build? Run `./scripts/build/build-universal.sh` to get started!** 🚀
