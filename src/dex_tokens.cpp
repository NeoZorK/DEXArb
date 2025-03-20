//
//  dex_tokens.cpp
//  NeoZorKDEXArb
//
//  Created by Rostyslav S. on 18.03.2025.
//
#include "dex_tokens.h"
#include "rpc_core.h"       // For make_rpc_call
#include "measure.h"        // For update_stats
#include <iostream>         // For console output

// Function to get pool tokens
std::pair<std::string, std::string> get_pool_tokens(const std::string& rpc_url,
                                                    const std::string& pool_address,
                                                    int request_limit,
                                                    struct_function_stats& stats) {
    // Start timing the function
    auto start = std::chrono::high_resolution_clock::now();
    
    // Variables to store token addresses
    std::string token0, token1;

    // Fetch token0 address (Payload for token0 call)
    std::string payload0 = "{\"jsonrpc\":\"2.0\",\"method\":\"eth_call\",\"params\":[{\"to\":\"" + pool_address +
                           "\",\"data\":\"0x0dfe1681\"},\"latest\"],\"id\":1}";
    
    // Make RPC call
    std::string result0 = make_rpc_call(rpc_url, payload0, request_limit, stats);
    
    // Check if result is valid
    if (!result0.empty()) {
        
        // Extract token0 address
        token0 = "0x" + result0.substr(26);
    }

    // Fetch token1 address (Payload for token1 call)
    std::string payload1 = "{\"jsonrpc\":\"2.0\",\"method\":\"eth_call\",\"params\":[{\"to\":\"" + pool_address +
                           "\",\"data\":\"0xd21220a7\"},\"latest\"],\"id\":1}";
    
    // Make RPC call
    std::string result1 = make_rpc_call(rpc_url, payload1, request_limit, stats);
    
    // Check if result is valid
    if (!result1.empty()) {
        
        // Extract token1 address
        token1 = "0x" + result1.substr(26);
    }

    // Update stats and return token pair
    auto end = std::chrono::high_resolution_clock::now();
    
    // Update stats
    update_stats(stats, start, end, payload0.size() + payload1.size(), result0.size() + result1.size());
    
    // Return token pair
    return {token0, token1};
}
