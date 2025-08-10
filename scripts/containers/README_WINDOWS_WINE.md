# Windows in Ubuntu Wine Container Runner

## Quick Start

```bash
# Make script executable
chmod +x run-windows-in-ubuntu-wine.sh

# Create container with Wine
./run-windows-in-ubuntu-wine.sh --create

# Test Wine installation
./run-windows-in-ubuntu-wine.sh --wine-test

# Build Windows application
./run-windows-in-ubuntu-wine.sh --build

# Run Windows application
./run-windows-in-ubuntu-wine.sh --run --help
```

## What It Does

- Creates Ubuntu 24.04 Docker container
- Installs Wine 64-bit and 32-bit
- Installs MinGW cross-compiler for Windows targets
- Builds Windows applications using CMake
- Runs Windows applications through Wine

## Key Features

- **Cross-compilation**: Build Windows apps on Linux
- **Wine integration**: Run Windows apps in Linux container
- **Virtual display**: Xvfb for GUI applications
- **Container management**: Create, start, stop, delete
- **Wine configuration**: Automatic setup for compatibility

## Prerequisites

- Apple Container CLI (`brew install --cask docker`)
- macOS with Docker support
- Sufficient disk space

## Documentation

Full documentation: `docs/getting-started/WINDOWS_IN_UBUNTU_WINE.md`

## Help

```bash
./run-windows-in-ubuntu-wine.sh --help
```

## Status

```bash
./run-windows-in-ubuntu-wine.sh --status
```
