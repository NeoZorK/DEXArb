# 🍷 Windows in Ubuntu Wine Container - Complete Guide

## 🎯 Overview

This implementation provides a complete solution for running Windows applications inside Ubuntu Docker containers using Wine. It enables cross-compilation of Windows applications on Linux and provides a robust testing environment for Windows compatibility.

## 🚀 Quick Start

### 1. Prerequisites
```bash
# Install Apple Container CLI
brew install --cask docker

# Make scripts executable
chmod +x scripts/container/*.sh
```

### 2. Create and Setup
```bash
# Create container with Wine (5-10 minutes)
./scripts/container/run-windows-in-ubuntu-wine.sh --create

# Test Wine installation
./scripts/container/run-windows-in-ubuntu-wine.sh --wine-test

# Build Windows application
./scripts/container/run-windows-in-ubuntu-wine.sh --build

# Run Windows application
./scripts/container/run-windows-in-ubuntu-wine.sh --run --help
```

## 📁 File Structure

```
scripts/container/
├── run-windows-in-ubuntu-wine.sh    # Main script (400+ lines)
├── README_WINDOWS_WINE.md           # Quick reference
├── README_WINDOWS_WINE_FINAL.md     # This comprehensive guide
├── test-windows-wine.sh             # Test script
├── example-usage.sh                 # Usage examples
└── QUICK_START_WINDOWS_WINE.md     # 5-minute quick start

docs/
├── getting-started/
│   └── WINDOWS_IN_UBUNTU_WINE.md   # Full documentation
└── reports/
    └── WINDOWS_WINE_IMPLEMENTATION_REPORT.md  # Implementation report
```

## 🔧 Core Features

### Container Management
- ✅ **Create**: Ubuntu 24.04 + Wine + MinGW setup
- ✅ **Build**: Cross-compilation for Windows targets
- ✅ **Run**: Execute Windows applications via Wine
- ✅ **Shell**: Interactive development environment
- ✅ **Status**: Container lifecycle monitoring
- ✅ **Clean**: Build artifact management

### Wine Integration
- ✅ **64-bit & 32-bit**: Full Wine support
- ✅ **Auto-configuration**: DLL overrides and compatibility
- ✅ **Font support**: Microsoft and Wine fonts
- ✅ **Virtual display**: Xvfb for GUI applications
- ✅ **Testing tools**: Wine installation validation

### Development Tools
- ✅ **MinGW-w64**: Windows cross-compiler toolchain
- ✅ **CMake integration**: Windows toolchain configuration
- ✅ **Build system**: Automated compilation pipeline
- ✅ **Error handling**: Comprehensive logging and debugging

## 📚 Documentation

### Quick References
- **[Quick Start](QUICK_START_WINDOWS_WINE.md)**: 5-minute setup guide
- **[Examples](example-usage.sh)**: Common workflow examples
- **[Testing](test-windows-wine.sh)**: Validation and testing
- **[Main Script](run-windows-in-ubuntu-wine.sh)**: Full functionality

### Comprehensive Guides
- **[Full Documentation](../../docs/getting-started/WINDOWS_IN_UBUNTU_WINE.md)**: Complete usage guide
- **[Implementation Report](../../docs/reports/WINDOWS_WINE_IMPLEMENTATION_REPORT.md)**: Technical details

## 🎯 Use Cases

### Primary Scenarios
1. **Cross-Platform Development**: Build Windows apps on Linux
2. **Wine Compatibility Testing**: Validate Windows app compatibility
3. **CI/CD Integration**: Windows builds in Linux environments
4. **Development Environment**: Consistent Windows development setup
5. **Testing & Validation**: Cross-platform compatibility testing

### Target Users
- **Developers**: Building cross-platform applications
- **DevOps Engineers**: CI/CD pipeline setup
- **Testers**: Cross-platform compatibility validation
- **System Administrators**: Container-based development environments

## 🔍 Command Reference

### Main Commands
| Command | Description | Use Case |
|---------|-------------|----------|
| `--create` | Create and setup container | Initial setup |
| `--build` | Build Windows application | Compilation |
| `--run` | Run Windows application | Execution |
| `--shell` | Open interactive shell | Development |
| `--wine-test` | Test Wine installation | Validation |
| `--wine-config` | Configure Wine settings | Customization |
| `--rebuild` | Force rebuild everything | Reset |
| `--status` | Show container status | Monitoring |
| `--clean` | Clean build artifacts | Maintenance |

### Workflow Examples

#### Complete Development Cycle
```bash
# Setup (one-time)
./run-windows-in-ubuntu-wine.sh --create
./run-windows-in-ubuntu-wine.sh --wine-test

# Development loop
./run-windows-in-ubuntu-wine.sh --build
./run-windows-in-ubuntu-wine.sh --run --help
./run-windows-in-ubuntu-wine.sh --clean
./run-windows-in-ubuntu-wine.sh --build
```

#### Interactive Development
```bash
# Open shell for development
./run-windows-in-ubuntu-wine.sh --shell

# Inside container
cd /workspace
mkdir -p build-windows-wine
cd build-windows-wine
cmake .. -DCMAKE_SYSTEM_NAME=Windows -DCMAKE_C_COMPILER=x86_64-w64-mingw32-gcc
make
wine NeoZorKDEXArb.exe --help
```

#### Troubleshooting
```bash
# Check status
./run-windows-in-ubuntu-wine.sh --status

# Test Wine
./run-windows-in-ubuntu-wine.sh --wine-test

# Rebuild if needed
./run-windows-in-ubuntu-wine.sh --rebuild
```

## 🚧 Technical Details

### Container Architecture
- **Base**: Ubuntu 24.04 (Noble)
- **Wine**: 64-bit and 32-bit support
- **Compiler**: MinGW-w64 cross-compiler
- **Build**: CMake with Windows toolchain
- **Display**: Xvfb virtual framebuffer

### Wine Configuration
```bash
export WINEPREFIX=/root/.wine
export WINEARCH=win64
wineboot --init

# DLL overrides for compatibility
wine reg add 'HKCU\\Software\\Wine\\DllOverrides' /v 'msvcrt' /t REG_SZ /d 'native,builtin' /f
wine reg add 'HKCU\\Software\\Wine\\DllOverrides' /v 'msvcp140' /t REG_SZ /d 'native,builtin' /f
```

### CMake Windows Toolchain
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

## 🚨 Limitations & Considerations

### Technical Limitations
- **Performance**: Wine adds overhead to Windows applications
- **Compatibility**: Not all Windows applications work perfectly with Wine
- **Resource Usage**: Container requires more resources than native
- **Startup Time**: Wine initialization takes time

### Security Considerations
- **Root Privileges**: Container runs as root inside
- **Network Access**: No network isolation by default
- **File Sharing**: Build artifacts shared with host system
- **Wine Prefix**: Persistent Wine configuration

## 🔮 Future Enhancements

### Potential Improvements
1. **Multi-Architecture**: ARM64 Windows targets
2. **Wine Versions**: Multiple Wine version management
3. **GUI Applications**: Better display handling
4. **Performance**: Faster Wine startup
5. **Security**: Network isolation, user permissions

### Integration Opportunities
1. **GitHub Actions**: CI/CD pipeline integration
2. **Docker Compose**: Multi-container orchestration
3. **Volume Management**: Persistent data management
4. **Health Checks**: Container health monitoring

## 📊 Implementation Metrics

### Code Quality
- **Main Script**: ~400 lines with comprehensive error handling
- **Documentation**: ~200 lines covering all functionality
- **Test Scripts**: ~100 lines for validation
- **Total Files**: 6 new files created
- **Documentation**: 3 comprehensive documentation files

### Quality Assurance
- ✅ **Error Handling**: Comprehensive error checking and logging
- ✅ **Logging**: Colored output with timestamps
- ✅ **Documentation**: 100% documented functionality
- ✅ **Testing**: Script validation completed
- ✅ **Integration**: Updated existing project documentation

## 🎉 Benefits

### For Developers
1. **Cross-Platform Testing**: Test Windows apps on Linux
2. **CI/CD Integration**: Windows builds in Linux environments
3. **Development Flexibility**: Linux tools with Windows targets
4. **Wine Testing**: Validate Wine compatibility
5. **Isolated Environment**: Clean container-based development

### For Project
1. **Platform Coverage**: Windows application support
2. **Testing Capabilities**: Cross-platform validation
3. **Deployment Options**: Windows binary generation
4. **Documentation**: Comprehensive usage guides
5. **Maintenance**: Easy container management

## 🆘 Support & Troubleshooting

### Getting Help
```bash
# Show all options
./run-windows-in-ubuntu-wine.sh --help

# Check container status
./run-windows-in-ubuntu-wine.sh --status

# Test Wine installation
./run-windows-in-ubuntu-wine.sh --wine-test

# Open shell for debugging
./run-windows-in-ubuntu-wine.sh --shell
```

### Common Issues
1. **Container not found**: Run `--create` first
2. **Wine not working**: Use `--wine-test` to diagnose
3. **Build failures**: Check dependencies with `--shell`
4. **Display issues**: Virtual framebuffer is automatically configured

### Debug Commands
```bash
# Check container status
./run-windows-in-ubuntu-wine.sh --status

# Open shell for debugging
./run-windows-in-ubuntu-wine.sh --shell

# Test Wine installation
./run-windows-in-ubuntu-wine.sh --wine-test

# Rebuild everything
./run-windows-in-ubuntu-wine.sh --rebuild
```

## 📞 Conclusion

The Windows Wine container implementation provides a robust, well-documented solution for running Windows applications in Ubuntu Docker containers. It significantly enhances the project's cross-platform capabilities and provides developers with a powerful tool for Windows application development and testing in Linux environments.

### Key Success Factors
- **Complete functionality** for container lifecycle management
- **Comprehensive documentation** for users and developers
- **Integration** with existing project structure
- **Quality assurance** through testing and validation
- **Future-ready** architecture for enhancements

### Next Steps
1. **Try it out**: Follow the quick start guide
2. **Explore features**: Use `--help` to see all options
3. **Read documentation**: Review the full documentation
4. **Provide feedback**: Report issues or suggest improvements

---

**Created by**: Rostyslav S.  
**Date**: 26.02.2025  
**Version**: 1.0.0  
**Status**: Complete and tested
