# Windows/Wine Quick Start

Quick start guide for running DEXArb on Windows using Wine or in Ubuntu container.

## 🚀 Quick Start with Wine

### Prerequisites
- **Ubuntu 20.04+** with Wine support
- **Docker** (optional, for containerized approach)
- **Git** and **CMake**

### Method 1: Native Wine Setup
```bash
# Install Wine
sudo apt update
sudo apt install wine64 wine32

# Install MinGW for cross-compilation
sudo apt install mingw-w64

# Clone and build
git clone <repository-url>
cd DEXArb
mkdir build && cd build
cmake .. -G "MinGW Makefiles"
mingw32-make
```

### Method 2: Docker with Wine
```bash
# Use the provided Wine container
docker run -it --rm \
  -v $(pwd):/workspace \
  -w /workspace \
  ubuntu:20.04

# Inside container, install Wine and build tools
apt update && apt install -y wine64 wine32 mingw-w64 cmake git
```

## 🐳 Container Approach (Recommended)

### Using the Wine Container Script
```bash
# Run the provided script
./scripts/containers/run-windows-wine-container.sh

# Or manually
docker run -it --rm \
  -v $(pwd):/workspace \
  -w /workspace \
  -e DISPLAY=$DISPLAY \
  -v /tmp/.X11-unix:/tmp/.X11-unix \
  dexarb-wine:latest
```

### Building the Wine Container
```bash
# Build custom Wine container
docker build -f scripts/containers/Dockerfile.wine -t dexarb-wine .

# Run with workspace mounted
docker run -it --rm \
  -v $(pwd):/workspace \
  -w /workspace \
  dexarb-wine:latest
```

## ⚙️ Configuration

### Wine-Specific Settings
```bash
# Configure Wine for better performance
export WINEPREFIX=~/.wine
winecfg

# Set Windows version to Windows 10
# Enable CSMT for better graphics performance
```

### Environment Variables
```bash
# Set Wine environment
export WINEARCH=win64
export WINEPREFIX=~/.wine64

# Set compiler
export CC=x86_64-w64-mingw32-gcc
export CXX=x86_64-w64-mingw32-g++
```

## 🔧 Troubleshooting

### Common Wine Issues
```bash
# Wine not found
sudo apt install wine64 wine32

# 32-bit support missing
sudo dpkg --add-architecture i386
sudo apt update
sudo apt install wine32

# Graphics issues
winecfg
# Set to Windows 10 and enable CSMT
```

### Build Issues
```bash
# MinGW not found
sudo apt install mingw-w64

# CMake generator issues
cmake .. -G "MinGW Makefiles" -DCMAKE_C_COMPILER=x86_64-w64-mingw32-gcc

# Library linking issues
# Ensure all dependencies are compiled for Windows
```

## 📊 Performance Tips

### Wine Optimization
- Use **Wine 6.0+** for better performance
- Enable **CSMT** in winecfg
- Set **Windows version** to Windows 10
- Use **64-bit Wine** when possible

### Build Optimization
- Use **Release build** for production
- Enable **optimizations** in CMake
- Use **parallel builds** with `-j$(nproc)`

## 🚀 Next Steps

After successful Wine setup:
- [Build the project](../build/README.md)
- [Configure the application](../configuration.md)
- [Run your first scan](../first-steps.md)

---

*Wine setup complete? Let's build DEXArb for Windows!*
