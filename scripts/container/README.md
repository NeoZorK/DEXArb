# Container Scripts

This directory contains scripts for running DEXArb in different Linux container environments using Apple Container CLI.

## 🐳 Available Runners

### Alpine Linux Runner
- **File**: `run-alpine-simple.sh`
- **Use case**: Lightweight, fast startup
- **Best for**: Quick testing and development
- **Documentation**: [Alpine Container Runner](../../docs/getting-started/ALPINE_CONTAINER_RUNNER.md)

### Ubuntu Linux Runner
- **File**: `run-ubuntu-container.sh`
- **Use case**: Full Linux compatibility, glibc support
- **Best for**: Production-like environment, compatibility testing
- **Documentation**: [Ubuntu Container Runner](../../docs/getting-started/UBUNTU_CONTAINER_RUNNER.md)

### Windows Runner
- **File**: `run-windows-container.sh`
- **Use case**: Windows compatibility, MSVC testing
- **Best for**: Cross-platform testing, Windows deployment
- **Documentation**: [Windows Container Runner](../../docs/getting-started/WINDOWS_CONTAINER_RUNNER.md)

### Windows in Ubuntu Wine Runner
- **File**: `run-windows-in-ubuntu-wine.sh`
- **Use case**: Windows applications in Linux container via Wine
- **Best for**: Cross-compilation, Wine testing, Linux-based Windows development
- **Documentation**: [Windows in Ubuntu Wine](../../docs/getting-started/WINDOWS_IN_UBUNTU_WINE.md)

## 🚀 Quick Start

```bash
# Make scripts executable
chmod +x scripts/container/*.sh

# Run in Alpine (lightweight)
./scripts/container/run-alpine-simple.sh

# Run in Ubuntu (full compatibility)
./scripts/container/run-ubuntu-container.sh

# Run in Windows (MSVC compatibility)
./scripts/container/run-windows-container.sh

# Run Windows apps in Ubuntu via Wine
./scripts/container/run-windows-in-ubuntu-wine.sh --create

# Show help
./scripts/container/run-ubuntu-container.sh --help
```

## 🔧 Features

### Alpine Runner
- ✅ Fast container startup
- ✅ Small image size (~5MB)
- ✅ Musl libc compatibility
- ✅ Pre-built binary available

### Ubuntu Runner
- ✅ Full glibc compatibility
- ✅ Automatic dependency installation
- ✅ Modern CMake support (3.28+)
- ✅ Runtime dependency management
- ✅ Automatic build system

### Windows Runner
- ✅ Windows Server Core compatibility
- ✅ MSVC compiler support
- ✅ Ninja build system
- ✅ vcpkg dependency management
- ✅ Windows-specific optimizations

### Windows in Ubuntu Wine Runner
- ✅ Ubuntu 24.04 base with Wine support
- ✅ MinGW cross-compiler for Windows targets
- ✅ Wine 64-bit and 32-bit support
- ✅ Virtual display (Xvfb) for GUI apps
- ✅ Cross-compilation with CMake

## 📋 Prerequisites

1. **Apple Container CLI** installed and available
2. **macOS** operating system
3. **Internet connection** for container images
4. **Sufficient disk space** for builds

## 🎯 Use Cases

| Use Case | Recommended Runner | Reason |
|----------|-------------------|---------|
| Quick testing | Alpine | Fast startup, small footprint |
| Development | Alpine | Quick iteration cycles |
| Production testing | Ubuntu | Full compatibility |
| Dependency testing | Ubuntu | Complete Linux environment |
| Windows testing | Windows | MSVC compatibility |
| Cross-platform | Windows | Windows deployment testing |
| Wine testing | Ubuntu Wine | Windows apps in Linux |
| Cross-compilation | Ubuntu Wine | Windows targets on Linux |
| CI/CD pipelines | Ubuntu | Reliable build environment |

## 🔄 Maintenance

### Updating Scripts
```bash
# Make executable
chmod +x scripts/container/*.sh

# Check Apple Container CLI version
container --version
```

### Rebuilding Applications
```bash
# Alpine (uses pre-built binary)
./scripts/container/run-alpine-simple.sh

# Ubuntu (rebuilds automatically)
./scripts/container/run-ubuntu-container.sh --build

# Windows (rebuilds automatically)
./scripts/container/run-windows-container.sh --build
```

## 📚 Documentation

- [Alpine Container Runner](../../docs/getting-started/ALPINE_CONTAINER_RUNNER.md)
- [Ubuntu Container Runner](../../docs/getting-started/UBUNTU_CONTAINER_RUNNER.md)
- [Windows Container Runner](../../docs/getting-started/WINDOWS_CONTAINER_RUNNER.md)
- [Getting Started](../../docs/getting-started/README.md)

---

**Created**: 26.02.2025  
**Author**: Rostyslav S.  
**Version**: 1.0.7
