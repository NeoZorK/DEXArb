//
//  queries.cpp
//  NeoZorKDEXArb
//
//  Created by Rostyslav S. on 18.03.2025.
//
#include "network/queries.h"
#include "dex/dex_pools.h"      // For pool functions
#include "dex/dex_tokens.h"     // For token functions
#include "config/config_manager.h" // For loading DEXes
#include <iostream>         // For console output
#include <thread>           // For multi-threading
#include <algorithm>        // For std::find_if
#include <map>             // For std::map

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

    // Get RPC endpoint URL
    std::string rpc_url = "";
    if (!rpc_endpoints.empty()) {
        rpc_url = rpc_endpoints[0].url; // Use first RPC endpoint
    }

    // Fetch fresh pool data
    dex.pools.clear(); // Clear existing pools
    for (uint64_t i = 0; i < dex.pool_count; ++i) { // Loop through pool indices
        FunctionStats stats; // Stats for this operation
        std::string addr = get_pool_address(rpc_url, dex.factory_address, i, 0, stats); // Get pool address
        if (!addr.empty()) { // Check if address is valid
            auto [token0, token1] = get_pool_tokens(rpc_url, addr, 0, stats); // Get tokens
            uint64_t liquidity = get_pool_liquidity(rpc_url, addr, 0, stats); // Get liquidity
            dex.pools.push_back({addr, token0, token1, liquidity}); // Add pool to DEX
        }
    }

    // Display pool information
    std::cout << GREEN << "Pools for " << dex.name << " (" << dex.factory_address << "):" << RESET << '\n'; // Header
    if (dex.pools.empty()) {
        std::cout << YELLOW << "No pools found for this DEX" << RESET << '\n';
    } else {
        for (const auto& pool : dex.pools) { // Loop through pools
            std::cout << YELLOW << "Pool: " << pool.address << ", Token0: " << pool.token0 << ", Token1: " << pool.token1
                      << ", Liquidity: " << pool.liquidity << RESET << '\n'; // Print pool details
        }
    }
}

void show_tokens(const std::vector<RpcEndpoint>& /* rpc_endpoints */, const std::string& dex_identifier) {
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
        std::string addr = get_pool_address("", dex.factory_address, i, 0, stats); // Get pool address
        if (!addr.empty()) { // Check if address is valid
            auto [token0, token1] = get_pool_tokens("", addr, 0, stats); // Get tokens
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

void find_token_in_dex(const std::vector<RpcEndpoint>& /* rpc_endpoints */, const std::string& dex_identifier, const std::string& token_address) {
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
            std::string addr = get_pool_address("", dex.factory_address, i, 0, stats); // Get pool address
            if (!addr.empty()) { // Check if address is valid
                auto [token0, token1] = get_pool_tokens("", addr, 0, stats); // Get tokens
                uint64_t liquidity = get_pool_liquidity("", addr, 0, stats); // Get liquidity
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

void find_tokens_across_dexes(const std::vector<RpcEndpoint>& /* rpc_endpoints */, const std::string& token_address) {
    // Load DEXes from config
    std::vector<DexInfo> dex_list = load_dexes_from_config(); // Get DEX list
    for (auto& dex : dex_list) { // Loop through all DEXes
        dex.pools.clear(); // Clear existing pools
        for (uint64_t i = 0; i < dex.pool_count; ++i) { // Loop through pool indices
            FunctionStats stats; // Stats for this operation
            std::string addr = get_pool_address("", dex.factory_address, i, 0, stats); // Get pool address
            if (!addr.empty()) { // Check if address is valid
                auto [token0, token1] = get_pool_tokens("", addr, 0, stats); // Get tokens
                uint64_t liquidity = get_pool_liquidity("", addr, 0, stats); // Get liquidity
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

void show_dexes(const std::vector<RpcEndpoint>& /* rpc_endpoints */) {
    // Load and display all DEXes
    std::vector<DexInfo> dex_list = load_dexes_from_config(); // Get DEX list
    std::cout << GREEN << "Found DEXes:" << RESET << '\n'; // Header
    for (const auto& dex : dex_list) { // Loop through DEXes
        std::cout << YELLOW << "Name: " << dex.name << ", Address: " << dex.factory_address << ", Pools: " << dex.pool_count << RESET << '\n'; // Print DEX details
    }
}

void show_all_tokens(const std::vector<RpcEndpoint>& /* rpc_endpoints */) {
    // Load all DEXes from config
    std::vector<DexInfo> dex_list = load_dexes_from_config(); // Get DEX list
    
    // Vector to store unique tokens across all DEXes
    std::vector<std::string> all_unique_tokens;
    
    // Collect tokens from all DEXes
    for (auto& dex : dex_list) { // Loop through all DEXes
        dex.pools.clear(); // Clear existing pools
        
        // Update pool count in real-time if it's 0
        if (dex.pool_count == 0) {
            FunctionStats stats;
            dex.pool_count = get_pool_count("", dex.factory_address, 0, stats);
            std::cout << "DEBUG: Updated pool count for " << dex.name << " to " << dex.pool_count << std::endl;
        }
        
        for (uint64_t i = 0; i < dex.pool_count; ++i) { // Loop through pool indices
            FunctionStats stats; // Stats for this operation
            std::string addr = get_pool_address("", dex.factory_address, i, 0, stats); // Get pool address
            if (!addr.empty()) { // Check if address is valid
                auto [token0, token1] = get_pool_tokens("", addr, 0, stats); // Get tokens
                if (!token0.empty() && std::find(all_unique_tokens.begin(), all_unique_tokens.end(), token0) == all_unique_tokens.end()) {
                    all_unique_tokens.push_back(token0); // Add token0 if valid and unique
                }
                if (!token1.empty() && std::find(all_unique_tokens.begin(), all_unique_tokens.end(), token1) == all_unique_tokens.end()) {
                    all_unique_tokens.push_back(token1); // Add token1 if valid and unique
                }
            }
        }
    }
    
    // Display all unique tokens
    std::cout << GREEN << "All unique tokens found across all DEXes:" << RESET << '\n'; // Header
    std::cout << YELLOW << "Total unique tokens: " << all_unique_tokens.size() << RESET << '\n'; // Token count
    
    for (const auto& token : all_unique_tokens) { // Loop through unique tokens
        std::cout << "Token: " << token << '\n'; // Print token address
    }
}

void show_all_dexes_by_blockchain() {
    // Define known DEXes for each blockchain
    std::map<std::string, std::vector<std::string>> blockchain_dexes = {
        {"Ethereum", {
            "Uniswap V2", "Uniswap V3", "SushiSwap", "Curve", "Balancer", 
            "1inch", "dYdX", "0x Protocol", "Kyber Network", "Bancor"
        }},
        {"Fantom", {
            "SpookySwap", "SpiritSwap", "Beethoven X", "TombSwap", "PaintSwap", 
            "SushiSwap", "Curve", "Solidly", "Tarot", "HyperJump"
        }},
        {"BSC", {
            "PancakeSwap", "Biswap", "ApeSwap", "DODO", "1inch", 
            "SushiSwap", "Curve", "Venus", "Alpaca Finance", "Ellipsis"
        }},
        {"Polygon", {
            "QuickSwap", "SushiSwap", "Curve", "Balancer", "1inch", 
            "dYdX", "0x Protocol", "Kyber Network", "Bancor", "Aave"
        }},
        {"Avalanche", {
            "TraderJoe", "Pangolin", "SushiSwap", "Curve", "1inch", 
            "dYdX", "0x Protocol", "Kyber Network", "Bancor", "Yield Yak"
        }},
        {"Solana", {
            "Raydium", "Orca", "Serum", "Saber", "Aldrin", 
            "Lifinity", "Crema", "Step", "Meteora", "Jupiter"
        }}
    };
    
    std::cout << GREEN << "Known DEXes by Blockchain:" << RESET << '\n';
    std::cout << std::string(60, '=') << '\n';
    
    for (const auto& [blockchain, dexes] : blockchain_dexes) {
        std::cout << YELLOW << blockchain << ":" << RESET << '\n';
        for (const auto& dex : dexes) {
            std::cout << "  • " << dex << '\n';
        }
        std::cout << '\n';
    }
}

void show_all_pools(const std::vector<RpcEndpoint>& rpc_endpoints, const std::string& blockchain) {
    // Load DEXes from config for the specified blockchain
    std::vector<DexInfo> dex_list = load_dexes_from_config(); // Get DEX list
    
    // Get RPC endpoint URL
    std::string rpc_url = "";
    if (!rpc_endpoints.empty()) {
        rpc_url = rpc_endpoints[0].url; // Use first RPC endpoint
    }
    
    std::cout << GREEN << "All pools found in " << blockchain << ":" << RESET << '\n';
    std::cout << std::string(60, '=') << '\n';
    
    bool found_any_pools = false;
    
    // Collect pools from all DEXes
    for (auto& dex : dex_list) { // Loop through all DEXes
        dex.pools.clear(); // Clear existing pools
        
        // Update pool count in real-time if it's 0
        if (dex.pool_count == 0) {
            FunctionStats stats;
            dex.pool_count = get_pool_count(rpc_url, dex.factory_address, 0, stats);
            std::cout << "DEBUG: Updated pool count for " << dex.name << " to " << dex.pool_count << std::endl;
        }
        
        for (uint64_t i = 0; i < dex.pool_count; ++i) { // Loop through pool indices
            FunctionStats stats; // Stats for this operation
            std::string addr = get_pool_address(rpc_url, dex.factory_address, i, 0, stats); // Get pool address
            if (!addr.empty()) { // Check if address is valid
                auto [token0, token1] = get_pool_tokens(rpc_url, addr, 0, stats); // Get tokens
                uint64_t liquidity = get_pool_liquidity(rpc_url, addr, 0, stats); // Get liquidity
                dex.pools.push_back({addr, token0, token1, liquidity}); // Add pool to DEX
            }
        }
        
        // Display pools for this DEX if any found
        if (!dex.pools.empty()) {
            found_any_pools = true;
            std::cout << YELLOW << "DEX: " << dex.name << " (" << dex.factory_address << ")" << RESET << '\n';
            for (const auto& pool : dex.pools) { // Loop through pools
                std::cout << "  Pool: " << pool.address << ", Token0: " << pool.token0 
                          << ", Token1: " << pool.token1 << ", Liquidity: " << pool.liquidity << '\n';
            }
            std::cout << '\n';
        }
    }
    
    if (!found_any_pools) {
        std::cout << YELLOW << "No pools found for " << blockchain << RESET << '\n';
    }
}
