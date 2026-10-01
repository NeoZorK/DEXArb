# Analysis: Scan Command Shows 0 Pools Issue

## Problem Description

When running the `-scan` command (e.g., `-scan avalanche 1000`), all DEXes show 0 pools, even though some of them should have active pools.

## Root Cause Analysis

### 1. Invalid DEX Addresses in Configuration

Many addresses in the `neozork-config` file are not DEX factory contracts:

- **Routers instead of Factories**: Addresses like `0x7a250d5630B4cF539739dF2C5dAcb4c659F2488D` (Uniswap V2 Router) and `0xE592427A0AEce92De3Edee1F18E0157C05861564` (Uniswap V3 Router) are routers, not factories
- **Invalid Addresses**: Some addresses like `0x7D5A56714658E5B9BfBfB8B8B8B8B8B8B8B8B8B8` are clearly invalid
- **Non-DEX Contracts**: Some addresses are other types of contracts (aggregators, etc.)

### 2. RPC Call Issues

The `get_pool_count` function encounters several issues:

- **"execution reverted" errors**: Many contracts return this error when called with pool count methods
- **Empty results ("0x")**: Some contracts return empty results instead of actual pool counts
- **Wrong method signatures**: Different DEX types use different method signatures for getting pool counts

### 3. Method Signature Problems

The current implementation tries these method signatures:
- `0x90e18a69` - `allPairsLength()` (Uniswap V2 style)
- `0x783cca1c` - `poolCount()`
- `0xb4d9b203` - `getPoolCount()`
- `0x112c2569` - `poolLength()`
- `0x1e83409a` - `allPairs(uint256)` (requires parameter)

But many DEXes use different signatures or require different parameters.

## Current Status

### Tested Blockchains

1. **Ethereum**: All DEXes show 0 pools
2. **Fantom**: All DEXes show 0 pools  
3. **BSC**: All DEXes show 0 pools
4. **Polygon**: All DEXes show 0 pools
5. **Avalanche**: All DEXes show 0 pools

### Working RPC Endpoints

The RPC endpoints are working correctly:
- Block numbers are fetched successfully
- RPC calls are made without connection errors
- Responses are received properly

## Solutions

### 1. Fix DEX Configuration

Update `neozork-config` with correct factory addresses:

```json
{
  "ethereum": {
    "dex": [
      {"name": "Uniswap V2", "address": "0x5C69bEe701ef814a2B6a3EDD4B1652CB9cc5aA6f", "pools": 0},
      {"name": "SushiSwap", "address": "0xC0AEe478e3658e2610c5F7A4A2E1777cE9e4f2Ac", "pools": 0}
    ]
  },
  "avalanche": {
    "dex": [
      {"name": "TraderJoe V2", "address": "0x9Ad6C38BE94206cA50bb0d90783181662f0Cfa10", "pools": 0},
      {"name": "Pangolin", "address": "0xefa94DE7a4656D787667C749f7E1223D71E9FD88", "pools": 0}
    ]
  }
}
```

### 2. Improve Pool Count Detection

Enhance the `get_pool_count` function to:

1. **Add more method signatures** for different DEX types
2. **Handle parameterized calls** (like `allPairs(uint256)`)
3. **Add DEX-specific detection** based on contract address
4. **Implement fallback methods** for different DEX architectures

### 3. Add DEX Type Detection

Create a mapping of known DEX addresses to their types and appropriate method signatures:

```cpp
struct DexInfo {
    std::string name;
    std::string address;
    std::string type;  // "uniswap_v2", "uniswap_v3", "sushiswap", etc.
    std::vector<std::string> pool_count_methods;
};
```

### 4. Implement Better Error Handling

- **Skip invalid contracts** instead of trying all methods
- **Add contract validation** before attempting pool count calls
- **Implement retry logic** with different RPC endpoints
- **Add detailed logging** for debugging

## Immediate Actions

1. **Clean up configuration**: Remove invalid addresses and routers
2. **Add correct factory addresses**: Research and add proper DEX factory addresses
3. **Test with known working DEXes**: Start with well-known DEXes like Uniswap V2
4. **Add validation**: Implement contract validation before pool count calls

## Expected Results

After implementing these fixes:
- Valid DEX factories should show actual pool counts
- Invalid addresses should be skipped or marked as invalid
- Better error messages should help with debugging
- The scan command should provide meaningful results

## Files to Modify

1. `neozork-config` - Update with correct DEX addresses
2. `src/dex/dex_pools.cpp` - Improve pool count detection
3. `include/dex/dex_pools.h` - Add new structures and methods
4. `src/dex/dex_scanner.cpp` - Add DEX type detection

## Testing Strategy

1. Test with individual DEX addresses first
2. Verify pool counts against blockchain explorers
3. Test with different RPC endpoints
4. Validate results across different blockchains
