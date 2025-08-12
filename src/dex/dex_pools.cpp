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
#include <vector>           // For std::vector

uint64_t get_pool_count(const std::string& rpc_url, const std::string& factory_address, int request_limit, FunctionStats& stats) {
    // Start timing the function
    auto start = std::chrono::high_resolution_clock::now();

    // Different method signatures for different DEX types
    std::vector<std::string> pool_count_methods = {
        "0x90e18a69", // allPairsLength() - Uniswap V2, SushiSwap, PancakeSwap V2
        "0x783cca1c", // poolCount() - Some DEXes use this
        "0xb4d9b203", // getPoolCount() - Another alternative
        "0x112c2569", // poolLength() - Yet another alternative
        "0x1e83409a"  // allPairs(uint256) - Alternative method (but needs parameter)
    };

    // Try each method until one works
    for (const auto& method : pool_count_methods) {
        std::string payload = "{\"jsonrpc\":\"2.0\",\"method\":\"eth_call\",\"params\":[{\"to\":\"" + factory_address + "\",\"data\":\"" + method + "\"}, \"latest\"],\"id\":1}";
        
        std::string response = make_rpc_call(rpc_url, payload, request_limit, stats);
        
        // Check if response contains error
        if (response.find("\"error\"") != std::string::npos) {
            continue; // Try next method
        }
        
        // Extract result from response
        size_t result_start = response.find("\"result\":\"");
        if (result_start != std::string::npos) {
            result_start += 10; // Skip "result":"
            size_t result_end = response.find("\"", result_start);
            if (result_end != std::string::npos) {
                std::string result = response.substr(result_start, result_end - result_start);
                
                // Check if result is not empty and not "0x"
                if (!result.empty() && result != "0x") {
                    // Convert hex to decimal
                    try {
                        uint64_t pool_count = std::stoull(result.substr(2), nullptr, 16); // Skip "0x" prefix
                        
                        // Update stats
                        auto end = std::chrono::high_resolution_clock::now();
                        update_stats(stats, start, end, payload.size(), response.size());
                        
                        return pool_count;
                    } catch (const std::exception&) {
                        // Invalid hex, try next method
                        continue;
                    }
                }
            }
        }
    }
    
    // Update stats
    auto end = std::chrono::high_resolution_clock::now();
    update_stats(stats, start, end, 0, 0);
    
    return 0; // No pools found
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
    if (!result.empty() && result != "0x0" && result != "0x") { // Check if result is valid
        try {
            return std::stoull(result.substr(2), nullptr, 16); // Convert hex to integer and return
        } catch (const std::exception& e) {
            std::cout << "DEBUG: Failed to convert result '" << result << "' to number: " << e.what() << std::endl;
            return 0; // Return 0 if conversion fails
        }
    }

    // Update stats and return 0 if failed
    auto end = std::chrono::high_resolution_clock::now(); // End timing
    stats.execution_time_ms = std::chrono::duration_cast<std::chrono::microseconds>(end - start).count() / 1000.0; // Calculate execution time
    return 0; // Return 0 if call failed
}
