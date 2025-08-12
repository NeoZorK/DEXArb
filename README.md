# NeoZorKDEXArb - DEX Arbitrage Scanner

[![C++](https://img.shields.io/badge/C++-23-blue.svg)](https://isocpp.org/)
[![CMake](https://img.shields.io/badge/CMake-3.28+-green.svg)](https://cmake.org/)
[![License](https://img.shields.io/badge/License-MIT-yellow.svg)](LICENSE)
[![Platform](https://img.shields.io/badge/Platform-Windows%20%7C%20Linux%20%7C%20macOS-lightgrey.svg)]()
[![Tests](https://img.shields.io/badge/Tests-100%25%20passed-brightgreen.svg)]()
[![Version](https://img.shields.io/badge/Version-1.0.7-blue.svg)]()

A high-performance C++ console application for scanning, analyzing, and identifying arbitrage opportunities across decentralized exchanges (DEX) on multiple blockchains.

## 🚀 Features

- **Multi-Blockchain Support**: Ethereum, BSC, Polygon, Fantom, Avalanche, Solana
- **DEX Discovery**: Automatic detection of factory contracts and pools
- **Real-time Scanning**: Multithreaded blockchain scanning with configurable ranges
- **Free RPC Integration**: Uses only free, public RPC endpoints
- **Cross-platform**: Windows, Linux, and macOS support
- **Performance Monitoring**: Built-in statistics and performance metrics
- **Configuration Management**: JSON-based configuration with automatic setup
- **Comprehensive Testing**: 100% test coverage with 3 test suites (18+ tests)

## 📋 Prerequisites

- **C++23** compatible compiler (GCC 13+, Clang 17+, MSVC 2022+)
- **CMake** 3.28 or higher
- **libcurl** development libraries
- **Google Test** framework for C++ testing

### Quick Install Dependencies

**Ubuntu/Debian:**
```bash
sudo apt install build-essential cmake libcurl4-openssl-dev
```

**macOS:**
```bash
brew install cmake curl
```

**Windows:**
```bash
# Using vcpkg
git clone https://github.com/Microsoft/vcpkg.git
cd vcpkg && ./bootstrap-vcpkg.bat
./vcpkg install curl:x64-windows
```

## 🛠️ Building

### Using Universal Build Script (Recommended)

The project includes a comprehensive universal build script that supports all platforms:

```bash
# Make script executable
chmod +x scripts/build/build-universal.sh

# Interactive build (recommended for first-time users)
./scripts/build/build-universal.sh

# Quick build for specific platform
./scripts/build/build-universal.sh --platform macos --build-type Release

# Full build with tests and package
./scripts/build/build-universal.sh --clean --test --package --verbose

# Show help
./scripts/build/build-universal.sh --help
```

**Features:**
- **Multi-platform**: macOS, Linux, Alpine, Windows
- **Multiple methods**: Native, Container, Cross-platform, Wine
- **Interactive mode**: User-friendly menu-driven interface
- **Automation ready**: Command-line options for CI/CD

**📖 [Complete Build Guide](docs/getting-started/UNIVERSAL_BUILD_SCRIPT.md)**

### Using Container Runners (Alternative)

The project also includes container runners for different environments:

```bash
# Make scripts executable
chmod +x scripts/containers/*.sh

# Run in Alpine Linux (lightweight)
./scripts/containers/run-alpine-simple.sh

# Run in Ubuntu Linux (full compatibility)
./scripts/containers/run-ubuntu-container.sh

# Run Windows apps in Ubuntu via Wine
./scripts/containers/run-windows-in-ubuntu-wine.sh --create

# Show help for any runner
./scripts/containers/run-ubuntu-container.sh --help
```

### Using CMake (Traditional)

```bash
git clone <repository-url>
cd DEXArb
mkdir build && cd build
cmake ..
cmake --build . --config Release
# Binary will be created in build/bin/NeoZorKDEXArb
```

## 🧪 Testing

The project includes comprehensive C++ unit tests using Google Test framework with 100% success rate, plus shell script testing:

```bash
# Build and run all tests
mkdir cmake-build-debug && cd cmake-build-debug
cmake -G "Unix Makefiles" ..
make -j$(sysctl -n hw.ncpu)  # macOS
# or
make -j$(nproc)              # Linux

# Run all tests
ctest --output-on-failure

# Run individual test suites
./NeoZorKDEXArbTests        # Basic functionality tests
./ModernResultTests          # Modern Result<T,E> class tests  
./ModernFormatTests          # Formatting utilities tests
./test_universal_build_script # Universal build script tests

# Test build scripts
cd scripts/build
./tests/test-universal-script.sh  # Test universal build script
```

### Test Results
- **Total Test Suites**: 4
- **Total Tests**: 19+
- **Success Rate**: 100%
- **Execution Time**: <0.01 seconds
- **Coverage**: Modern utilities, core functionality, error handling, build scripts

### Build Script Testing
- **Universal Build Script**: 12 comprehensive tests
- **Test Coverage**: 100% of script functionality
- **Execution Time**: <5 seconds
- **Dependencies**: Bash shell only
- **Platform**: Cross-platform compatible

### Using Build Scripts (Recommended)

The project includes organized build scripts in the `scripts/` directory:

```bash
# Make scripts executable
chmod +x scripts/**/*.sh

# Universal build script (recommended for all users)
./scripts/build/build-universal.sh

# Modern build with vcpkg
./scripts/build/build-modern.sh

# Multi-platform build
./scripts/build/build-multi-platform.sh

# Apple Silicon container build
./scripts/build/build-apple-container.sh

# Basic CMake build
./scripts/build/cmake.sh
```

### Using Container Scripts

For containerized development:

```bash
# Run in lightweight Alpine container (recommended)
./scripts/containers/run-alpine-simple.sh -- --help

# Quick testing
./scripts/testing/quick-test.sh

# Deploy binaries
./scripts/deployment/DeployBins.sh
```

**Scripts Directory Structure:**
- **`scripts/build/`** - Build and compilation scripts
- **`scripts/containers/`** - Container management scripts
- **`scripts/docker/`** - Docker files and configurations
- **`scripts/testing/`** - Testing and validation scripts
- **`scripts/deployment/`** - Deployment automation
- **`scripts/utilities/`** - Utility and helper scripts

See [scripts/README.md](scripts/README.md) for detailed usage information.

## 🎯 Usage

### Basic Commands

```bash
# Scan Ethereum for DEXes (last 10,000 blocks)
./NeoZorKDEXArb -scan ethereum 10000

# Show discovered DEXes
./NeoZorKDEXArb -showDEXES ethereum

# Show pools for a specific DEX
./NeoZorKDEXArb -showPOOLS ethereum 0x5C69bEe701ef814a2B6a3EDD4B1652CB9cc5aA6f

# Find a specific token
./NeoZorKDEXArb -findTOKEN ethereum 0x5C69bEe701ef814a2B6a3EDD4B1652CB9cc5aA6f 0xA0b86a33E6441b8C4C8C8C8C8C8C8C8C8C8C8C8C8
```

### Supported Blockchains

| Blockchain | Command | Example |
|------------|---------|---------|
| Ethereum | `ethereum` | `./NeoZorKDEXArb -scan ethereum 5000` |
| Binance Smart Chain | `bsc` | `./NeoZorKDEXArb -scan bsc 5000` |
| Polygon | `polygon` | `./NeoZorKDEXArb -scan polygon 5000` |
| Fantom | `fantom` | `./NeoZorKDEXArb -scan fantom 5000` |
| Avalanche | `avalanche` | `./NeoZorKDEXArb -scan avalanche 5000` |
| Solana | `solana` | `./NeoZorKDEXArb -showSCAN-CONFIG solana` |

### Available Flags

| Flag | Description | Example |
|------|-------------|---------|
| `-scan` | Scan blockchain for DEXes | `-scan ethereum 10000` |
| `-showDEXES` | Show all discovered DEXes | `-showDEXES ethereum` |
| `-showPOOLS` | Show pools for a DEX | `-showPOOLS ethereum 0x...` |
| `-showTOKENS` | Show tokens for a DEX | `-showTOKENS ethereum 0x...` |
| `-findTOKEN` | Find token in a DEX | `-findTOKEN ethereum 0x... 0x...` |
| `-findTOKENS` | Find token across all DEXes | `-findTOKENS ethereum 0x...` |
| `-showSCAN-CONFIG` | Show scan configuration | `-showSCAN-CONFIG ethereum` |
| `-showSCAN` | Show scan results | `-showSCAN ethereum` |
| `-showSCAN-STAT` | Show scan statistics | `-showSCAN-STAT ethereum` |

## ⚙️ Configuration

The application automatically creates a `neozork-config` file on first run:

```json
{
  "threads": 3,
  "ethereum": {
    "rpc_endpoints": [
      {"url": "https://rpc.ankr.com/eth", "request_limit": 20},
      {"url": "https://eth.llamarpc.com", "request_limit": 25}
    ],
    "dex": []
  }
}
```

### Configuration Options

- **threads**: Number of parallel threads for scanning
- **rpc_endpoints**: Array of RPC endpoints with rate limits
- **dex**: Automatically populated with discovered DEX information

## 📊 Output Files

- **`neozork-config`**: Configuration and discovered DEX data
- **`neozork-scan-stat`**: Performance statistics and metrics
- **Console Output**: Color-coded real-time information

## 🔧 Performance Optimization

### Thread Count Recommendations

| CPU Cores | Recommended Threads |
|-----------|---------------------|
| 4-core | 3-4 threads |
| 8-core | 6-8 threads |
| 16+ core | 8-12 threads |

### Block Range Guidelines

- **Small scans** (1,000-10,000): Quick testing
- **Medium scans** (10,000-100,000): Good discovery
- **Large scans** (100,000+): Comprehensive analysis

## 🏗️ Project Structure

```
DEXArb/
├── bin/                    # Compiled binaries
├── include/               # Header files
├── src/                   # Source files
├── docs/                  # Documentation
├── CMakeLists.txt         # Build configuration
└── README.md             # This file
```

## 📚 Documentation

- **[Documentation Index](docs/README.md)**: Complete documentation overview
- **[Quick Start Guide](docs/QUICK_START.md)**: Get up and running in 5 minutes
- **[Universal Build Script](docs/getting-started/UNIVERSAL_BUILD_SCRIPT.md)**: One script for all platforms
- **[Build and Usage Guide](docs/BUILD_AND_USAGE.md)**: Comprehensive setup and usage instructions
- **[Project Description](docs/PROJECT_DESCRIPTION.md)**: Detailed technical overview and architecture

## 🚧 Current Limitations

- **Solana Support**: Limited to configuration display
- **Arbitrage Execution**: Core logic implemented as stubs
- **Wallet Integration**: Placeholder functions
- **Profit Analysis**: Basic implementation

## 🔒 Security

- **Read-only Operations**: All blockchain operations are read-only
- **No Private Keys**: Application doesn't handle private keys
- **Free RPC Only**: Uses only free, public RPC endpoints
- **Rate Limiting**: Built-in protection against RPC rate limits

## 🤝 Contributing

1. Fork the repository
2. Create a feature branch (`git checkout -b feature/amazing-feature`)
3. Commit your changes (`git commit -m 'Add amazing feature'`)
4. Push to the branch (`git push origin feature/amazing-feature`)
5. Open a Pull Request

## 📄 License

This project is licensed under the MIT License - see the [LICENSE](LICENSE) file for details.

## 🆘 Support

For issues and questions:

1. **Build Issues**: Check [Universal Build Script Guide](docs/getting-started/UNIVERSAL_BUILD_SCRIPT.md#troubleshooting)
2. **General Issues**: Check [Build and Usage Guide](docs/BUILD_AND_USAGE.md#troubleshooting)
3. Review the configuration file
4. Verify RPC endpoint availability
5. Check system resources and network connectivity

## 🔮 Roadmap

- [x] **Universal Build Script** - One script for all platforms ✅
- [ ] Complete arbitrage execution functionality
- [ ] Enhanced Solana support
- [ ] Wallet integration for automated trading
- [ ] Advanced profit analysis and risk management
- [ ] Web interface for data visualization
- [ ] Real-time monitoring and alerts

---

**Version**: 1.0.7  
**Last Updated**: March 2025  
**Author**: Rostyslav S.  
**Build System**: Universal Build Script v1.0.7 ✅
