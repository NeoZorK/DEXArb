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

    // First, check if this is a valid contract
    std::string code_payload = "{\"jsonrpc\":\"2.0\",\"method\":\"eth_getCode\",\"params\":[\"" + factory_address + "\",\"latest\"],\"id\":1}";
    std::string code_response = make_rpc_call(rpc_url, code_payload, request_limit, stats);
    
    if (code_response.find("\"result\":\"0x\"") != std::string::npos || 
        code_response.find("\"result\":\"0x0\"") != std::string::npos) {
        // Not a valid contract
        auto end = std::chrono::high_resolution_clock::now();
        update_stats(stats, start, end, code_payload.size(), code_response.size());
        return 0;
    }

    // DEX-specific method signatures based on known addresses
    std::vector<std::pair<std::string, std::string>> pool_count_methods;
    
    // Check if this is a known DEX and add specific methods
    if (factory_address == "0x5C69bEe701ef814a2B6a3EDD4B1652CB9cc5aA6f" || // Uniswap V2
        factory_address == "0xC0AEe478e3658e2610c5F7A4A2E1777cE9e4f2Ac" || // SushiSwap
        factory_address == "0x152eE697f2E276fA89E96742e9bB9aB51FcFcA15" || // SpookySwap
        factory_address == "0xEF45d134b73241eDa7703fa787148D9C9F4950b0" || // SpiritSwap
        factory_address == "0xcA143Ce32Fe78f1f7019d7d551a6402fC5350c73" || // PancakeSwap V2
        factory_address == "0x5757371414417b8C6CAad45bAeF941aBc7d3Ab32" || // QuickSwap
        factory_address == "0x9Ad6C38BE94206cA50bb0d90783181662f0Cfa10" || // TraderJoe V2
        factory_address == "0xefa94DE7a4656D787667C749f7E1223D71E9FD88") {  // Pangolin
        // Uniswap V2 style DEXes - these use allPairsLength()
        pool_count_methods = {
            {"0x90e18a69", ""}, // allPairsLength() - no parameters
            {"0x783cca1c", ""}, // poolCount() - no parameters
            {"0xb4d9b203", ""}, // getPoolCount() - no parameters
            {"0x112c2569", ""}, // poolLength() - no parameters
            {"0x4d2301cc", ""}, // getPairCount() - no parameters
            {"0x8a19c8bc", ""}  // poolCount() - alternative
        };
    } else if (factory_address == "0x1F98431c8aD98523631AE4a59f267346ea31F984") { // Uniswap V3
        // Uniswap V3 style DEXes - these use poolCount()
        pool_count_methods = {
            {"0x783cca1c", ""}, // poolCount() - no parameters
            {"0xb4d9b203", ""}, // getPoolCount() - no parameters
            {"0x112c2569", ""}, // poolLength() - no parameters
            {"0x90e18a69", ""}, // allPairsLength() (fallback)
            {"0x4d2301cc", ""}, // getPairCount() - no parameters
            {"0x8a19c8bc", ""}  // poolCount() - alternative
        };
    } else if (factory_address == "0xBA12222222228d8Ba445958a75a0704d566BF2C8") { // Balancer V2
        // Balancer V2 style DEXes - these use getPoolCount()
        pool_count_methods = {
            {"0xb4d9b203", ""}, // getPoolCount() - no parameters
            {"0x783cca1c", ""}, // poolCount() - no parameters
            {"0x112c2569", ""}, // poolLength() - no parameters
            {"0x90e18a69", ""}, // allPairsLength() (fallback)
            {"0x4d2301cc", ""}, // getPairCount() - no parameters
            {"0x8a19c8bc", ""}  // poolCount() - alternative
        };
    } else if (factory_address == "0x9DEB29c9a4c7A88a3C0257393b7f3335338D9A9D") { // Beethoven X
        // Beethoven X style DEXes - these use poolCount()
        pool_count_methods = {
            {"0x783cca1c", ""}, // poolCount() - no parameters
            {"0xb4d9b203", ""}, // getPoolCount() - no parameters
            {"0x112c2569", ""}, // poolLength() - no parameters
            {"0x90e18a69", ""}, // allPairsLength() (fallback)
            {"0x4d2301cc", ""}, // getPairCount() - no parameters
            {"0x8a19c8bc", ""}  // poolCount() - alternative
        };
    } else {
        // Generic methods for unknown DEXes
        pool_count_methods = {
            {"0x90e18a69", ""}, // allPairsLength() - Uniswap V2, SushiSwap, PancakeSwap V2
            {"0x783cca1c", ""}, // poolCount() - Some DEXes use this
            {"0xb4d9b203", ""}, // getPoolCount() - Another alternative
            {"0x112c2569", ""}, // poolLength() - Yet another alternative
            {"0x4d2301cc", ""}, // getPairCount() - no parameters
            {"0x8a19c8bc", ""}  // poolCount() - alternative
        };
    }

    // Try each method until one works
    for (const auto& method_pair : pool_count_methods) {
        std::string method = method_pair.first;
        std::string params = method_pair.second;
        
        std::string payload = "{\"jsonrpc\":\"2.0\",\"method\":\"eth_call\",\"params\":[{\"to\":\"" + factory_address + "\",\"data\":\"" + method + params + "\"}, \"latest\"],\"id\":1}";
        
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
