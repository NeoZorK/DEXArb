# Build and Usage Guide

## Prerequisites

### System Requirements
- **Operating System**: Windows, Linux, or macOS
- **Compiler**: GCC 7+, Clang 6+, or MSVC 2019+
- **CMake**: Version 3.28 or higher
- **libcurl**: Development libraries

### Installing Dependencies

#### Linux (Ubuntu/Debian)
```bash
sudo apt update
sudo apt install build-essential cmake libcurl4-openssl-dev
```

#### Linux (CentOS/RHEL/Fedora)
```bash
# CentOS/RHEL
sudo yum install gcc-c++ cmake libcurl-devel

# Fedora
sudo dnf install gcc-c++ cmake libcurl-devel
```

#### macOS
```bash
# Install Homebrew if not already installed
/bin/bash -c "$(curl -fsSL https://raw.githubusercontent.com/Homebrew/install/HEAD/install.sh)"

# Install dependencies
brew install cmake curl
```

#### Windows
```bash
# Using vcpkg (recommended)
git clone https://github.com/Microsoft/vcpkg.git
cd vcpkg
./bootstrap-vcpkg.bat
./vcpkg install curl:x64-windows

# Or using Chocolatey
choco install cmake curl
```

## Building the Project

### Method 1: Using CMake (Recommended)

1. **Clone and navigate to the project**:
   ```bash
   cd /path/to/DEXArb
   ```

2. **Create build directory**:
   ```bash
   mkdir build
   cd build
   ```

3. **Configure with CMake**:
   ```bash
   cmake ..
   ```

4. **Build the project**:
   ```bash
   cmake --build . --config Release
   ```

5. **Install (optional)**:
   ```bash
   cmake --install .
   ```

### Method 2: Using the Deploy Script

The project includes a deployment script for quick builds:

```bash
chmod +x DeployBins.sh
./DeployBins.sh
```

This script will:
- Pull latest changes from git
- Compile static and dynamic binaries
- Move binaries to the `bin/` directory
- Commit and push changes

### Method 3: Manual Compilation

For advanced users who want to compile manually:

```bash
cd src
g++ -std=c++17 -O2 -Wall -Wextra \
    -I../include \
    main.cpp blockchain.cpp config_manager.cpp dex_scanner.cpp \
    dex_pools.cpp dex_tokens.cpp dex_stats.cpp rpc_core.cpp \
    queries.cpp input.cpp measure.cpp arbitrage.cpp wallet.cpp \
    profit_analyzer.cpp \
    -lcurl -lpthread \
    -o ../bin/NeoZorKDEXArb
```

## Running the Application

### Basic Usage

The application is a command-line tool with the following syntax:

```bash
./NeoZorKDEXArb [flag] <blockchain> [parameters]
```

### Available Commands

#### 1. Scanning Blockchain
```bash
# Scan last 1000 blocks of Ethereum
./NeoZorKDEXArb -scan ethereum 1000

# Scan last 5000 blocks of BSC
./NeoZorKDEXArb -scan bsc 5000

# Scan last 10000 blocks of Polygon
./NeoZorKDEXArb -scan polygon 10000
```

#### 2. Viewing Configuration
```bash
# Show scan configuration
./NeoZorKDEXArb -showSCAN-CONFIG ethereum

# Show scan results
./NeoZorKDEXArb -showSCAN ethereum

# Show scan statistics
./NeoZorKDEXArb -showSCAN-STAT ethereum
```

#### 3. DEX Information
```bash
# Show all DEXes for a blockchain
./NeoZorKDEXArb -showDEXES ethereum

# Show pools for a specific DEX
./NeoZorKDEXArb -showPOOLS ethereum 0x5C69bEe701ef814a2B6a3EDD4B1652CB9cc5aA6f

# Show tokens for a specific DEX
./NeoZorKDEXArb -showTOKENS ethereum 0x5C69bEe701ef814a2B6a3EDD4B1652CB9cc5aA6f
```

#### 4. Token Search
```bash
# Find a specific token in a DEX
./NeoZorKDEXArb -findTOKEN ethereum 0x5C69bEe701ef814a2B6a3EDD4B1652CB9cc5aA6f 0xA0b86a33E6441b8C4C8C8C8C8C8C8C8C8C8C8C8C8

# Find a token across all DEXes
./NeoZorKDEXArb -findTOKENS ethereum 0xA0b86a33E6441b8C4C8C8C8C8C8C8C8C8C8C8C8C8
```

### Supported Blockchains

- `ethereum` - Ethereum mainnet
- `bsc` - Binance Smart Chain
- `polygon` - Polygon (Matic)
- `fantom` - Fantom Opera
- `avalanche` - Avalanche C-Chain
- `solana` - Solana (limited support)

### Block Range Limits

- **Minimum**: 1,000 blocks
- **Maximum**: 1,000,000 blocks
- **Recommended**: 5,000-50,000 blocks for optimal performance

## Configuration

### Automatic Configuration

On first run, the application will automatically create a `neozork-config` file with default settings:

```json
{
  "threads": 3,
  "ethereum": {
    "rpc_endpoints": [
      {"url": "https://rpc.ankr.com/eth", "request_limit": 20},
      {"url": "https://eth.llamarpc.com", "request_limit": 25}
    ]
  },
  "bsc": {
    "rpc_endpoints": [
      {"url": "https://bsc-dataseed.binance.org", "request_limit": 50},
      {"url": "https://rpc.ankr.com/bsc", "request_limit": 30}
    ]
  }
}
```

### Manual Configuration

You can edit the `neozork-config` file to customize:

- **Thread count**: Number of parallel threads for scanning
- **RPC endpoints**: Custom RPC nodes with rate limits
- **DEX data**: Previously discovered DEX information

## Output Files

### 1. Configuration File (`neozork-config`)
Contains:
- RPC endpoint settings
- Discovered DEX information
- Pool and token data
- Performance settings

### 2. Statistics File (`neozork-scan-stat`)
Contains:
- Execution time measurements
- Network usage statistics
- Memory and CPU usage
- Disk I/O metrics

### 3. Console Output
Color-coded information:
- **Green**: Success messages and completed operations
- **Yellow**: Data and information messages
- **Red**: Errors and warnings
- **Blue**: Command information
- **Cyan**: Progress indicators

## Performance Optimization

### 1. Thread Count
Adjust the thread count in configuration based on your system:
- **4-core CPU**: 3-4 threads
- **8-core CPU**: 6-8 threads
- **16+ core CPU**: 8-12 threads

### 2. Block Range
- **Small scans** (1,000-10,000 blocks): Quick for testing
- **Medium scans** (10,000-100,000 blocks): Good for discovery
- **Large scans** (100,000+ blocks): Comprehensive but slow

### 3. Network Optimization
- Use local RPC nodes if available
- Consider using multiple RPC endpoints for redundancy
- Monitor rate limits to avoid being blocked

## Troubleshooting

### Common Issues

#### 1. Build Errors
```bash
# Error: CURL not found
# Solution: Install libcurl development package
sudo apt install libcurl4-openssl-dev  # Ubuntu/Debian
brew install curl                      # macOS
```

#### 2. Runtime Errors
```bash
# Error: Cannot connect to RPC endpoint
# Solution: Check internet connection and RPC endpoint availability
# Try alternative RPC endpoints in configuration
```

#### 3. Performance Issues
```bash
# Slow scanning
# Solution: Reduce thread count or block range
# Check system resources (CPU, memory, network)
```

#### 4. Configuration Issues
```bash
# Missing configuration file
# Solution: Delete neozork-config and restart application
# It will create a new default configuration
```

### Debug Mode

For debugging, you can compile with debug information:

```bash
mkdir build-debug
cd build-debug
cmake -DCMAKE_BUILD_TYPE=Debug ..
cmake --build .
```

## Examples

### Complete Workflow Example

1. **Initial Setup**:
   ```bash
   ./NeoZorKDEXArb -showSCAN-CONFIG ethereum
   ```

2. **Scan for DEXes**:
   ```bash
   ./NeoZorKDEXArb -scan ethereum 10000
   ```

3. **View Discovered DEXes**:
   ```bash
   ./NeoZorKDEXArb -showDEXES ethereum
   ```

4. **Analyze Specific DEX**:
   ```bash
   ./NeoZorKDEXArb -showPOOLS ethereum 0x5C69bEe701ef814a2B6a3EDD4B1652CB9cc5aA6f
   ```

5. **Check Statistics**:
   ```bash
   ./NeoZorKDEXArb -showSCAN-STAT ethereum
   ```

### Multi-Chain Analysis

```bash
# Scan multiple blockchains
./NeoZorKDEXArb -scan ethereum 5000
./NeoZorKDEXArb -scan bsc 5000
./NeoZorKDEXArb -scan polygon 5000

# Compare DEXes across chains
./NeoZorKDEXArb -showDEXES ethereum
./NeoZorKDEXArb -showDEXES bsc
./NeoZorKDEXArb -showDEXES polygon
```

## Security Considerations

1. **RPC Endpoints**: Only free, public endpoints are used
2. **No Private Keys**: The application doesn't handle private keys
3. **Read-Only Operations**: All operations are read-only
4. **Rate Limiting**: Built-in rate limiting to respect RPC limits

## Support

For issues and questions:
1. Check the troubleshooting section
2. Review the configuration file
3. Verify RPC endpoint availability
4. Check system resources and network connectivity
