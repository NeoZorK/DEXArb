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

### Using CMake (Recommended)

```bash
git clone <repository-url>
cd DEXArb
mkdir build && cd build
cmake ..
cmake --build . --config Release
# Binary will be created in build/bin/NeoZorKDEXArb
```

## 🧪 Testing

The project includes comprehensive C++ unit tests using Google Test framework with 100% success rate:

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
```

### Test Results
- **Total Test Suites**: 3
- **Total Tests**: 18+
- **Success Rate**: 100%
- **Execution Time**: <0.01 seconds
- **Coverage**: Modern utilities, core functionality, error handling

### Using Build Script (Recommended)

```bash
chmod +x build-and-deploy.sh
./build-and-deploy.sh
```

This script will:
- Check dependencies automatically
- Pull latest changes from git
- Build project with CMake
- Copy binary to `bin/` directory
- Commit and push changes

### Using Legacy Deploy Script

```bash
chmod +x DeployBins.sh
./DeployBins.sh
```

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

1. Check the [troubleshooting section](docs/BUILD_AND_USAGE.md#troubleshooting)
2. Review the configuration file
3. Verify RPC endpoint availability
4. Check system resources and network connectivity

## 🔮 Roadmap

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
