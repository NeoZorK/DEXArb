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

std::pair<std::string, std::string> get_pool_tokens(const std::string& rpc_url, const std::string& pool_address, int request_limit, FunctionStats& stats) {
    // Start timing the function
    auto start = std::chrono::high_resolution_clock::now();
    std::string token0, token1; // Variables to store token addresses

    // Fetch token0 address
    std::string payload0 = "{\"jsonrpc\":\"2.0\",\"method\":\"eth_call\",\"params\":[{\"to\":\"" + pool_address +
                           "\",\"data\":\"0x0dfe1681\"},\"latest\"],\"id\":1}"; // Payload for token0 call
    std::string result0 = make_rpc_call(rpc_url, payload0, request_limit, stats); // Make RPC call
    if (!result0.empty()) { // Check if result is valid
        token0 = "0x" + result0.substr(26); // Extract token0 address
    }

    // Fetch token1 address
    std::string payload1 = "{\"jsonrpc\":\"2.0\",\"method\":\"eth_call\",\"params\":[{\"to\":\"" + pool_address +
                           "\",\"data\":\"0xd21220a7\"},\"latest\"],\"id\":1}"; // Payload for token1 call
    std::string result1 = make_rpc_call(rpc_url, payload1, request_limit, stats); // Make RPC call
    if (!result1.empty()) { // Check if result is valid
        token1 = "0x" + result1.substr(26); // Extract token1 address
    }

    // Update stats and return token pair
    auto end = std::chrono::high_resolution_clock::now(); // End timing
    update_stats(stats, start, end, payload0.size() + payload1.size(), result0.size() + result1.size()); // Update stats
    return {token0, token1}; // Return token pair
}
