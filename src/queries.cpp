//
//  queries.cpp
//  NeoZorKDEXArb
//
//  Created by Rostyslav S. on 18.03.2025.
//
#include "queries.h"
#include "dex_pools.h"      // For pool functions
#include "dex_tokens.h"     // For token functions
#include "config_manager.h" // For loading DEXes
#include <iostream>         // For console output
#include <thread>           // For multi-threading
#include <algorithm>        // For std::find_if

void show_pools(const std::vector<RpcEndpoint>& rpc_endpoints, const std::string& dex_identifier) {
    // Load DEXes from config
    std::vector<DexInfo> dex_list = load_dexes_from_config(); // Get DEX list
    auto it = std::find_if(dex_list.begin(), dex_list.end(),
                           [&dex_identifier](const DexInfo& dex) { return dex.name == dex_identifier || dex.factory_address == dex_identifier; }); // Find DEX
    if (it == dex_list.end()) { // Check if DEX was found
        std::cerr << RED << "DEX not found" << RESET << '\n'; // Report error
        return; // Exit function
    }
    DexInfo& dex = *it; // Reference to found DEX

    // Fetch fresh pool data
    dex.pools.clear(); // Clear existing pools
    for (uint64_t i = 0; i < dex.pool_count; ++i) { // Loop through pool indices
        FunctionStats stats; // Stats for this operation
        std::string addr = get_pool_address(rpc_endpoints[0].url, dex.factory_address, i, rpc_endpoints[0].request_limit, stats); // Get pool address
        if (!addr.empty()) { // Check if address is valid
            auto [token0, token1] = get_pool_tokens(rpc_endpoints[0].url, addr, rpc_endpoints[0].request_limit, stats); // Get tokens
            uint64_t liquidity = get_pool_liquidity(rpc_endpoints[0].url, addr, rpc_endpoints[0].request_limit, stats); // Get liquidity
            dex.pools.push_back({addr, token0, token1, liquidity}); // Add pool to DEX
        }
    }

    // Display pool information
    std::cout << GREEN << "Pools for " << dex.name << " (" << dex.factory_address << "):" << RESET << '\n'; // Header
    for (const auto& pool : dex.pools) { // Loop through pools
        std::cout << YELLOW << "Pool: " << pool.address << ", Token0: " << pool.token0 << ", Token1: " << pool.token1
                  << ", Liquidity: " << pool.liquidity << RESET << '\n'; // Print pool details
    }
}

void show_tokens(const std::vector<RpcEndpoint>& rpc_endpoints, const std::string& dex_identifier) {
    // Load DEXes from config
    std::vector<DexInfo> dex_list = load_dexes_from_config(); // Get DEX list
    auto it = std::find_if(dex_list.begin(), dex_list.end(),
                           [&dex_identifier](const DexInfo& dex) { return dex.name == dex_identifier || dex.factory_address == dex_identifier; }); // Find DEX
    if (it == dex_list.end()) { // Check if DEX was found
        std::cerr << RED << "DEX not found" << RESET << '\n'; // Report error
        return; // Exit function
    }
    DexInfo& dex = *it; // Reference to found DEX

    // Fetch tokens and ensure uniqueness
    std::vector<std::string> unique_tokens; // List of unique tokens
    dex.pools.clear(); // Clear existing pools
    for (uint64_t i = 0; i < dex.pool_count; ++i) { // Loop through pool indices
        FunctionStats stats; // Stats for this operation
        std::string addr = get_pool_address(rpc_endpoints[0].url, dex.factory_address, i, rpc_endpoints[0].request_limit, stats); // Get pool address
        if (!addr.empty()) { // Check if address is valid
            auto [token0, token1] = get_pool_tokens(rpc_endpoints[0].url, addr, rpc_endpoints[0].request_limit, stats); // Get tokens
            if (std::find(unique_tokens.begin(), unique_tokens.end(), token0) == unique_tokens.end()) unique_tokens.push_back(token0); // Add token0 if unique
            if (std::find(unique_tokens.begin(), unique_tokens.end(), token1) == unique_tokens.end()) unique_tokens.push_back(token1); // Add token1 if unique
        }
    }

    // Display token information
    std::cout << GREEN << "Tokens for " << dex.name << " (" << dex.factory_address << "):" << RESET << '\n'; // Header
    for (const auto& token : unique_tokens) { // Loop through tokens
        std::cout << YELLOW << "Token: " << token << RESET << '\n'; // Print token address
    }
}

void find_token_in_dex(const std::vector<RpcEndpoint>& rpc_endpoints, const std::string& dex_identifier, const std::string& token_address) {
    // Load DEXes from config
    std::vector<DexInfo> dex_list = load_dexes_from_config(); // Get DEX list
    auto it = std::find_if(dex_list.begin(), dex_list.end(),
                           [&dex_identifier](const DexInfo& dex) { return dex.name == dex_identifier || dex.factory_address == dex_identifier; }); // Find DEX
    if (it == dex_list.end()) { // Check if DEX was found
        std::cerr << RED << "DEX not found" << RESET << '\n'; // Report error
        return; // Exit function
    }
    DexInfo& dex = *it; // Reference to found DEX

    // Search for token in pools
    dex.pools.clear(); // Clear existing pools
    for (uint64_t i = 0; i < dex.pool_count; ++i) { // Loop through pool indices
        FunctionStats stats; // Stats for this operation
        std::string addr = get_pool_address(rpc_endpoints[0].url, dex.factory_address, i, rpc_endpoints[0].request_limit, stats); // Get pool address
        if (!addr.empty()) { // Check if address is valid
            auto [token0, token1] = get_pool_tokens(rpc_endpoints[0].url, addr, rpc_endpoints[0].request_limit, stats); // Get tokens
            uint64_t liquidity = get_pool_liquidity(rpc_endpoints[0].url, addr, rpc_endpoints[0].request_limit, stats); // Get liquidity
            if (token0 == token_address || token1 == token_address) { // Check if token matches
                dex.pools.push_back({addr, token0, token1, liquidity}); // Add matching pool
            }
        }
    }

    // Display results
    std::cout << GREEN << "Token " << token_address << " in " << dex.name << ":" << RESET << '\n'; // Header
    for (const auto& pool : dex.pools) { // Loop through matching pools
        std::cout << YELLOW << "Pool: " << pool.address << ", Token0: " << pool.token0 << ", Token1: " << pool.token1
                  << ", Liquidity: " << pool.liquidity << RESET << '\n'; // Print pool details
    }
}

void find_tokens_across_dexes(const std::vector<RpcEndpoint>& rpc_endpoints, const std::string& token_address) {
    // Load DEXes from config
    std::vector<DexInfo> dex_list = load_dexes_from_config(); // Get DEX list
    for (auto& dex : dex_list) { // Loop through all DEXes
        dex.pools.clear(); // Clear existing pools
        for (uint64_t i = 0; i < dex.pool_count; ++i) { // Loop through pool indices
            FunctionStats stats; // Stats for this operation
            std::string addr = get_pool_address(rpc_endpoints[0].url, dex.factory_address, i, rpc_endpoints[0].request_limit, stats); // Get pool address
            if (!addr.empty()) { // Check if address is valid
                auto [token0, token1] = get_pool_tokens(rpc_endpoints[0].url, addr, rpc_endpoints[0].request_limit, stats); // Get tokens
                uint64_t liquidity = get_pool_liquidity(rpc_endpoints[0].url, addr, rpc_endpoints[0].request_limit, stats); // Get liquidity
                if (token0 == token_address || token1 == token_address) { // Check if token matches
                    dex.pools.push_back({addr, token0, token1, liquidity}); // Add matching pool
                }
            }
        }
    }

    // Display results
    std::cout << GREEN << "Pools with " << token_address << ":" << RESET << '\n'; // Header
    for (const auto& dex : dex_list) { // Loop through DEXes
        if (!dex.pools.empty()) { // Check if DEX has matching pools
            std::cout << YELLOW << "DEX: " << dex.name << " (" << dex.factory_address << ")" << RESET << '\n'; // Print DEX info
            for (const auto& pool : dex.pools) { // Loop through pools
                std::cout << "  Pool: " << pool.address << ", Token0: " << pool.token0 << ", Token1: " << pool.token1
                          << ", Liquidity: " << pool.liquidity << '\n'; // Print pool details
            }
        }
    }
}

void show_dexes(const std::vector<RpcEndpoint>& rpc_endpoints) {
    // Load and display all DEXes
    std::vector<DexInfo> dex_list = load_dexes_from_config(); // Get DEX list
    std::cout << GREEN << "Found DEXes:" << RESET << '\n'; // Header
    for (const auto& dex : dex_list) { // Loop through DEXes
        std::cout << YELLOW << "Name: " << dex.name << ", Address: " << dex.factory_address << ", Pools: " << dex.pool_count << RESET << '\n'; // Print DEX details
    }
}
