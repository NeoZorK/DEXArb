# NeoZorKDEXArb - DEX Pool Scanner (archived prototype)

[![C++](https://img.shields.io/badge/C++-20-blue.svg)](https://isocpp.org/)
[![CMake](https://img.shields.io/badge/CMake-3.16+-green.svg)](https://cmake.org/)
[![License](https://img.shields.io/badge/License-MIT-yellow.svg)](LICENSE)
[![Status](https://img.shields.io/badge/Status-archived-lightgrey.svg)]()

> **ARCHIVED — prototype, no longer maintained (2026-09-30).**
> This repository is kept for reference only. No further development, issue handling or
> security updates are planned. It is not an arbitrage bot and must not be used to trade.

A C++23 console tool that scans EVM chains for DEX factory contracts and pools through
public JSON-RPC endpoints (`eth_getLogs`) and collects basic swap statistics.
The name "arbitrage" reflects the original plan; the arbitrage part was never implemented.

## What works / What does not

| Area | Status |
|------|--------|
| Multi-threaded factory scan (`PairCreated` logs) over public JSON-RPC, EVM chains (Ethereum, BSC, Polygon, Fantom, Avalanche) | Works (prototype quality) |
| Swap log statistics per pool (`eth_getLogs`) | Works (prototype quality) |
| JSON config file, rate limiting per RPC endpoint, CLI flags | Works |
| Arbitrage detection and execution (`src/core/arbitrage.cpp`) | Not implemented, stub |
| Profit analysis (`src/core/profit_analyzer.cpp`) | Not implemented, stub |
| Wallet (`src/utils/wallet.cpp`) | Not implemented, stub |
| Solana | Not supported (name appears only in config and help output) |
| Real-time / streaming monitoring | Not supported; scans are batch requests over a block range |

## Known issues

- The DEX catalog in `src/network/queries.cpp` (`show_all_dexes_by_blockchain`) and the known
  address lists in `src/dex/dex_scanner.cpp` contain wrong and duplicated addresses (the same
  address is listed for several different protocols; some are routers, not factories) and
  non-AMM entries (1inch, dYdX, 0x, Kyber, Bancor, lending and other protocols). Treat the
  catalog as unverified; check every address against the official deployment lists.
- Some catalog addresses are visibly placeholders (repeated hex patterns).
- Test counts, coverage figures and status reports in `docs/` and `tests/cpp/` were written
  during development and are stale; they were not re-verified at archiving time.
- The project was not built or re-tested when it was archived. The `scripts/build/` directory referenced in the build sections is not tracked in this repository; use plain CMake.
- Public RPC endpoints in the config examples may no longer exist or may rate-limit.

## 📋 Prerequisites

- **C++20** compatible compiler (GCC 10+, Clang 12+, MSVC 2022+)
- **CMake** 3.16 or higher
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

The project includes comprehensive C++ unit tests using Google Test framework plus shell script testing:

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
The figures previously listed here were not verified and have been removed.

### Build Script Testing
- **Universal Build Script**: 12 comprehensive tests
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
- **Console Output**: Color-coded console information

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
- **[Quick Start Guide](docs/getting-started/QUICK_START.md)**: Get up and running in 5 minutes
- **[Universal Build Script](docs/getting-started/UNIVERSAL_BUILD_SCRIPT.md)**: One script for all platforms
- **[Build and Usage Guide](docs/getting-started/BUILD_AND_USAGE.md)**: Comprehensive setup and usage instructions
- **[Project Description](docs/development/PROJECT_DESCRIPTION.md)**: Detailed technical overview and architecture

## 🔒 Security

- The scanner only issues read-only JSON-RPC calls.
- The wallet module is a stub; do not pass real private keys to it.
- Uses free public RPC endpoints subject to their own rate limits.

## 🤝 Contributing

The project is archived; contributions are not being accepted. Forks are welcome under the MIT license.

## 📄 License

This project is licensed under the MIT License - see the [LICENSE](LICENSE) file for details.

## 🆘 Support

For issues and questions:

1. **Build Issues**: Check [Universal Build Script Guide](docs/getting-started/UNIVERSAL_BUILD_SCRIPT.md#troubleshooting)
2. **General Issues**: Check [Build and Usage Guide](docs/getting-started/BUILD_AND_USAGE.md#troubleshooting)
3. Review the configuration file
4. Verify RPC endpoint availability
5. Check system resources and network connectivity

## 🔮 Roadmap

None. The project is archived.

---

**Version**: 1.0.7  
**Last Updated**: 2026-09-30 (archived)  
**Author**: Rostyslav S.  
**Build System**: Universal Build Script v1.0.7 ✅
