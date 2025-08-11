# First Steps with DEXArb

Congratulations! You've successfully built DEXArb. Now let's run your first arbitrage scan and explore the basic features.

## 🚀 Your First Run

### 1. Check the Application
```bash
# Navigate to your build directory
cd build

# Check if the binary was created
ls -la DEXArb

# Get help information
./DEXArb --help
./DEXArb -h
```

### 2. Basic Command Structure
```bash
# General syntax
./DEXArb [command] [options] [parameters]

# Examples
./DEXArb --version                    # Check version
./DEXArb --help                       # Show help
./DEXArb --scan                       # Start scanning
./DEXArb --config config.json         # Use config file
```

## 🔍 Your First Scan

### Basic Network Scan
```bash
# Scan Ethereum mainnet
./DEXArb --scan --network ethereum

# Scan with verbose output
./DEXArb --scan --network ethereum --verbose

# Scan with specific timeout
./DEXArb --scan --network ethereum --timeout 30
```

### Multi-Network Scan
```bash
# Scan multiple networks
./DEXArb --scan --networks ethereum,polygon,arbitrum

# Scan all available networks
./DEXArb --scan --networks all
```

### Token-Specific Scan
```bash
# Scan for specific token pairs
./DEXArb --scan --network ethereum --tokens USDC,ETH

# Scan with custom token addresses
./DEXArb --scan --network ethereum --token-addresses 0xA0b86a33E6441b8c4C8C8C8C8C8C8C8C8C8C8C8C
```

## ⚙️ Configuration

### Create Basic Config File
```json
{
  "networks": {
    "ethereum": {
      "rpc_url": "https://mainnet.infura.io/v3/YOUR_API_KEY",
      "scan_interval": 30,
      "max_gas_price": 100
    },
    "polygon": {
      "rpc_url": "https://polygon-rpc.com",
      "scan_interval": 15,
      "max_gas_price": 50
    }
  },
  "scanning": {
    "min_profit_threshold": 0.5,
    "max_slippage": 1.0,
    "timeout": 30
  },
  "logging": {
    "level": "info",
    "file": "logs/dexarb.log"
  }
}
```

### Use Configuration
```bash
# Load configuration file
./DEXArb --config config.json

# Override config with command line
./DEXArb --config config.json --min-profit 1.0 --timeout 60
```

## 📊 Understanding Output

### Scan Results Format
```
[INFO] Starting arbitrage scan on Ethereum mainnet
[INFO] Connected to RPC endpoint: https://mainnet.infura.io/v3/...
[INFO] Scanning DEX pools...
[INFO] Found 15 pools with potential arbitrage
[INFO] Analyzing profit opportunities...
[INFO] Top opportunities:
  1. Pool: Uniswap V3 USDC/ETH
     Profit: 2.45%
     Gas cost: $15.30
     Net profit: $89.70
  2. Pool: SushiSwap USDC/ETH
     Profit: 1.87%
     Gas cost: $12.45
     Net profit: $67.55
```

### Log Levels
```bash
# Different verbosity levels
./DEXArb --scan --network ethereum --log-level error    # Only errors
./DEXArb --scan --network ethereum --log-level warn     # Warnings and errors
./DEXArb --scan --network ethereum --log-level info     # Information (default)
./DEXArb --scan --network ethereum --log-level debug    # Detailed debugging
./DEXArb --scan --network ethereum --log-level trace    # Maximum detail
```

## 🎯 Common Use Cases

### 1. Market Analysis
```bash
# Get market overview
./DEXArb --market-overview --network ethereum

# Analyze specific token
./DEXArb --analyze-token ETH --network ethereum

# Compare DEX platforms
./DEXArb --compare-dex --tokens USDC,ETH --network ethereum
```

### 2. Profit Monitoring
```bash
# Monitor for opportunities above threshold
./DEXArb --scan --network ethereum --min-profit 1.0 --continuous

# Alert on high-profit opportunities
./DEXArb --scan --network ethereum --min-profit 5.0 --alert

# Export results to file
./DEXArb --scan --network ethereum --export results.json
```

### 3. Historical Analysis
```bash
# Analyze historical data
./DEXArb --historical --network ethereum --days 7

# Export historical data
./DEXArb --historical --network ethereum --days 30 --export history.csv
```

## 🔧 Troubleshooting

### Common Issues

#### Connection Problems
```bash
# Check RPC endpoint
./DEXArb --test-connection --network ethereum

# Test with different endpoint
./DEXArb --scan --network ethereum --rpc-url https://alternative-rpc.com

# Check network status
./DEXArb --network-status --network ethereum
```

#### Performance Issues
```bash
# Reduce scan frequency
./DEXArb --scan --network ethereum --scan-interval 60

# Limit concurrent requests
./DEXArb --scan --network ethereum --max-concurrent 5

# Use lower verbosity
./DEXArb --scan --network ethereum --log-level warn
```

#### Memory Issues
```bash
# Limit memory usage
./DEXArb --scan --network ethereum --max-memory 1024

# Use streaming mode
./DEXArb --scan --network ethereum --streaming

# Process in batches
./DEXArb --scan --network ethereum --batch-size 100
```

## 📈 Advanced Features

### Custom Filters
```bash
# Filter by pool size
./DEXArb --scan --network ethereum --min-liquidity 1000000

# Filter by volume
./DEXArb --scan --network ethereum --min-volume 500000

# Filter by DEX
./DEXArb --scan --network ethereum --dex uniswap,sushiswap
```

### Export Options
```bash
# Export to different formats
./DEXArb --scan --network ethereum --export results.json    # JSON
./DEXArb --scan --network ethereum --export results.csv     # CSV
./DEXArb --scan --network ethereum --export results.xml     # XML

# Export with specific fields
./DEXArb --scan --network ethereum --export results.csv --fields pool,profit,gas_cost
```

### Integration
```bash
# Output for scripts
./DEXArb --scan --network ethereum --output-format script

# Machine-readable output
./DEXArb --scan --network ethereum --output-format machine

# Custom delimiter
./DEXArb --scan --network ethereum --output-format csv --delimiter ";"
```

## 🎉 Congratulations!

You've successfully:
- ✅ Built DEXArb
- ✅ Run your first scan
- ✅ Learned basic commands
- ✅ Understood output format
- ✅ Explored configuration options

## 📚 Next Steps

### Immediate Actions
1. **Explore more networks** - Try different blockchain networks
2. **Customize configuration** - Create your own config files
3. **Set up monitoring** - Run continuous scans
4. **Analyze results** - Export and analyze data

### Advanced Topics
- [**Development Guide**](../../development/development-guide.md) - Contribute to the project
- [**API Reference**](../../development/api/README.md) - Programmatic usage
- [**Deployment Guide**](../../deployment/README.md) - Production setup
- [**Testing Guide**](../../testing/README.md) - Quality assurance

### Community
- **GitHub Issues** - Report bugs and request features
- **Discussions** - Join community conversations
- **Contributing** - Help improve DEXArb

---

*Ready to explore more? Check out the advanced features and start building your arbitrage strategies!*
