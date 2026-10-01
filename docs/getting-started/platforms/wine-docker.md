# Wine Docker Usage Guide

Comprehensive guide for using DEXArb with Wine in Docker containers for cross-platform compatibility.

## 🐳 Overview

This guide covers running DEXArb on Windows using Wine within Docker containers, providing a consistent environment across different host systems.

## 🚀 Quick Start

### Prerequisites
- **Docker** installed and running
- **Git** for cloning the repository
- **Linux/macOS** host system (Windows users can use WSL2)

### Basic Wine Container
```bash
# Pull the official Wine container
docker pull ubuntu:20.04

# Run interactive Wine container
docker run -it --rm \
  -v $(pwd):/workspace \
  -w /workspace \
  ubuntu:20.04

# Inside container, install Wine and build tools
apt update && apt install -y wine64 wine32 mingw-w64 cmake git
```

## 🏗️ Custom Wine Container

### Dockerfile
```dockerfile
# Dockerfile.wine
FROM ubuntu:20.04

# Set environment variables
ENV DEBIAN_FRONTEND=noninteractive
ENV TZ=UTC
ENV WINEARCH=win64
ENV WINEPREFIX=/root/.wine64

# Install system dependencies
RUN apt update && apt install -y \
    wine64 \
    wine32 \
    mingw-w64 \
    cmake \
    git \
    curl \
    wget \
    pkg-config \
    && rm -rf /var/lib/apt/lists/*

# Configure Wine
RUN winecfg /v win10 && \
    wineserver -k

# Set working directory
WORKDIR /workspace

# Create wine prefix
RUN wineboot --init

# Expose port for DEXArb
EXPOSE 8080

# Default command
CMD ["/bin/bash"]
```

### Build and Run
```bash
# Build the Wine container
docker build -f Dockerfile.wine -t dexarb-wine .

# Run with workspace mounted
docker run -it --rm \
  -v $(pwd):/workspace \
  -w /workspace \
  dexarb-wine:latest
```

## ⚙️ Advanced Configuration

### Wine Configuration
```bash
# Inside container, configure Wine
winecfg

# Set Windows version to Windows 10
# Enable CSMT for better performance
# Configure graphics settings
```

### Environment Variables
```bash
# Set Wine environment
export WINEARCH=win64
export WINEPREFIX=/root/.wine64
export WINEDEBUG=-all

# Set compiler environment
export CC=x86_64-w64-mingw32-gcc
export CXX=x86_64-w64-mingw32-g++
export CMAKE_GENERATOR="MinGW Makefiles"
```

## 🔧 Building DEXArb

### CMake Configuration
```bash
# Configure with MinGW
cmake .. -G "MinGW Makefiles" \
  -DCMAKE_C_COMPILER=x86_64-w64-mingw32-gcc \
  -DCMAKE_CXX_COMPILER=x86_64-w64-mingw32-g++ \
  -DCMAKE_BUILD_TYPE=Release

# Build the project
mingw32-make -j$(nproc)
```

### Dependency Management
```bash
# Install Windows dependencies
# Note: Some libraries may need to be compiled for Windows

# For nlohmann-json
# Download and install Windows version

# For OpenSSL
# Use MinGW-compatible version
```

## 📊 Performance Optimization

### Wine Performance Settings
```bash
# Enable CSMT (Command Stream Multi-Threading)
winecfg
# Graphics tab -> Enable CSMT

# Set Windows version to Windows 10
# Advanced tab -> Windows version

# Optimize memory settings
# Memory tab -> Adjust as needed
```

### Container Optimization
```bash
# Run with performance optimizations
docker run -it --rm \
  --cpus=4 \
  --memory=8g \
  --shm-size=1g \
  -v $(pwd):/workspace \
  -w /workspace \
  dexarb-wine:latest
```

## 🔍 Troubleshooting

### Common Issues

#### Wine Not Starting
```bash
# Check Wine installation
wine --version

# Reinstall Wine if needed
apt remove --purge wine* && apt autoremove
apt install wine64 wine32
```

#### Build Failures
```bash
# Check compiler availability
x86_64-w64-mingw32-gcc --version

# Verify CMake generator
cmake --help | grep "MinGW Makefiles"

# Check library paths
pkg-config --list-all
```

#### Performance Issues
```bash
# Monitor Wine performance
winecfg
# Check graphics and memory settings

# Monitor container resources
docker stats
```

### Debug Information
```bash
# Enable Wine debugging
export WINEDEBUG=+all

# Check Wine prefix
ls -la ~/.wine64

# Verify Wine registry
wine regedit
```

## 🚀 Production Deployment

### Persistent Wine Container
```bash
# Create named container for persistence
docker run -d --name dexarb-wine \
  -v $(pwd):/workspace \
  -w /workspace \
  dexarb-wine:latest

# Execute commands in running container
docker exec -it dexarb-wine bash

# Stop and remove when done
docker stop dexarb-wine
docker rm dexarb-wine
```

### Multi-Stage Build
```dockerfile
# Build stage
FROM ubuntu:20.04 AS builder
RUN apt update && apt install -y wine64 wine32 mingw-w64 cmake git
COPY . /workspace
WORKDIR /workspace
RUN cmake .. -G "MinGW Makefiles" && mingw32-make

# Runtime stage
FROM ubuntu:20.04
RUN apt update && apt install -y wine64 wine32
COPY --from=builder /workspace/build/DEXArb.exe /app/
WORKDIR /app
CMD ["wine", "DEXArb.exe"]
```

## 📚 Integration with Host

### Volume Mounting
```bash
# Mount host directories
docker run -it --rm \
  -v $(pwd):/workspace \
  -v ~/.wine:/root/.wine \
  -v /tmp/.X11-unix:/tmp/.X11-unix \
  -e DISPLAY=$DISPLAY \
  dexarb-wine:latest
```

### Network Access
```bash
# Expose network ports
docker run -it --rm \
  -p 8080:8080 \
  -v $(pwd):/workspace \
  dexarb-wine:latest
```

## 🔄 Automation

### Build Script
```bash
#!/bin/bash
# build-wine.sh

set -e

echo "Building DEXArb for Windows using Wine..."

# Build container if needed
if [[ "$(docker images -q dexarb-wine:latest 2> /dev/null)" == "" ]]; then
    echo "Building Wine container..."
    docker build -f Dockerfile.wine -t dexarb-wine .
fi

# Run build in container
docker run --rm \
  -v $(pwd):/workspace \
  -w /workspace \
  dexarb-wine:latest \
  bash -c "
    cmake .. -G 'MinGW Makefiles' \
      -DCMAKE_C_COMPILER=x86_64-w64-mingw32-gcc \
      -DCMAKE_CXX_COMPILER=x86_64-w64-mingw32-g++ \
      -DCMAKE_BUILD_TYPE=Release
    mingw32-make -j$(nproc)
  "

echo "Build completed successfully!"
```

## 📚 Next Steps

After setting up Wine in Docker:
- [Build the project](../build/README.md)
- [Configure the application](../configuration.md)
- [Run your first scan](../first-steps.md)
- [Deploy to production](../../deployment/README.md)

---

*Wine Docker setup complete? Let's build and run DEXArb!*
