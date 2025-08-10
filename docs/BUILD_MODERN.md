# Modern Build System for NeoZorKDEXArb

## Overview

This document describes the modernized build system for NeoZorKDEXArb, which uses:
- **vcpkg** for dependency management
- **Apple Container CLI** for isolated build environments
- **CMake 3.28+** with C++23 support
- **Modern compiler optimizations**

## Prerequisites

### Required Software

1. **Xcode Command Line Tools** (for macOS)
   ```bash
   xcode-select --install
   ```

2. **CMake 3.28 or higher**
   ```bash
   # Install via Homebrew
   brew install cmake
   
   # Or download from https://cmake.org/download/
   ```

3. **Apple Container CLI**
   - Available with Xcode 15+ or Command Line Tools
   - Check installation: `container version`

4. **Git**
   ```bash
   # Usually pre-installed with Xcode
   # Or install via Homebrew: brew install git
   ```

### Optional Software

- **uv** for Python testing: `pip install uv`
- **pytest** for Python testing: `pip install pytest pytest-xdist`

## Build Methods

### 1. Container Build (Recommended)

Build using Apple Container for isolated, reproducible builds:

```bash
# Make script executable
chmod +x build-apple-container.sh

# Build with container
./build-apple-container.sh

# Build with container and create package
./build-apple-container.sh --package
```

**Advantages:**
- Isolated build environment
- Reproducible builds
- No system dependency conflicts
- Cross-platform compatibility

### 2. Local Build

Build locally with vcpkg dependencies:

```bash
# Build locally
./build-apple-container.sh --local

# Build locally and create package
./build-apple-container.sh --local --package
```

**Advantages:**
- Faster build times
- Direct access to system tools
- Easier debugging

## Build Process

### Step 1: Dependency Check

The build script automatically checks for:
- Apple Container CLI availability
- CMake version compatibility
- C++ compiler (clang++)
- Git availability

### Step 2: vcpkg Setup

Automatically clones and configures vcpkg:
```bash
git clone https://github.com/Microsoft/vcpkg.git
cd vcpkg
./bootstrap-vcpkg.sh
./vcpkg install curl nlohmann-json
```

### Step 3: Build Configuration

CMake configuration with vcpkg toolchain:
```bash
cmake .. -DCMAKE_TOOLCHAIN_FILE=../vcpkg/scripts/buildsystems/vcpkg.cmake
```

### Step 4: Compilation

Parallel compilation with system-optimized flags:
```bash
make -j$(sysctl -n hw.ncpu)
```

### Step 5: Testing

Automatic testing of:
- Executable functionality (help, version commands)
- Python tests (if pytest/uv available)

## Configuration

### vcpkg.json

Dependencies are managed in `vcpkg.json`:
```json
{
  "name": "neozork-dexarb",
  "version": "1.0.7",
  "dependencies": [
    {
      "name": "curl",
      "version>=": "8.0.0"
    },
    {
      "name": "nlohmann-json",
      "version>=": "3.11.0"
    }
  ],
  "builtin-baseline": "2024-12-19"
}
```

### CMake Configuration

Modern CMake features enabled:
- C++23 standard
- Target-based configuration
- Automatic dependency resolution
- Cross-platform compatibility

## Output Structure

```
build/
├── bin/
│   └── NeoZorKDEXArb          # Main executable
├── lib/                        # Libraries (if any)
└── CMakeCache.txt             # CMake cache
```

## Troubleshooting

### Common Issues

1. **Apple Container CLI not found**
   ```bash
   # Install Command Line Tools
   xcode-select --install
   
   # Verify installation
   container version
   ```

2. **CMake version too old**
   ```bash
   # Install via Homebrew
   brew install cmake
   
   # Check version
   cmake --version
   ```

3. **vcpkg build failures**
   ```bash
   # Clean and retry
   rm -rf vcpkg
   ./build-apple-container.sh
   ```

4. **Permission denied**
   ```bash
   # Make script executable
   chmod +x build-apple-container.sh
   ```

### Debug Mode

For detailed build information:
```bash
# Set debug environment variable
export CMAKE_BUILD_TYPE=Debug

# Run build with verbose output
./build-apple-container.sh --local
```

## Performance Optimization

### Compiler Flags

- **-O3**: Maximum optimization
- **-march=native**: CPU-specific optimizations
- **-fstack-protector-strong**: Security hardening
- **-D_FORTIFY_SOURCE=2**: Buffer overflow protection

### Parallel Build

Automatic detection of CPU cores:
```bash
# macOS
cores=$(sysctl -n hw.ncpu)

# Linux
cores=$(nproc)

# Use all cores for parallel compilation
make -j$cores
```

## Package Creation

Create deployment packages:
```bash
./build-apple-container.sh --package
```

Package contents:
- Executable binary
- Documentation
- Configuration files
- Archive format: `.tar.gz`

## Integration with CI/CD

### GitHub Actions Example

```yaml
name: Build and Test

on: [push, pull_request]

jobs:
  build:
    runs-on: macos-latest
    
    steps:
    - uses: actions/checkout@v4
    
    - name: Install dependencies
      run: |
        xcode-select --install
        brew install cmake
    
    - name: Build with container
      run: |
        chmod +x build-apple-container.sh
        ./build-apple-container.sh --container
    
    - name: Run tests
      run: |
        ./build-apple-container.sh --local
        ./build/bin/NeoZorKDEXArb -h
        ./build/bin/NeoZorKDEXArb -v
```

## Migration from Old Build System

### Changes Required

1. **Remove old build script**: `build-modern.sh`
2. **Update CI/CD pipelines** to use new script
3. **Update documentation** references
4. **Test new build process** on all target platforms

### Benefits of Migration

- **Dependency Management**: Automatic via vcpkg
- **Isolation**: Container-based builds
- **Performance**: Modern compiler optimizations
- **Maintainability**: Standardized build process
- **Reproducibility**: Consistent build environments

## Support

For build system issues:
1. Check prerequisites installation
2. Review troubleshooting section
3. Check CMake and vcpkg documentation
4. Verify Apple Container CLI availability

## Future Enhancements

Planned improvements:
- **Multi-stage builds** for smaller containers
- **Caching** for faster rebuilds
- **Cross-compilation** support
- **Integration** with package managers
- **Automated testing** in containers
