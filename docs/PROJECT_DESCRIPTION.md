# NeoZorKDEXArb - DEX Arbitrage Scanner

## Project Overview

**NeoZorKDEXArb** is a high-performance C++ console application designed for scanning, analyzing, and identifying arbitrage opportunities across decentralized exchanges (DEX) on multiple blockchains. The project focuses on cross-platform compatibility, minimal dependencies, and efficient blockchain data processing.

## Supported Blockchains

- **Ethereum** - Mainnet with Uniswap V2/V3 support
- **Binance Smart Chain (BSC)** - PancakeSwap and other DEXes
- **Polygon** - QuickSwap and Polygon-based DEXes
- **Fantom** - SpookySwap and Fantom DEXes
- **Avalanche** - TraderJoe and Avalanche DEXes
- **Solana** - Limited support (configuration only)

## Key Features

### 1. Blockchain Scanning
- **Block Range Scanning**: Scans specified block ranges (1,000 to 1,000,000 blocks)
- **Factory Contract Detection**: Identifies DEX factory contracts by analyzing transaction logs
- **Event Signature Matching**: Detects pool creation events (PairCreated, PoolCreated)
- **Multithreaded Processing**: Distributes scanning across multiple threads for performance

### 2. DEX Analysis
- **Pool Discovery**: Automatically discovers all pools for detected DEXes
- **Token Information**: Extracts token addresses and metadata from pools
- **Liquidity Analysis**: Calculates total liquidity and TVL (Total Value Locked)
- **Trading Statistics**: Collects 24-hour trading volume and transaction counts

### 3. Configuration Management
- **JSON Configuration**: Uses `neozork-config` file for settings
- **Free RPC Endpoints**: Only uses free, public RPC nodes
- **Automatic Setup**: Creates default configuration if none exists
- **Cross-chain Support**: Separate configuration sections for each blockchain

### 4. Data Output
- **Console Display**: Color-coded output (green=success, yellow=data, red=errors)
- **Statistics Logging**: Performance metrics saved to `neozork-scan-stat`
- **Progress Tracking**: Real-time progress bars during scanning
- **Structured Data**: JSON-formatted configuration updates

## Technical Architecture

### Core Components

1. **Main Controller** (`main.cpp`)
   - Command-line argument parsing
   - Program flow orchestration
   - Help and usage display

2. **Blockchain Interface** (`blockchain.cpp`)
   - Blockchain type enumeration
   - RPC endpoint management
   - Cross-chain compatibility

3. **Configuration Manager** (`config_manager.cpp`)
   - JSON configuration parsing
   - DEX data persistence
   - Settings validation

4. **DEX Scanner** (`dex_scanner.cpp`)
   - Factory contract detection
   - Event log analysis
   - Contract interaction

5. **Pool Management** (`dex_pools.cpp`)
   - Pool address retrieval
   - Liquidity calculation
   - Token pair extraction

6. **Statistics Collection** (`dex_stats.cpp`)
   - Trading volume analysis
   - Transaction counting
   - Performance metrics

7. **RPC Core** (`rpc_core.cpp`)
   - HTTP request handling
   - JSON-RPC communication
   - Rate limiting

### Dependencies

- **libcurl**: HTTP client for RPC communication
- **C++17 Standard Library**: Modern C++ features and threading
- **CMake 3.28+**: Cross-platform build system

### Performance Features

- **Multithreading**: Parallel block scanning and data collection
- **Memory Management**: Efficient data structures and memory usage
- **Network Optimization**: Rate limiting and connection pooling
- **Disk I/O**: Minimal file operations with buffered I/O

## Free RPC Endpoints

The application exclusively uses free, public RPC endpoints:

| Blockchain | Primary Endpoint | Secondary Endpoint | Rate Limit |
|------------|------------------|-------------------|------------|
| Ethereum | rpc.ankr.com/eth | eth.llamarpc.com | 20-25 req/s |
| BSC | bsc-dataseed.binance.org | rpc.ankr.com/bsc | 30-50 req/s |
| Polygon | polygon-rpc.com | rpc.ankr.com/polygon | 30-50 req/s |
| Fantom | rpc.ftm.tools | rpc.ankr.com/fantom | 25-30 req/s |
| Avalanche | rpc.ankr.com/avalanche | api.avax.network/ext/bc/C/rpc | 20-25 req/s |
| Solana | api.mainnet-beta.solana.com | solana-mainnet.rpc.extrnode.com | 50-100 req/s |

## Project Structure

```
DEXArb/
├── bin/                    # Compiled binaries
│   ├── dexarb             # Static binary
│   └── dexarb2            # Dynamic binary
├── include/               # Header files
│   ├── main.h            # Core structures and constants
│   ├── blockchain.h      # Blockchain type definitions
│   ├── config_manager.h  # Configuration management
│   ├── dex_scanner.h     # DEX scanning functionality
│   ├── dex_pools.h       # Pool management
│   ├── dex_tokens.h      # Token handling
│   ├── dex_stats.h       # Statistics collection
│   ├── rpc_core.h        # RPC communication
│   ├── queries.h         # Query functions
│   ├── input.h           # Input/output utilities
│   ├── measure.h         # Performance measurement
│   ├── arbitrage.h       # Arbitrage analysis (stub)
│   ├── wallet.h          # Wallet management (stub)
│   └── profit_analyzer.h # Profit analysis (stub)
├── src/                  # Source files
│   ├── main.cpp          # Main program entry
│   ├── blockchain.cpp    # Blockchain interface
│   ├── config_manager.cpp # Configuration handling
│   ├── dex_scanner.cpp   # DEX scanning logic
│   ├── dex_pools.cpp     # Pool operations
│   ├── dex_tokens.cpp    # Token operations
│   ├── dex_stats.cpp     # Statistics collection
│   ├── rpc_core.cpp      # RPC communication
│   ├── queries.cpp       # Query implementation
│   ├── input.cpp         # I/O utilities
│   ├── measure.cpp       # Performance measurement
│   ├── arbitrage.cpp     # Arbitrage logic (stub)
│   ├── wallet.cpp        # Wallet operations (stub)
│   └── profit_analyzer.cpp # Profit analysis (stub)
├── docs/                 # Documentation
├── CMakeLists.txt        # CMake build configuration
├── CMakePresets.json     # CMake presets
├── DeployBins.sh         # Build and deployment script
└── read.md              # Project overview
```

## Current Limitations

1. **Solana Support**: Limited to configuration display due to API differences
2. **Arbitrage Execution**: Core arbitrage logic is implemented as stubs
3. **Wallet Integration**: Wallet management functions are placeholders
4. **Profit Analysis**: Advanced profit calculation features are not fully implemented

## Future Development

- Complete arbitrage execution functionality
- Enhanced Solana support
- Wallet integration for automated trading
- Advanced profit analysis and risk management
- Web interface for data visualization
- Real-time monitoring and alerts
