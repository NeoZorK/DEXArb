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

## 🚀 Quick Start

```bash
# Make scripts executable
chmod +x scripts/container/*.sh

# Run in Alpine (lightweight)
./scripts/container/run-alpine-simple.sh

# Run in Ubuntu (full compatibility)
./scripts/container/run-ubuntu-container.sh

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
```

## 📚 Documentation

- [Alpine Container Runner](../../docs/getting-started/ALPINE_CONTAINER_RUNNER.md)
- [Ubuntu Container Runner](../../docs/getting-started/UBUNTU_CONTAINER_RUNNER.md)
- [Getting Started](../../docs/getting-started/README.md)

---

**Created**: 26.02.2025  
**Author**: Rostyslav S.  
**Version**: 1.0.7
