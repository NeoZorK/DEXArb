# Platform Setup Guide

This guide covers setting up DEXArb on different platforms and operating systems.

## 🐧 Linux Platforms

### Ubuntu 18.04+ / Debian 10+
```bash
# Update system
sudo apt update && sudo apt upgrade -y

# Install essential packages
sudo apt install -y build-essential cmake git curl wget

# Install development libraries
sudo apt install -y libssl-dev libcurl4-openssl-dev nlohmann-json3-dev

# Install additional tools
sudo apt install -y pkg-config autoconf automake libtool

# Verify installation
gcc --version
cmake --version
git --version
```

### Alpine Linux 3.14+
```bash
# Update package index
apk update

# Install build tools
apk add --no-cache build-base cmake git

# Install development libraries
apk add --no-cache openssl-dev curl-dev nlohmann-json-dev

# Install additional tools
apk add --no-cache pkgconfig autoconf automake libtool

# Verify installation
gcc --version
cmake --version
git --version
```

### CentOS 7+ / RHEL 7+
```bash
# Enable EPEL repository
sudo yum install -y epel-release

# Install development tools
sudo yum groupinstall -y "Development Tools"

# Install individual packages
sudo yum install -y cmake git openssl-devel curl-devel

# Install nlohmann-json (may need to build from source)
git clone https://github.com/nlohmann/json.git
cd json
mkdir build && cd build
cmake ..
make install

# Verify installation
gcc --version
cmake --version
git --version
```

## 🍎 macOS

### macOS 10.15+ (Catalina)
```bash
# Install Homebrew (if not installed)
/bin/bash -c "$(curl -fsSL https://raw.githubusercontent.com/Homebrew/install/HEAD/install.sh)"

# Install build tools
brew install cmake git

# Install development libraries
brew install openssl curl nlohmann-json

# Install additional tools
brew install pkg-config autoconf automake libtool

# Set up environment variables
echo 'export PATH="/usr/local/opt/openssl/bin:$PATH"' >> ~/.zshrc
echo 'export LDFLAGS="-L/usr/local/opt/openssl/lib"' >> ~/.zshrc
echo 'export CPPFLAGS="-I/usr/local/opt/openssl/include"' >> ~/.zshrc

# Reload shell configuration
source ~/.zshrc

# Verify installation
gcc --version
cmake --version
git --version
```

### Using MacPorts (Alternative)
```bash
# Install MacPorts from macports.org

# Install build tools
sudo port install cmake git

# Install development libraries
sudo port install openssl curl nlohmann-json

# Install additional tools
sudo port install pkgconfig autoconf automake libtool

# Verify installation
gcc --version
cmake --version
git --version
```

## 🪟 Windows

### Native Windows 10+ (MSVC)
```bash
# Install Visual Studio 2019/2022 Community Edition
# Include "Desktop development with C++" workload

# Install CMake
# Download from cmake.org or use winget
winget install Kitware.CMake

# Install Git
# Download from git-scm.com or use winget
winget install Git.Git

# Install vcpkg for dependencies
git clone https://github.com/Microsoft/vcpkg.git
cd vcpkg
./bootstrap-vcpkg.bat
./vcpkg integrate install

# Install required packages
./vcpkg install nlohmann-json cpr

# Verify installation
cmake --version
git --version
```

### Windows with MinGW-w64
```bash
# Install MSYS2 from msys2.org

# Update package database
pacman -Syu

# Install build tools
pacman -S mingw-w64-x86_64-toolchain cmake git

# Install development libraries
pacman -S mingw-w64-x86_64-openssl mingw-w64-x86_64-curl mingw-w64-x86_64-nlohmann-json

# Add MinGW to PATH
echo 'export PATH="/mingw64/bin:$PATH"' >> ~/.bashrc
source ~/.bashrc

# Verify installation
gcc --version
cmake --version
git --version
```

### Windows with WSL2
```bash
# Enable WSL2 in Windows
# Install Ubuntu from Microsoft Store

# Follow Ubuntu setup instructions above
# Build Linux version in Windows environment
```

## 🐳 Container Platforms

### Docker
```dockerfile
# Base image for development
FROM ubuntu:20.04

# Set environment variables
ENV DEBIAN_FRONTEND=noninteractive
ENV TZ=UTC

# Install system packages
RUN apt update && apt install -y \
    build-essential \
    cmake \
    git \
    curl \
    wget \
    libssl-dev \
    libcurl4-openssl-dev \
    nlohmann-json3-dev \
    pkg-config \
    autoconf \
    automake \
    libtool \
    && rm -rf /var/lib/apt/lists/*

# Set working directory
WORKDIR /app

# Copy source code
COPY . .

# Build the project
RUN mkdir build && cd build && \
    cmake .. && \
    make -j$(nproc)

# Set entry point
CMD ["./build/DEXArb"]
```

### Docker Compose
```yaml
version: '3.8'
services:
  dexarb:
    build: .
    volumes:
      - ./data:/app/data
      - ./logs:/app/logs
    environment:
      - NETWORK=ethereum
      - LOG_LEVEL=info
    ports:
      - "8080:8080"
```

## 🔧 Platform-Specific Configuration

### Linux Environment Variables
```bash
# Add to ~/.bashrc or ~/.zshrc
export CC=gcc
export CXX=g++
export CMAKE_BUILD_TYPE=Release
export CMAKE_CXX_FLAGS="-O3 -march=native"
```

### macOS Environment Variables
```bash
# Add to ~/.zshrc
export CC=clang
export CXX=clang++
export CMAKE_BUILD_TYPE=Release
export CMAKE_CXX_FLAGS="-O3"
```

### Windows Environment Variables
```cmd
# Set in System Properties > Environment Variables
set CC=cl
set CXX=cl
set CMAKE_BUILD_TYPE=Release
```

## 📊 Platform Comparison

| Platform | Compiler | Package Manager | Pros | Cons |
|----------|----------|----------------|------|------|
| **Ubuntu** | GCC | apt | Stable, well-supported | Larger footprint |
| **Alpine** | GCC | apk | Lightweight, secure | Limited packages |
| **macOS** | Clang | Homebrew/MacPorts | Native development | Apple-specific |
| **Windows** | MSVC | vcpkg | Native Windows | Complex setup |
| **WSL** | GCC | apt | Linux in Windows | Performance overhead |

## 🚀 Quick Platform Setup

### One-liner for Ubuntu
```bash
curl -fsSL https://raw.githubusercontent.com/DEXArb/setup/main/ubuntu-setup.sh | bash
```

### One-liner for Alpine
```bash
curl -fsSL https://raw.githubusercontent.com/DEXArb/setup/main/alpine-setup.sh | bash
```

### One-liner for macOS
```bash
curl -fsSL https://raw.githubusercontent.com/DEXArb/setup/main/macos-setup.sh | bash
```

## 🔍 Verification Commands

### Check Platform
```bash
# Operating system
uname -a
cat /etc/os-release  # Linux
sw_vers              # macOS
ver                  # Windows

# Architecture
uname -m
arch
```

### Check Tools
```bash
# Compiler
gcc --version
clang --version

# Build tools
cmake --version
make --version

# Package managers
apt --version        # Ubuntu/Debian
apk --version        # Alpine
brew --version       # macOS
```

## 🆘 Platform-Specific Issues

### Linux Issues
- **Permission denied**: Use `sudo` for system-wide installations
- **Library not found**: Install development packages (`-dev` suffix)
- **Compiler not found**: Install `build-essential` package

### macOS Issues
- **OpenSSL not found**: Use Homebrew version, set environment variables
- **Command line tools**: Install Xcode Command Line Tools
- **Permission issues**: Check Homebrew permissions

### Windows Issues
- **Path not found**: Add tools to system PATH
- **Build tools missing**: Install Visual Studio Build Tools
- **Dependencies**: Use vcpkg for C++ libraries

## 📚 Next Steps

After platform setup:
- [Build the project](../build/README.md)
- [Configure the application](../configuration.md)
- [Run your first scan](../first-steps.md)

---

*Platform ready? Let's build DEXArb!*
