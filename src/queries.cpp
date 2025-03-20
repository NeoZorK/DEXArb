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

// Load DEXes from config
void show_pools(const std::vector<struct_rpc_endpoint>& rpc_endpoints, const std::string& dex_identifier) {
    
    // Get DEX list
    std::vector<struct_dex_info> dex_list = load_dexes_from_config();
    
    // Find DEX
    auto it = std::find_if(dex_list.begin(),
                           dex_list.end(),
                           [&dex_identifier]
                           (const struct_dex_info& dex)
                           { return dex.name == dex_identifier
                            || dex.factory_address == dex_identifier;
                            }); // Find DEX
    
    // Check if DEX was found
    if (it == dex_list.end()) {
        std::cerr << RED << "DEX not found" << RESET << '\n';
        return;
    }
    
    // Reference to found DEX
    struct_dex_info& dex = *it;

    // Fetch fresh pool data
    // Clear existing pools
    dex.pools.clear();
    
    // Loop through pool indices
    for (uint64_t i = 0; i < dex.pool_count; ++i) {
        
        // Stats for this operation
        struct_function_stats stats;
        
        // Get pool address
        std::string addr = get_pool_address(rpc_endpoints[0].url, dex.factory_address, i, rpc_endpoints[0].request_limit, stats);
        
        // Check if address is valid
        if (!addr.empty()) {
            
            // Get tokens
            auto [token0, token1] = get_pool_tokens(rpc_endpoints[0].url, addr, rpc_endpoints[0].request_limit, stats);
            
            // Get liquidity
            uint64_t liquidity = get_pool_liquidity(rpc_endpoints[0].url, addr, rpc_endpoints[0].request_limit, stats);
            
            // Add pool to DEX
            dex.pools.push_back({addr, token0, token1, liquidity});
        }
    }

    // Display pool information ( Header )
    std::cout << GREEN << "Pools for " << dex.name << " (" << dex.factory_address << "):" << RESET << '\n';
    for (const auto& pool : dex.pools) {
        
        // Loop through pools
        std::cout << YELLOW << "Pool: " << pool.address << ", Token0: " << pool.token0 << ", Token1: " << pool.token1
                  << ", Liquidity: " << pool.liquidity << RESET << '\n';
    }
}

// Load DEXes from config
void show_tokens(const std::vector<struct_rpc_endpoint>& rpc_endpoints, const std::string& dex_identifier) {
    
    // Get DEX list
    std::vector<struct_dex_info> dex_list = load_dexes_from_config();
    
    // Find DEX
    auto it = std::find_if(dex_list.begin(), dex_list.end(),
                           [&dex_identifier](const struct_dex_info& dex) { return dex.name == dex_identifier || dex.factory_address == dex_identifier; });
    
    // Check if DEX was found
    if (it == dex_list.end()) {
        std::cerr << RED << "DEX not found" << RESET << '\n';
        return;
    }
    
    // Reference to found DEX
    struct_dex_info& dex = *it;

    // Fetch tokens and ensure uniqueness
    // List of unique tokens
    std::vector<std::string> unique_tokens;
    
    // Clear existing pools
    dex.pools.clear();
    
    // Loop through pool indices
    for (uint64_t i = 0; i < dex.pool_count; ++i) {
       
        // Stats for this operation
        struct_function_stats stats;
        
        // Get pool address
        std::string addr = get_pool_address(rpc_endpoints[0].url, dex.factory_address, i, rpc_endpoints[0].request_limit, stats);
       
        // Check if address is valid
        if (!addr.empty()) {
            
            // Get tokens
            auto [token0, token1] = get_pool_tokens(rpc_endpoints[0].url, addr, rpc_endpoints[0].request_limit, stats);
            
            // Add token0 if unique
            if (std::find(unique_tokens.begin(), unique_tokens.end(), token0) == unique_tokens.end()) unique_tokens.push_back(token0);
            
            // Add token1 if unique
            if (std::find(unique_tokens.begin(), unique_tokens.end(), token1) == unique_tokens.end()) unique_tokens.push_back(token1);
        }
    }

    // Display token information
    std::cout << GREEN << "Tokens for " << dex.name << " (" << dex.factory_address << "):" << RESET << '\n';
    
    // Loop through tokens
    for (const auto& token : unique_tokens) {
        
        // Print token address
        std::cout << YELLOW << "Token: " << token << RESET << '\n';
    }
}

// Load DEXes from config
void find_token_in_dex(const std::vector<struct_rpc_endpoint>& rpc_endpoints, const std::string& dex_identifier, const std::string& token_address) {
    
    // Get DEX list
    std::vector<struct_dex_info> dex_list = load_dexes_from_config();
    
    // Find DEX
    auto it = std::find_if(dex_list.begin(), dex_list.end(),
                           [&dex_identifier](const struct_dex_info& dex) { return dex.name == dex_identifier || dex.factory_address == dex_identifier; });
    
    // Check if DEX was found
    if (it == dex_list.end()) {
        std::cerr << RED << "DEX not found" << RESET << '\n';
        return;
    }
    
    // Reference to found DEX
    struct_dex_info& dex = *it;

    // Search for token in pools
    // Clear existing pools
    dex.pools.clear();
    
    // Loop through pool indices
    for (uint64_t i = 0; i < dex.pool_count; ++i) {
        
        // Stats for this operation
        struct_function_stats stats;
        
        // Get pool address
        std::string addr = get_pool_address(rpc_endpoints[0].url, dex.factory_address, i, rpc_endpoints[0].request_limit, stats);
        
        // Check if address is valid
        if (!addr.empty()) {
            
            // Get tokens
            auto [token0, token1] = get_pool_tokens(rpc_endpoints[0].url, addr, rpc_endpoints[0].request_limit, stats);
            
            // Get liquidity
            uint64_t liquidity = get_pool_liquidity(rpc_endpoints[0].url, addr, rpc_endpoints[0].request_limit, stats);
            
            // Check if token matches
            if (token0 == token_address || token1 == token_address) {
                
                // Add matching pool
                dex.pools.push_back({addr, token0, token1, liquidity});
            }
        }
    }

    // Display results
    std::cout << GREEN << "Token " << token_address << " in " << dex.name << ":" << RESET << '\n';
    
    // Loop through matching pools
    for (const auto& pool : dex.pools) {
        std::cout << YELLOW << "Pool: " << pool.address << ", Token0: " << pool.token0 << ", Token1: " << pool.token1
                  << ", Liquidity: " << pool.liquidity << RESET << '\n';
    }
}

// Load DEXes from config
void find_tokens_across_dexes(const std::vector<struct_rpc_endpoint>& rpc_endpoints, const std::string& token_address) {
   
    // Get DEX list
    std::vector<struct_dex_info> dex_list = load_dexes_from_config();
    
    // Loop through all DEXes
    for (auto& dex : dex_list) {
        
        // Clear existing pools
        dex.pools.clear();
        
        // Loop through pool indices
        for (uint64_t i = 0; i < dex.pool_count; ++i) {
            
            // Stats for this operation
            struct_function_stats stats;
            
            // Get pool address
            std::string addr = get_pool_address(rpc_endpoints[0].url, dex.factory_address, i, rpc_endpoints[0].request_limit, stats);
            
            // Check if address is valid
            if (!addr.empty()) {
                
                // Get tokens
                auto [token0, token1] = get_pool_tokens(rpc_endpoints[0].url, addr, rpc_endpoints[0].request_limit, stats);
                
                // Get liquidity
                uint64_t liquidity = get_pool_liquidity(rpc_endpoints[0].url, addr, rpc_endpoints[0].request_limit, stats);
                
                // Check if token matches
                if (token0 == token_address || token1 == token_address) {
                    
                    // Add matching pool
                    dex.pools.push_back({addr, token0, token1, liquidity});
                }
            }
        }
    }

    // Display results
    std::cout << GREEN << "Pools with " << token_address << ":" << RESET << '\n';
    
    // Loop through DEXes
    for (const auto& dex : dex_list) {
        
        // Check if DEX has matching pools
        if (!dex.pools.empty()) {
            std::cout << YELLOW << "DEX: " << dex.name << " (" << dex.factory_address << ")" << RESET << '\n';
            
            // Loop through pools
            for (const auto& pool : dex.pools) {
                std::cout << "  Pool: " << pool.address << ", Token0: " << pool.token0 << ", Token1: " << pool.token1
                          << ", Liquidity: " << pool.liquidity << '\n';
            }
        }
    }
}

// Load and display all DEXes
void show_dexes(const std::vector<struct_rpc_endpoint>& rpc_endpoints) {
   
    // Get DEX list
    std::vector<struct_dex_info> dex_list = load_dexes_from_config();
    
    // Header
    std::cout << GREEN << "Found DEXes:" << RESET << '\n';
    
    // Loop through DEXes
    for (const auto& dex : dex_list) {
        std::cout << YELLOW << "Name: " << dex.name << ", Address: " << dex.factory_address << ", Pools: " << dex.pool_count << RESET << '\n';
    }
}
