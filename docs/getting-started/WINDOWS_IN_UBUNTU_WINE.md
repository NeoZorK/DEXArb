# Windows Application Runner in Ubuntu Docker Container with Wine

This document describes how to run Windows applications inside an Ubuntu Docker container using Wine.

## Overview

The `run-windows-in-ubuntu-wine.sh` script provides a complete solution for:
- Creating Ubuntu Docker containers with Wine support
- Building Windows applications using MinGW cross-compiler
- Running Windows applications through Wine
- Managing container lifecycle and Wine configuration

## Prerequisites

- Apple Container CLI (install with `brew install --cask docker`)
- macOS with Docker support
- Sufficient disk space for Ubuntu container and Wine

## Quick Start

### 1. Create and Setup Container

```bash
./scripts/container/run-windows-in-ubuntu-wine.sh --create
```

This will:
- Create Ubuntu 24.04 container
- Install Wine 64-bit and 32-bit
- Install MinGW cross-compiler tools
- Install development dependencies
- Configure Wine for better compatibility

### 2. Test Wine Installation

```bash
./scripts/container/run-windows-in-ubuntu-wine.sh --wine-test
```

Verify that Wine is working correctly in the container.

### 3. Build Windows Application

```bash
./scripts/container/run-windows-in-ubuntu-wine.sh --build
```

Build the application using MinGW cross-compiler targeting Windows.

### 4. Run Windows Application

```bash
./scripts/container/run-windows-in-ubuntu-wine.sh --run --help
```

Run the Windows application with arguments.

## Script Options

| Option | Description |
|--------|-------------|
| `--create` | Create and setup container with Wine |
| `--build` | Build Windows application in container |
| `--run` | Run Windows application in container |
| `--shell` | Open interactive shell in container |
| `--stop` | Stop container |
| `--delete` | Delete container |
| `--status` | Show container status |
| `--clean` | Clean build artifacts |
| `--rebuild` | Force rebuild container and application |
| `--wine-config` | Configure Wine settings |
| `--wine-test` | Test Wine installation |

## Container Architecture

- **Base Image**: Ubuntu 24.04 (Noble)
- **Wine Version**: 64-bit and 32-bit support
- **Cross-Compiler**: MinGW-w64 for Windows targets
- **Build System**: CMake with Windows toolchain
- **Display**: Virtual framebuffer (Xvfb) for GUI applications

## Wine Configuration

The script automatically configures Wine for better compatibility:

- Sets 64-bit architecture (`WINEARCH=win64`)
- Configures DLL overrides for common libraries
- Sets up Wine prefix in `/root/.wine`
- Installs Microsoft fonts and Wine fonts

## Build Process

The build process uses CMake with Windows toolchain:

```bash
cmake .. \
    -DCMAKE_BUILD_TYPE=Release \
    -DCMAKE_SYSTEM_NAME=Windows \
    -DCMAKE_C_COMPILER=x86_64-w64-mingw32-gcc \
    -DCMAKE_CXX_COMPILER=x86_64-w64-mingw32-g++ \
    -DCMAKE_RC_COMPILER=x86_64-w64-mingw32-windres \
    -DCMAKE_FIND_ROOT_PATH=/usr/x86_64-w64-mingw32 \
    -DBUILD_TESTING=OFF
```

## Running Applications

Applications are run through Wine with proper environment setup:

- Wine prefix configuration
- Virtual display for GUI applications
- Argument passing from host to container
- Error handling and logging

## Troubleshooting

### Common Issues

1. **Container not found**: Run `--create` first
2. **Wine not working**: Use `--wine-test` to diagnose
3. **Build failures**: Check dependencies with `--shell`
4. **Display issues**: Virtual framebuffer is automatically configured

### Debug Commands

```bash
# Check container status
./scripts/container/run-windows-in-ubuntu-wine.sh --status

# Open shell for debugging
./scripts/container/run-windows-in-ubuntu-wine.sh --shell

# Test Wine installation
./scripts/container/run-windows-in-ubuntu-wine.sh --wine-test

# Rebuild everything
./scripts/container/run-windows-in-ubuntu-wine.sh --rebuild
```

## Examples

### Basic Workflow

```bash
# Setup environment
./scripts/container/run-windows-in-ubuntu-wine.sh --create
./scripts/container/run-windows-in-ubuntu-wine.sh --wine-test

# Build and run
./scripts/container/run-windows-in-ubuntu-wine.sh --build
./scripts/container/run-windows-in-ubuntu-wine.sh --run --help
```

### Development Workflow

```bash
# Open shell for development
./scripts/container/run-windows-in-ubuntu-wine.sh --shell

# Inside container shell
cd /workspace
mkdir -p build-windows-wine
cd build-windows-wine
cmake .. -DCMAKE_SYSTEM_NAME=Windows -DCMAKE_C_COMPILER=x86_64-w64-mingw32-gcc
make
```

### Cleanup

```bash
# Stop container
./scripts/container/run-windows-in-ubuntu-wine.sh --stop

# Delete container
./scripts/container/run-windows-in-ubuntu-wine.sh --delete

# Clean build artifacts
./scripts/container/run-windows-in-ubuntu-wine.sh --clean
```

## Performance Considerations

- Container startup takes time due to Wine initialization
- Build process is slower due to cross-compilation
- Wine adds overhead for Windows application execution
- Consider using `--rebuild` only when necessary

## Security Notes

- Container runs with root privileges inside
- Wine prefix is persistent across container restarts
- Build artifacts are shared with host system
- No network isolation by default

## Support

For issues and questions:
1. Check container status with `--status`
2. Test Wine installation with `--wine-test`
3. Use `--shell` for interactive debugging
4. Review logs and error messages
5. Consider `--rebuild` for complete reset
