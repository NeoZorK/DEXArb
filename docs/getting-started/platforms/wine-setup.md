# Wine Setup Guide

Comprehensive guide for setting up Wine environment for DEXArb development and testing.

## 🍷 Overview

Wine (Wine Is Not an Emulator) allows you to run Windows applications on Linux and macOS. This guide covers setting up Wine for DEXArb development and testing.

## 🚀 Installation

### Ubuntu/Debian
```bash
# Add 32-bit architecture support
sudo dpkg --add-architecture i386

# Update package list
sudo apt update

# Install Wine
sudo apt install wine64 wine32

# Install additional Wine tools
sudo apt install wine-stable winehq-stable
```

### CentOS/RHEL
```bash
# Enable EPEL repository
sudo yum install -y epel-release

# Install Wine
sudo yum install -y wine wine-devel

# Install 32-bit support
sudo yum install -y wine.i686 wine-devel.i686
```

### macOS
```bash
# Using Homebrew
brew install --cask wine-stable

# Using MacPorts
sudo port install wine
```

## ⚙️ Initial Configuration

### First Run Setup
```bash
# Initialize Wine (creates ~/.wine directory)
winecfg

# Set Windows version to Windows 10
# Enable CSMT for better performance
# Configure graphics settings
```

### Wine Prefix Configuration
```bash
# Create 64-bit Wine prefix
export WINEARCH=win64
export WINEPREFIX=~/.wine64
wineboot --init

# Configure the new prefix
WINEPREFIX=~/.wine64 winecfg
```

## 🛠️ Development Tools

### MinGW Cross-Compiler
```bash
# Install MinGW for Windows cross-compilation
sudo apt install mingw-w64

# Verify installation
x86_64-w64-mingw32-gcc --version
x86_64-w64-mingw32-g++ --version

# Set environment variables
export CC=x86_64-w64-mingw32-gcc
export CXX=x86_64-w64-mingw32-g++
```

### CMake Configuration
```bash
# Install CMake
sudo apt install cmake

# Configure for MinGW
cmake .. -G "MinGW Makefiles" \
  -DCMAKE_C_COMPILER=x86_64-w64-mingw32-gcc \
  -DCMAKE_CXX_COMPILER=x86_64-w64-mingw32-g++ \
  -DCMAKE_BUILD_TYPE=Release
```

## 🔧 Wine Configuration

### Graphics Settings
```bash
# Open Wine configuration
winecfg

# Graphics tab:
# - Enable CSMT (Command Stream Multi-Threading)
# - Set video memory size
# - Enable Direct3D 11
# - Enable OpenGL
```

### Audio Settings
```bash
# Audio tab in winecfg:
# - Select audio driver (ALSA recommended)
# - Configure sample rate
# - Set buffer size
```

### Windows Version
```bash
# Applications tab:
# - Set Windows version to Windows 10
# - Apply to all applications
# - Override for specific applications if needed
```

## 📁 File System Integration

### Drive Mapping
```bash
# Configure drive mappings in winecfg
# - C: drive maps to ~/.wine/drive_c
# - Z: drive maps to root filesystem
# - Custom mappings for project directories
```

### Symbolic Links
```bash
# Create symbolic links for easy access
ln -s ~/projects/DEXArb ~/.wine/drive_c/DEXArb
ln -s ~/downloads ~/.wine/drive_c/Downloads
```

## 🚀 Building DEXArb

### Project Setup
```bash
# Clone repository
git clone <repository-url>
cd DEXArb

# Create build directory
mkdir build && cd build

# Configure with MinGW
cmake .. -G "MinGW Makefiles" \
  -DCMAKE_C_COMPILER=x86_64-w64-mingw32-gcc \
  -DCMAKE_CXX_COMPILER=x86_64-w64-mingw32-g++ \
  -DCMAKE_BUILD_TYPE=Release \
  -DCMAKE_INSTALL_PREFIX=/usr/local
```

### Build Process
```bash
# Build the project
mingw32-make -j$(nproc)

# Install (optional)
mingw32-make install

# Run tests
ctest --output-on-failure
```

## 🔍 Troubleshooting

### Common Issues

#### Wine Not Starting
```bash
# Check Wine installation
wine --version

# Check Wine prefix
ls -la ~/.wine

# Reinstall Wine if needed
sudo apt remove --purge wine* && sudo apt autoremove
sudo apt install wine64 wine32
```

#### Build Failures
```bash
# Check compiler availability
which x86_64-w64-mingw32-gcc

# Verify library paths
echo $PATH
echo $LD_LIBRARY_PATH

# Check CMake configuration
cmake --help | grep "MinGW Makefiles"
```

#### Performance Issues
```bash
# Monitor Wine performance
winecfg
# Check graphics and memory settings

# Monitor system resources
htop
free -h
```

### Debug Information
```bash
# Enable Wine debugging
export WINEDEBUG=+all

# Check Wine prefix structure
ls -la ~/.wine

# Verify registry
wine regedit

# Check Wine logs
tail -f ~/.wine/dosdevices/c:/windows/system32/wine.log
```

## 📊 Performance Optimization

### Wine Performance Settings
```bash
# Enable CSMT
winecfg
# Graphics tab -> Enable CSMT

# Optimize memory
winecfg
# Memory tab -> Adjust as needed

# Set Windows version
winecfg
# Applications tab -> Windows 10
```

### System Optimization
```bash
# Increase shared memory
sudo sysctl -w kernel.shmmax=2147483648

# Optimize file system
# Use ext4 or XFS for better performance

# Monitor and adjust
watch -n 1 'free -h && echo "---" && df -h'
```

## 🔒 Security Considerations

### User Permissions
```bash
# Run Wine as regular user (not root)
# Wine creates files in ~/.wine with user permissions

# Secure Wine prefix
chmod 700 ~/.wine
chmod 700 ~/.wine64
```

### Network Security
```bash
# Wine applications can access network
# Monitor network connections
netstat -tulpn | grep wine

# Use firewall rules if needed
sudo ufw deny out from any to any app wine
```

## 🔄 Maintenance

### Regular Updates
```bash
# Update Wine
sudo apt update && sudo apt upgrade wine*

# Update MinGW
sudo apt update && sudo apt upgrade mingw-w64

# Clean Wine prefix
rm -rf ~/.wine
wineboot --init
```

### Backup and Recovery
```bash
# Backup Wine configuration
tar -czf wine-config-backup.tar.gz ~/.wine

# Backup Wine prefix
tar -czf wine-prefix-backup.tar.gz ~/.wine64

# Restore from backup
tar -xzf wine-config-backup.tar.gz -C ~/
```

## 🚀 Advanced Usage

### Multiple Wine Prefixes
```bash
# Create separate prefixes for different applications
export WINEPREFIX=~/.wine-dexarb winecfg
export WINEPREFIX=~/.wine-testing winecfg

# Use specific prefix
WINEPREFIX=~/.wine-dexarb wine DEXArb.exe
```

### Wine Development
```bash
# Install Wine development headers
sudo apt install wine-dev

# Build Wine from source (advanced)
git clone https://github.com/wine-mirror/wine.git
cd wine
./configure --prefix=/usr/local
make -j$(nproc)
sudo make install
```

## 📚 Next Steps

After setting up Wine:
- [Build DEXArb for Windows](../build/README.md)
- [Configure the application](../configuration.md)
- [Run your first scan](../first-steps.md)
- [Deploy to production](../../deployment/README.md)

---

*Wine setup complete? Let's build DEXArb for Windows!*
