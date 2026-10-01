# Wine Reference Guide

Comprehensive reference guide for Wine usage with DEXArb, including configuration, troubleshooting, and advanced features.

## 🍷 Wine Overview

Wine (Wine Is Not an Emulator) is a compatibility layer that allows Windows applications to run on Unix-like systems. This guide covers Wine usage specifically for DEXArb development and testing.

## 📚 Wine Fundamentals

### What is Wine?
- **Compatibility Layer** - Translates Windows API calls to POSIX calls
- **Not an Emulator** - Runs Windows applications natively
- **Open Source** - Free software under LGPL license
- **Cross-Platform** - Works on Linux, macOS, and BSD

### Wine Architecture
```
┌─────────────────────────────────────────────────────────┐
│                 Wine Architecture                       │
├─────────────────────────────────────────────────────────┤
│  ┌─────────────┐  ┌─────────────┐  ┌─────────────┐    │
│  │  Windows    │  │    Wine     │  │   POSIX     │    │
│  │ Application │  │ Compatibility│  │   System    │    │
│  │             │  │    Layer    │  │             │    │
│  │ • DEXArb    │◄─┤ • API       │◄─┤ • Linux     │    │
│  │ • .exe      │  │ • Registry  │  │ • macOS     │    │
│  │ • DLLs      │  │ • File I/O  │  │ • BSD       │    │
│  └─────────────┘  └─────────────┘  └─────────────┘    │
└─────────────────────────────────────────────────────────┘
```

### Wine Components
- **Wine Core** - Main compatibility layer
- **Wine Server** - Inter-process communication
- **Wine Loader** - Executable loading and linking
- **Wine Registry** - Windows registry emulation
- **Wine Drivers** - Hardware and system drivers

## 🚀 Installation and Setup

### Linux Installation

#### Ubuntu/Debian
```bash
# Add 32-bit architecture support
sudo dpkg --add-architecture i386

# Update package list
sudo apt update

# Install Wine
sudo apt install wine64 wine32

# Install additional tools
sudo apt install wine-stable winehq-stable
```

#### Alpine Linux
```bash
# Install Wine
apk add --no-cache wine

# Install development tools
apk add --no-cache wine-dev
```

#### CentOS/RHEL
```bash
# Enable EPEL repository
sudo yum install -y epel-release

# Install Wine
sudo yum install -y wine wine-devel

# Install 32-bit support
sudo yum install -y wine.i686 wine-devel.i686
```

### macOS Installation
```bash
# Using Homebrew
brew install --cask wine-stable

# Using MacPorts
sudo port install wine
```

### Windows Installation
```bash
# Wine is not needed on Windows
# Use native Windows tools instead
```

## ⚙️ Configuration

### Wine Prefix
```bash
# Wine prefix is the virtual Windows environment
export WINEPREFIX=~/.wine-dexarb

# Create new prefix
export WINEARCH=win64
wineboot --init

# Configure prefix
WINEPREFIX=~/.wine-dexarb winecfg
```

### Wine Configuration (winecfg)
```bash
# Open Wine configuration
winecfg

# Graphics tab settings:
# - Enable CSMT for better performance
# - Set video memory size
# - Enable Direct3D 11
# - Enable OpenGL

# Audio tab settings:
# - Select audio driver (ALSA recommended)
# - Configure sample rate
# - Set buffer size

# Applications tab settings:
# - Set Windows version to Windows 10
# - Apply to all applications
# - Override for specific applications
```

### Environment Variables
```bash
# Wine environment variables
export WINEARCH=win64                    # 64-bit architecture
export WINEPREFIX=~/.wine-dexarb         # Custom prefix
export WINEDEBUG=-all                    # Disable debug output
export WINEDLLOVERRIDES="mscoree,mshtml=" # DLL overrides

# Performance variables
export WINEDLLPATH=/usr/lib/wine
export WINE_LD_LIBRARY_PATH=/usr/lib/wine
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
export CMAKE_GENERATOR="MinGW Makefiles"
```

### CMake Configuration
```bash
# Configure for MinGW
cmake .. -G "MinGW Makefiles" \
    -DCMAKE_C_COMPILER=x86_64-w64-mingw32-gcc \
    -DCMAKE_CXX_COMPILER=x86_64-w64-mingw32-g++ \
    -DCMAKE_BUILD_TYPE=Release

# Build with MinGW
mingw32-make -j$(nproc)
```

### Wine Development Headers
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

## 🔧 Advanced Configuration

### Registry Configuration
```bash
# Open Wine registry editor
wine regedit

# Common registry keys:
# HKEY_CURRENT_USER\Software\Wine
# HKEY_LOCAL_MACHINE\Software\Wine
# HKEY_CURRENT_USER\Software\Microsoft\Windows\CurrentVersion

# Example: Set Windows version
wine reg add "HKEY_CURRENT_USER\Software\Wine" /v "Version" /t REG_SZ /d "win10" /f
```

### DLL Overrides
```bash
# Set DLL overrides
export WINEDLLOVERRIDES="mscoree,mshtml="

# Common overrides:
# - mscoree: .NET Framework
# - mshtml: Internet Explorer
# - d3d11: Direct3D 11
# - d3d9: Direct3D 9
```

### File System Integration
```bash
# Configure drive mappings in winecfg
# - C: drive maps to ~/.wine/drive_c
# - Z: drive maps to root filesystem
# - Custom mappings for project directories

# Create symbolic links
ln -s ~/projects/DEXArb ~/.wine/drive_c/DEXArb
ln -s ~/downloads ~/.wine/drive_c/Downloads
```

## 📊 Performance Optimization

### Wine Performance Settings
```bash
# Enable CSMT (Command Stream Multi-Threading)
winecfg
# Graphics tab -> Enable CSMT

# Set Windows version to Windows 10
winecfg
# Applications tab -> Windows 10

# Optimize memory settings
winecfg
# Memory tab -> Adjust as needed
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

### Compiler Optimizations
```bash
# GCC optimizations for cross-compilation
export CFLAGS="-O3 -march=native -mtune=native"
export CXXFLAGS="-O3 -march=native -mtune=native"

# CMake optimizations
cmake .. -G "MinGW Makefiles" \
    -DCMAKE_CXX_FLAGS="-O3 -march=native" \
    -DCMAKE_BUILD_TYPE=Release
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

# Check Wine processes
ps aux | grep wine
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

### Wine Debugging Tools
```bash
# Wine debugger
winedbg

# Wine console
wineconsole

# Wine file manager
winefile

# Wine task manager
winetask
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

### File System Security
```bash
# Wine can access host file system
# Limit access through drive mappings
# Use symbolic links for controlled access
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

### Cleanup
```bash
# Remove old Wine prefixes
rm -rf ~/.wine-*

# Clean Wine cache
rm -rf ~/.cache/wine

# Remove Wine temporary files
rm -rf /tmp/.wine-*
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

### Wine Scripts
```bash
#!/bin/bash
# wine-dexarb.sh

export WINEPREFIX=~/.wine-dexarb
export WINEARCH=win64

# Run DEXArb with Wine
wine DEXArb.exe "$@"
```

## 📚 Additional Resources

### Official Documentation
- **Wine Wiki** - wiki.winehq.org
- **Wine User Guide** - wiki.winehq.org/User_Guide
- **Wine Developer Guide** - wiki.winehq.org/Developer_Guide
- **Wine FAQ** - wiki.winehq.org/FAQ

### Community Resources
- **Wine Forums** - forum.winehq.org
- **Wine IRC** - irc.freenode.net/#wine
- **Wine Bug Reports** - bugs.winehq.org
- **Wine Patches** - wine-patches@winehq.org

### Related Tools
- **Winetricks** - Scripts for installing Windows applications
- **PlayOnLinux** - Wine application manager
- **Lutris** - Gaming platform with Wine support
- **CrossOver** - Commercial Wine-based solution

---

**Wine Reference Status**: ✅ **COMPLETE**  
**Last Updated**: March 2025  
**Next Review**: Quarterly review
