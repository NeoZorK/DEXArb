# Configuration Guide

This guide covers all configuration options for DEXArb, including configuration files, environment variables, and command-line parameters.

## 📁 Configuration Files

### Default Locations
```bash
# System-wide configuration
/etc/dexarb/config.json

# User configuration
~/.config/dexarb/config.json

# Project configuration
./config.json

# Custom location
./DEXArb --config /path/to/config.json
```

### Basic Configuration Structure
```json
{
  "application": {
    "name": "DEXArb",
    "version": "2.0.0",
    "log_level": "info",
    "log_file": "logs/dexarb.log"
  },
  "networks": {
    "ethereum": {
      "enabled": true,
      "rpc_url": "https://mainnet.infura.io/v3/YOUR_API_KEY",
      "chain_id": 1,
      "scan_interval": 30,
      "max_gas_price": 100,
      "timeout": 30
    },
    "polygon": {
      "enabled": true,
      "rpc_url": "https://polygon-rpc.com",
      "chain_id": 137,
      "scan_interval": 15,
      "max_gas_price": 50,
      "timeout": 20
    }
  },
  "scanning": {
    "enabled": true,
    "min_profit_threshold": 0.5,
    "max_slippage": 1.0,
    "batch_size": 100,
    "max_concurrent": 10,
    "timeout": 30
  },
  "dex_platforms": {
    "uniswap_v2": {
      "enabled": true,
      "version": "2.0",
      "priority": 1
    },
    "uniswap_v3": {
      "enabled": true,
      "version": "3.0",
      "priority": 2
    },
    "sushiswap": {
      "enabled": true,
      "version": "2.0",
      "priority": 3
    }
  },
  "tokens": {
    "whitelist": [
      "USDC",
      "ETH",
      "USDT",
      "WBTC"
    ],
    "blacklist": [
      "SCAM_TOKEN",
      "TEST_TOKEN"
    ]
  },
  "alerts": {
    "enabled": true,
    "min_profit": 2.0,
    "notification_methods": ["console", "file", "webhook"],
    "webhook_url": "https://your-webhook.com/notify"
  },
  "export": {
    "enabled": true,
    "format": "json",
    "directory": "exports",
    "filename_template": "dexarb_{timestamp}_{network}.{format}"
  }
}
```

## 🔧 Network Configuration

### Ethereum Mainnet
```json
{
  "ethereum": {
    "enabled": true,
    "rpc_url": "https://mainnet.infura.io/v3/YOUR_API_KEY",
    "chain_id": 1,
    "scan_interval": 30,
    "max_gas_price": 100,
    "timeout": 30,
    "block_time": 12,
    "confirmations": 12,
    "max_priority_fee": 2
  }
}
```

### Polygon Network
```json
{
  "polygon": {
    "enabled": true,
    "rpc_url": "https://polygon-rpc.com",
    "chain_id": 137,
    "scan_interval": 15,
    "max_gas_price": 50,
    "timeout": 20,
    "block_time": 2,
    "confirmations": 256,
    "max_priority_fee": 1
  }
}
```

### Arbitrum One
```json
{
  "arbitrum": {
    "enabled": true,
    "rpc_url": "https://arb1.arbitrum.io/rpc",
    "chain_id": 42161,
    "scan_interval": 20,
    "max_gas_price": 0.1,
    "timeout": 25,
    "block_time": 1,
    "confirmations": 1,
    "max_priority_fee": 0.01
  }
}
```

## ⚙️ Scanning Configuration

### Basic Scanning
```json
{
  "scanning": {
    "enabled": true,
    "mode": "continuous",
    "scan_interval": 30,
    "min_profit_threshold": 0.5,
    "max_slippage": 1.0,
    "timeout": 30
  }
}
```

### Advanced Scanning
```json
{
  "scanning": {
    "enabled": true,
    "mode": "batch",
    "batch_size": 100,
    "max_concurrent": 10,
    "retry_attempts": 3,
    "retry_delay": 5,
    "circuit_breaker": {
      "enabled": true,
      "failure_threshold": 5,
      "reset_timeout": 300
    }
  }
}
```

### Performance Tuning
```json
{
  "scanning": {
    "performance": {
      "max_memory": 2048,
      "cpu_limit": 0.8,
      "network_timeout": 30,
      "connection_pool_size": 20,
      "keep_alive": true
    }
  }
}
```

## 🏪 DEX Platform Configuration

### Uniswap V2
```json
{
  "uniswap_v2": {
    "enabled": true,
    "version": "2.0",
    "priority": 1,
    "factory_address": "0x5C69bEe701ef814a2B6a3EDD4B1652CB9cc5aA6f",
    "router_address": "0x7a250d5630B4cF539739dF2C5dAcb4c659F2488D",
    "max_pools_per_token": 100,
    "min_liquidity": 10000
  }
}
```

### Uniswap V3
```json
{
  "uniswap_v3": {
    "enabled": true,
    "version": "3.0",
    "priority": 2,
    "factory_address": "0x1F98431c8aD98523631AE4a59f267346ea31F984",
    "quoter_address": "0xb27308f9F90D607463bb33eA1BeBb41C27CE5AB6",
    "max_pools_per_token": 200,
    "min_liquidity": 50000,
    "fee_tiers": [500, 3000, 10000]
  }
}
```

### SushiSwap
```json
{
  "sushiswap": {
    "enabled": true,
    "version": "2.0",
    "priority": 3,
    "factory_address": "0xC0AEe478e3658e2610c5F7A4A2E1777cE9e4f2Ac",
    "router_address": "0xd9e1cE17f2641f24aE83637ab66a2cca9C378B9F",
    "max_pools_per_token": 80,
    "min_liquidity": 8000
  }
}
```

## 🪙 Token Configuration

### Token Lists
```json
{
  "tokens": {
    "whitelist": [
      {
        "symbol": "USDC",
        "address": "0xA0b86a33E6441b8c4C8C8C8C8C8C8C8C8C8C8C8C",
        "decimals": 6,
        "priority": 1
      },
      {
        "symbol": "ETH",
        "address": "0xC02aaA39b223FE8D0A0e5C4F27eAD9083C756Cc2",
        "decimals": 18,
        "priority": 1
      }
    ],
    "blacklist": [
      "0x1234567890123456789012345678901234567890",
      "0x0987654321098765432109876543210987654321"
    ],
    "custom_tokens": [
      {
        "symbol": "CUSTOM",
        "address": "0xCustomTokenAddress",
        "decimals": 18,
        "name": "Custom Token"
      }
    ]
  }
}
```

### Token Pairs
```json
{
  "token_pairs": {
    "enabled": true,
    "pairs": [
      {
        "base": "USDC",
        "quote": "ETH",
        "min_liquidity": 1000000,
        "priority": 1
      },
      {
        "base": "USDT",
        "quote": "ETH",
        "min_liquidity": 500000,
        "priority": 2
      }
    ]
  }
}
```

## 📊 Alert Configuration

### Basic Alerts
```json
{
  "alerts": {
    "enabled": true,
    "min_profit": 2.0,
    "notification_methods": ["console", "file"],
    "log_file": "logs/alerts.log"
  }
}
```

### Advanced Alerts
```json
{
  "alerts": {
    "enabled": true,
    "min_profit": 2.0,
    "notification_methods": ["console", "file", "webhook", "email"],
    "webhook": {
      "url": "https://your-webhook.com/notify",
      "method": "POST",
      "headers": {
        "Authorization": "Bearer YOUR_TOKEN"
      },
      "timeout": 10
    },
    "email": {
      "smtp_server": "smtp.gmail.com",
      "smtp_port": 587,
      "username": "your-email@gmail.com",
      "password": "your-app-password",
      "recipients": ["admin@company.com"]
    },
    "telegram": {
      "bot_token": "YOUR_BOT_TOKEN",
      "chat_id": "YOUR_CHAT_ID"
    }
  }
}
```

## 📤 Export Configuration

### Basic Export
```json
{
  "export": {
    "enabled": true,
    "format": "json",
    "directory": "exports",
    "filename_template": "dexarb_{timestamp}_{network}.{format}"
  }
}
```

### Advanced Export
```json
{
  "export": {
    "enabled": true,
    "formats": ["json", "csv", "xml"],
    "directory": "exports",
    "filename_template": "dexarb_{timestamp}_{network}_{format}.{ext}",
    "compression": "gzip",
    "retention": {
      "enabled": true,
      "max_files": 100,
      "max_age_days": 30
    },
    "fields": [
      "timestamp",
      "network",
      "pool",
      "profit",
      "gas_cost",
      "net_profit"
    ]
  }
}
```

## 🔐 Security Configuration

### API Keys and Secrets
```json
{
  "security": {
    "api_keys": {
      "infura": "YOUR_INFURA_API_KEY",
      "alchemy": "YOUR_ALCHEMY_API_KEY",
      "etherscan": "YOUR_ETHERSCAN_API_KEY"
    },
    "encryption": {
      "enabled": true,
      "algorithm": "AES-256-GCM",
      "key_file": "config/encryption.key"
    }
  }
}
```

### Access Control
```json
{
  "security": {
    "access_control": {
      "enabled": true,
      "allowed_ips": ["192.168.1.0/24", "10.0.0.0/8"],
      "rate_limiting": {
        "enabled": true,
        "max_requests_per_minute": 100
      }
    }
  }
}
```

## 🌍 Environment Variables

### Override Configuration
```bash
# Network settings
export DEXARB_ETHEREUM_RPC_URL="https://mainnet.infura.io/v3/YOUR_KEY"
export DEXARB_ETHEREUM_SCAN_INTERVAL="30"
export DEXARB_ETHEREUM_MAX_GAS_PRICE="100"

# Scanning settings
export DEXARB_SCANNING_MIN_PROFIT="0.5"
export DEXARB_SCANNING_TIMEOUT="30"
export DEXARB_SCANNING_BATCH_SIZE="100"

# Logging settings
export DEXARB_LOG_LEVEL="info"
export DEXARB_LOG_FILE="logs/dexarb.log"

# Export settings
export DEXARB_EXPORT_ENABLED="true"
export DEXARB_EXPORT_FORMAT="json"
export DEXARB_EXPORT_DIRECTORY="exports"
```

### Priority Order
1. **Command-line arguments** (highest priority)
2. **Environment variables**
3. **Configuration file**
4. **Default values** (lowest priority)

## 🔄 Configuration Reloading

### Hot Reload
```json
{
  "application": {
    "hot_reload": {
      "enabled": true,
      "watch_config": true,
      "reload_on_change": true,
      "check_interval": 30
    }
  }
}
```

### Reload Commands
```bash
# Reload configuration
./DEXArb --reload-config

# Reload specific section
./DEXArb --reload-config --section networks

# Validate configuration
./DEXArb --validate-config
```

## ✅ Configuration Validation

### Validate Configuration
```bash
# Validate entire config
./DEXArb --validate-config

# Validate specific section
./DEXArb --validate-config --section networks

# Check configuration syntax
./DEXArb --check-config-syntax
```

### Common Validation Errors
```json
{
  "validation_errors": [
    {
      "field": "networks.ethereum.rpc_url",
      "error": "Invalid URL format",
      "value": "invalid-url"
    },
    {
      "field": "scanning.min_profit_threshold",
      "error": "Value must be positive",
      "value": -1
    }
  ]
}
```

## 📚 Next Steps

After configuring DEXArb:
- [Run your first scan](first-steps.md)
- [Learn about deployment](../../deployment/README.md)
- [Explore advanced features](../../development/api/README.md)

---

*Configuration complete? Let's start scanning for arbitrage opportunities!*
