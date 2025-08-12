# New Flags Implementation

## Overview

This document describes the implementation of three new command-line flags for the DEX Arbitrage Scanner:

1. `-dexes` - Show all known DEXes organized by blockchain
2. `-showPOOLS <blockchain>` - Show all pools found in a blockchain (without requiring DEX parameter)
3. `-showTOKENS <blockchain>` - Show all tokens found in a blockchain (without requiring DEX parameter)

## Implementation Details

### 1. New Command Type: SHOW_ALL_DEXES

**Location**: `include/cli/command_parser.h`

Added new command type to the `CommandType` enum:
```cpp
enum class CommandType {
    // ... existing types ...
    SHOW_ALL_DEXES,
    // ... other types ...
};
```

**Parser Support**: `src/cli/command_parser.cpp`

Added support for `-dexes` and `--dexes` flags:
```cpp
} else if (flag == "-dexes" || flag == "--dexes") {
    return CommandType::SHOW_ALL_DEXES;
```

### 2. Modified Command Parsing

**Location**: `src/cli/command_parser.cpp`

Updated parsing logic for `SHOW_POOLS` and `SHOW_TOKENS` to support blockchain-only parameters:

```cpp
case CommandType::SHOW_POOLS:
case CommandType::SHOW_TOKENS:
    if (argc >= 4) {
        // Existing logic for blockchain + DEX
        cmd.dex_name = argv[3];
        cmd.is_valid = true;
    } else if (argc == 3) {
        // NEW: Allow blockchain-only for showPOOLS and showTOKENS
        std::string blockchain_or_id = argv[2];
        if (is_network_id(blockchain_or_id)) {
            cmd.blockchain = network_id_to_blockchain(blockchain_or_id);
        } else {
            cmd.blockchain = blockchain_or_id;
        }
        cmd.is_valid = true;
    } else {
        cmd.error_message = "Command requires blockchain parameter";
    }
    break;
```

### 3. Updated Function Requirements

**Location**: `src/cli/command_parser.cpp`

Modified `requires_dex()` function to no longer require DEX for `SHOW_POOLS` and `SHOW_TOKENS`:

```cpp
bool CommandParser::requires_dex(CommandType type) {
    return type == CommandType::FIND_TOKEN; // Only FIND_TOKEN requires DEX now
}
```

### 4. New Functions Implementation

**Location**: `src/network/queries.cpp`

#### show_all_dexes_by_blockchain()

Displays all known DEXes organized by blockchain:

```cpp
void show_all_dexes_by_blockchain() {
    std::map<std::string, std::vector<std::string>> blockchain_dexes = {
        {"Ethereum", {"Uniswap V2", "Uniswap V3", "SushiSwap", "Curve", "Balancer", ...}},
        {"Fantom", {"SpookySwap", "SpiritSwap", "Beethoven X", "TombSwap", ...}},
        {"BSC", {"PancakeSwap", "Biswap", "ApeSwap", "DODO", ...}},
        // ... other blockchains
    };
    
    // Display formatted output
}
```

#### show_all_pools()

Shows all pools across all DEXes in a specified blockchain:

```cpp
void show_all_pools(const std::vector<RpcEndpoint>& rpc_endpoints, const std::string& blockchain) {
    // Load DEXes from config
    // Fetch pools from all DEXes
    // Display pools grouped by DEX
}
```

### 5. Main Application Integration

**Location**: `src/main.cpp`

Added handling for new command types:

```cpp
case cli::CommandType::SHOW_ALL_DEXES: {
    modern_utils::Logger::info("Showing all known DEXes by blockchain");
    show_all_dexes_by_blockchain();
    break;
}

case cli::CommandType::SHOW_POOLS: {
    if (cmd.dex_name.empty()) {
        // Show all pools across all DEXes
        show_all_pools(rpc_endpoints, normalized_blockchain);
    } else {
        // Show pools for specific DEX
        show_pools(rpc_endpoints, cmd.dex_name);
    }
    break;
}
```

### 6. Help Display Updates

**Location**: `src/cli/help_display.cpp`

Updated help text to reflect new functionality:

```cpp
output << "   " << format_text("DEX Analysis", GREEN) << '\n';
output << "      " << format_text("-dexes", BLUE) << format_column("", 8) << "                              " << format_text("Show all known DEXes by blockchain", CYAN) << '\n';
output << "      " << format_text("-showDEXES", BLUE) << format_column("", 4) << "<blockchain>              " << format_text("Show discovered DEXes", CYAN) << '\n';
output << "      " << format_text("-showPOOLS", BLUE) << format_column("", 4) << "<blockchain> [DEX]        " << format_text("Show pools (all or specific DEX)", CYAN) << '\n';
output << "      " << format_text("-showTOKENS", BLUE) << format_column("", 3) << "<blockchain> [DEX]        " << format_text("Show tokens (all or specific DEX)", CYAN) << '\n\n';
```

## Usage Examples

### 1. Show All Known DEXes
```bash
# Show all known DEXes organized by blockchain
./dexarb -dexes

# Output:
# Known DEXes by Blockchain:
# ============================================================
# Ethereum:
#   • Uniswap V2
#   • Uniswap V3
#   • SushiSwap
#   • Curve
#   • Balancer
#   ...
# 
# Fantom:
#   • SpookySwap
#   • SpiritSwap
#   • Beethoven X
#   ...
```

### 2. Show All Pools in Blockchain
```bash
# Show all pools in Fantom (using network ID)
./dexarb -showPOOLS 250

# Show all pools in Ethereum
./dexarb -showPOOLS ethereum

# Output:
# All pools found in fantom:
# ============================================================
# DEX: SpookySwap (0x152eE697f2E276fA89E96742e9bB9aB51FcFcA15)
#   Pool: 0x1234..., Token0: 0xabcd..., Token1: 0xefgh..., Liquidity: 1000000
#   Pool: 0x5678..., Token0: 0xijkl..., Token1: 0xmnop..., Liquidity: 2000000
# 
# DEX: SpiritSwap (0xEF45d134b73241eDa7703fa787148D9C9F4950b0)
#   Pool: 0x9abc..., Token0: 0xqrst..., Token1: 0xuvwx..., Liquidity: 1500000
```

### 3. Show All Tokens in Blockchain
```bash
# Show all tokens in BSC
./dexarb -showTOKENS bsc

# Show all tokens in Polygon (using network ID)
./dexarb -showTOKENS 137

# Output:
# All unique tokens found across all DEXes:
# Total unique tokens: 1250
# Token: 0x1234567890abcdef...
# Token: 0xabcdef1234567890...
# Token: 0x9876543210fedcba...
# ...
```

## Testing

### Unit Tests
**Location**: `tests/cpp/test_new_features.cpp`

Comprehensive test coverage for:
- `-dexes` flag parsing
- `-showPOOLS` without DEX parameter
- `-showTOKENS` without DEX parameter
- Backward compatibility with existing DEX parameters
- Command descriptions
- Function requirements

### Test Cases
1. **Flag Parsing**: Verify correct command type assignment
2. **Parameter Handling**: Test with and without DEX parameters
3. **Network ID Support**: Test with blockchain names and network IDs
4. **Error Handling**: Verify proper error messages
5. **Backward Compatibility**: Ensure existing functionality still works

## Backward Compatibility

All existing functionality remains unchanged:
- `-showPOOLS <blockchain> <DEX>` still works
- `-showTOKENS <blockchain> <DEX>` still works
- All other flags continue to function as before

## Performance Considerations

1. **DEX Listing**: Static data, no performance impact
2. **All Pools**: May be slower as it queries all DEXes
3. **All Tokens**: May be slower as it processes all pools
4. **Caching**: Consider implementing caching for frequently accessed data

## Future Enhancements

1. **Pagination**: For large datasets
2. **Filtering**: By liquidity, volume, etc.
3. **Sorting**: By various metrics
4. **Export**: CSV/JSON output options
5. **Real-time Data**: Live pool/token information
