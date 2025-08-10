# Quick Start Guide

## What is NeoZorKDEXArb?

**NeoZorKDEXArb** is a powerful C++ tool that scans multiple blockchains to discover decentralized exchanges (DEX) and analyze their pools, tokens, and trading data. It's designed for arbitrage research and DEX analysis.

## 🚀 Get Started in 5 Minutes

### 1. Install Dependencies

**macOS:**
```bash
brew install cmake curl
```

**Ubuntu/Debian:**
```bash
sudo apt install build-essential cmake libcurl4-openssl-dev
```

### 2. Build the Project

```bash
git clone <your-repo-url>
cd DEXArb
mkdir build && cd build
cmake ..
cmake --build . --config Release
```

### 3. Run Your First Scan

```bash
# Check application version
./NeoZorKDEXArb -v
# or
./NeoZorKDEXArb -version

# Get help and see all available commands
./NeoZorKDEXArb -h
# or
./NeoZorKDEXArb -help

# Scan Ethereum for DEXes (last 5,000 blocks)
./NeoZorKDEXArb -scan ethereum 5000

# Show discovered DEXes
./NeoZorKDEXArb -showDEXES ethereum
```

## 🎯 Common Use Cases

### Discover DEXes on Different Blockchains

```bash
# Ethereum
./NeoZorKDEXArb -scan ethereum 10000

# Binance Smart Chain
./NeoZorKDEXArb -scan bsc 10000

# Polygon
./NeoZorKDEXArb -scan polygon 10000
```

### Analyze Specific DEX

```bash
# Show pools for Uniswap V2 (Ethereum)
./NeoZorKDEXArb -showPOOLS ethereum 0x5C69bEe701ef814a2B6a3EDD4B1652CB9cc5aA6f

# Show tokens for PancakeSwap (BSC)
./NeoZorKDEXArb -showTOKENS bsc 0xcA143Ce32Fe78f1f7019d7d551a6402fC5350c73
```

### Find Specific Tokens

```bash
# Find USDT across all Ethereum DEXes
./NeoZorKDEXArb -findTOKENS ethereum 0xdAC17F958D2ee523a2206206994597C13D831ec7
```

## 📊 What You'll Get

### Output Files

- **`neozork-config`**: Configuration and discovered DEX data
- **`neozork-scan-stat`**: Performance statistics
- **Console Output**: Real-time colored information

### Data Collected

- **DEX Factory Contracts**: Addresses of DEX factories
- **Liquidity Pools**: Pool addresses and token pairs
- **Trading Statistics**: 24-hour volume and transaction counts
- **Token Information**: Token addresses and metadata

## ⚡ Performance Tips

### Optimal Settings

| Use Case | Block Range | Threads | Expected Time |
|----------|-------------|---------|---------------|
| Quick Test | 1,000-5,000 | 3-4 | 1-5 minutes |
| Discovery | 10,000-50,000 | 6-8 | 10-30 minutes |
| Deep Analysis | 100,000+ | 8-12 | 1+ hours |

### System Requirements

- **CPU**: 4+ cores recommended
- **RAM**: 4GB+ available
- **Network**: Stable internet connection
- **Storage**: 100MB+ free space

## 🔧 Troubleshooting

### Common Issues

**Build fails:**
```bash
# Make sure you have all dependencies
brew install cmake curl  # macOS
sudo apt install build-essential cmake libcurl4-openssl-dev  # Ubuntu
```

**Scan is slow:**
```bash
# Reduce block range and thread count
./NeoZorKDEXArb -scan ethereum 1000  # Start small
```

**No DEXes found:**
```bash
# Try different block ranges
./NeoZorKDEXArb -scan ethereum 10000  # Increase range
./NeoZorKDEXArb -scan ethereum 1000   # Try recent blocks
```

## 📈 Next Steps

1. **Read the full documentation**: [Build and Usage Guide](BUILD_AND_USAGE.md)
2. **Explore project structure**: [Project Description](PROJECT_DESCRIPTION.md)
3. **Browse all documentation**: [Documentation Index](README.md)
3. **Try different blockchains**: BSC, Polygon, Fantom, Avalanche
4. **Analyze specific DEXes**: Use discovered factory addresses
5. **Customize configuration**: Edit `neozork-config` for your needs

## 🆘 Need Help?

- Check the [troubleshooting section](BUILD_AND_USAGE.md#troubleshooting)
- Verify your internet connection and RPC endpoint availability
- Start with small block ranges for testing
- Review the configuration file for errors

---

**Ready to start?** Run your first scan now:

```bash
./NeoZorKDEXArb -scan ethereum 5000
```
