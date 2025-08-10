//
//  dex_pools.cpp
//  NeoZorKDEXArb
//
//  Created by Rostyslav S. on 18.03.2025.
//

#include "dex/dex_pools.h"      // Include own header for declarations
#include "network/rpc_core.h"       // For make_rpc_call
#include "utils/measure.h"        // For update_stats
#include <iostream>         // For console output
#include <sstream>          // For stringstream in payload construction
#include <iomanip>          // For hex formatting

uint64_t get_pool_count(const std::string& rpc_url, const std::string& factory_address, int /* request_limit */, FunctionStats& stats) {
    // Start timing the function
    auto start = std::chrono::high_resolution_clock::now();

    // Prepare payload for eth_call to get allPairsLength
    std::string payload = "{\"jsonrpc\":\"2.0\",\"method\":\"eth_call\",\"params\":[{\"to\":\"" + factory_address +
                          "\",\"data\":\"0x90e18a69\"},\"latest\"],\"id\":1}";
    std::string result = make_rpc_call(rpc_url, payload, 0, stats); // Make RPC call

    // Parse the result
    if (!result.empty() && result != "0x0") { // Check if result is valid
        return std::stoull(result.substr(2), nullptr, 16); // Convert hex to integer and return
    }

    // Update stats and return 0 if no pools
    auto end = std::chrono::high_resolution_clock::now(); // End timing
    stats.execution_time_ms = std::chrono::duration_cast<std::chrono::microseconds>(end - start).count() / 1000.0; // Calculate execution time
    return 0; // Return 0 if call failed or no pools
}

std::string get_pool_address(const std::string& rpc_url, const std::string& factory_address, uint64_t index, int /* request_limit */, FunctionStats& stats) {
    // Start timing the function
    auto start = std::chrono::high_resolution_clock::now();

    // Prepare payload for eth_call to get allPairs(index)
    std::stringstream data; // Stream to build data field
    data << "0x1e83409a" << std::setfill('0') << std::setw(64) << std::hex << index; // Build call data
    std::string payload = "{\"jsonrpc\":\"2.0\",\"method\":\"eth_call\",\"params\":[{\"to\":\"" + factory_address +
                          "\",\"data\":\"" + data.str() + "\"},\"latest\"],\"id\":1}";
    std::string result = make_rpc_call(rpc_url, payload, 0, stats); // Make RPC call

    // Parse the result
    if (!result.empty() && result != "0x") { // Check if result is valid
        return "0x" + result.substr(26); // Extract and return pool address
    }

    // Update stats and return empty string if failed
    auto end = std::chrono::high_resolution_clock::now(); // End timing
    stats.execution_time_ms = std::chrono::duration_cast<std::chrono::microseconds>(end - start).count() / 1000.0; // Calculate execution time
    return ""; // Return empty string if call failed
}

uint64_t get_pool_liquidity(const std::string& rpc_url, const std::string& pool_address, int /* request_limit */, FunctionStats& stats) {
    // Start timing the function
    auto start = std::chrono::high_resolution_clock::now();

    // Prepare payload for eth_call to get reserves
    std::string payload = "{\"jsonrpc\":\"2.0\",\"method\":\"eth_call\",\"params\":[{\"to\":\"" + pool_address +
                          "\",\"data\":\"0x1a686502\"},\"latest\"],\"id\":1}";
    std::string result = make_rpc_call(rpc_url, payload, 0, stats); // Make RPC call

    // Parse the result
    if (!result.empty() && result != "0x0") { // Check if result is valid
        return std::stoull(result.substr(2), nullptr, 16); // Convert hex to integer and return
    }

    // Update stats and return 0 if failed
    auto end = std::chrono::high_resolution_clock::now(); // End timing
    stats.execution_time_ms = std::chrono::duration_cast<std::chrono::microseconds>(end - start).count() / 1000.0; // Calculate execution time
    return 0; // Return 0 if call failed
}
