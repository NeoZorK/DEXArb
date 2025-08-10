# Alpine Linux Container Runner

This document describes how to run NeoZorKDEXArb in an Alpine Linux container with musl libc using Apple Container CLI.

## Overview

The Alpine Linux container runner provides a lightweight, secure environment for running the DEX Arbitrage Scanner. It uses musl libc instead of glibc, making it ideal for containerized deployments.

## Prerequisites

- macOS with Apple Silicon (M1/M2/M3)
- Apple Container CLI installed (`brew install --cask docker`)
- Git repository cloned

## Quick Start

### 1. Run with default settings

```bash
./scripts/run-alpine-simple.sh
```

This will:
- Check if Alpine binary exists
- Build if necessary (main app only, no tests)
- Run the application in Alpine container
- Show help by default

### 2. Run with specific commands

```bash
# Show help
./scripts/run-alpine-simple.sh -- --help

# Show version
./scripts/run-alpine-simple.sh -- -version

# Show supported DEXes for Ethereum
./scripts/run-alpine-simple.sh -- -showDEXES ethereum

# Scan Ethereum blockchain
./scripts/run-alpine-simple.sh -- -scan ethereum 5000
```

### 3. Force rebuild

```bash
./scripts/run-alpine-simple.sh --build
```

## Script Options

### `run-alpine-simple.sh`

- **`-h, --help`**: Show script help
- **`-b, --build`**: Force rebuild Alpine binary
- **`-v, --verbose`**: Enable verbose output
- **`--`**: Pass remaining arguments to the application

### `run-alpine-container.sh`

- Full build with tests (may have compilation issues)
- More comprehensive but slower

## Container Details

### Base Image
- **Alpine Linux**: Latest version (3.22+)
- **Architecture**: ARM64 (Apple Silicon optimized)
- **Libc**: musl (lightweight alternative to glibc)

### Runtime Dependencies
```bash
apk add --no-cache \
    libstdc++ \
    libgcc \
    libcurl \
    ca-certificates
```

### Build Dependencies
```bash
apk add --no-cache \
    alpine-sdk \
    cmake \
    g++ \
    curl-dev \
    nlohmann-json \
    git
```

## Build Process

1. **Container Setup**: Downloads and installs Alpine Linux
2. **Dependencies**: Installs build tools and libraries
3. **CMake Configuration**: Configures with `-DBUILD_TESTING=OFF`
4. **Compilation**: Builds only the main application
5. **Runtime**: Installs minimal runtime dependencies

## Performance Characteristics

### Advantages
- **Lightweight**: ~15MB runtime dependencies
- **Fast startup**: Minimal container overhead
- **Security**: Reduced attack surface
- **Portability**: musl compatibility

### Considerations
- **Build time**: ~2-3 minutes for first build
- **Memory**: ~300MB during build, ~15MB runtime
- **Network**: Requires internet for package downloads

## Troubleshooting

### Common Issues

1. **Apple Container CLI not found**
   ```bash
   brew install --cask docker
   ```

2. **Build failures**
   ```bash
   # Clean and rebuild
   ./scripts/run-alpine-simple.sh --build
   ```

3. **Permission denied**
   ```bash
   chmod +x scripts/run-alpine-simple.sh
   ```

### Debug Mode

```bash
./scripts/run-alpine-simple.sh -v -- --help
```

## Examples

### Basic Usage
```bash
# Show help
./scripts/run-alpine-simple.sh

# Show version
./scripts/run-alpine-simple.sh -- -version

# Show help
./scripts/run-alpine-simple.sh -- --help
```

### Blockchain Operations
```bash
# Show Ethereum DEXes
./scripts/run-alpine-simple.sh -- -showDEXES ethereum

# Show BSC DEXes
./scripts/run-alpine-simple.sh -- -showDEXES bsc

# Show Polygon DEXes
./scripts/run-alpine-simple.sh -- -showDEXES polygon
```

### Scanning Operations
```bash
# Scan Ethereum (5000 blocks)
./scripts/run-alpine-simple.sh -- -scan ethereum 5000

# Scan BSC (10000 blocks)
./scripts/run-alpine-simple.sh -- -scan bsc 10000
```

## Integration

### CI/CD Pipeline
```yaml
# Example GitHub Actions step
- name: Test Alpine Container
  run: |
    chmod +x scripts/run-alpine-simple.sh
    ./scripts/run-alpine-simple.sh -- --help
```

### Docker Alternative
```bash
# If you prefer Docker over Apple Container
docker run --rm -v $(pwd):/workspace -w /workspace \
    alpine:latest sh -c "
        apk add --no-cache libstdc++ libgcc libcurl ca-certificates
        cd /workspace
        ./build-alpine/bin/NeoZorKDEXArb --help
    "
```

## Security Notes

- Container runs with minimal privileges
- No persistent storage outside workspace
- Network access limited to package downloads
- Runtime dependencies verified from Alpine repositories

## Performance Tips

1. **First run**: Allow 2-3 minutes for initial build
2. **Subsequent runs**: ~10-15 seconds for container startup
3. **Memory usage**: ~15MB for runtime, ~300MB during build
4. **Network**: ~15MB download for runtime dependencies

## Support

For issues with Alpine container runner:
1. Check script permissions: `chmod +x scripts/run-alpine-simple.sh`
2. Verify Apple Container CLI: `container --version`
3. Clean build: `./scripts/run-alpine-simple.sh --build`
4. Check logs for specific error messages
