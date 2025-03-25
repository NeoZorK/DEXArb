//
//  main.h
//  NeoZorKDEXArb
//
//  Created by Rostyslav S. on 26.02.2025.
//
#ifndef MAIN_H
#define MAIN_H

#include <string>           // For string handling in structures
#include <vector>           // For dynamic arrays in structures
#include <chrono>           // For timing in FunctionStats
#include "json.hpp"         // nlohman json parsing
#include "httplib.h"        // HTTP library for making HTTP requests

// ANSI color codes for console output
const std::string GREEN = "\033[32m";    // Green color for success messages
const std::string YELLOW = "\033[33m";   // Yellow color for warnings or highlights
const std::string RED = "\033[31m";      // Red color for errors
const std::string BLUE = "\033[34m";     // Blue color for info
const std::string RESET = "\033[0m";     // Reset color to default
const std::string CYAN = "\033[36m";     // Cyan color for progress bars
const std::string WHITE = "\033[37m";   // White color for emphasis

// Structure for RPC endpoint details
struct struct_rpc_endpoint {
    std::string url;       // URL of the RPC endpoint
    int request_limit;     // Maximum requests per second allowed by the endpoint
    bool active;           // Active or Not, by default false (when created config)
};

// Structure for pool information
struct struct_pool_info {
    std::string address;   // Pool contract address
    std::string token0;    // Address of the first token in the pool
    std::string token1;    // Address of the second token in the pool
    uint64_t liquidity;    // Total liquidity in the pool
};

// Structure for DEX details
struct struct_dex_info {
    std::string name;             // Name of the DEX (e.g., Uniswap)
    std::string factory_address;  // Factory contract address of the DEX
    uint64_t pool_count = 0;      // Number of pools managed by the factory
    std::vector<struct_pool_info> pools;  // List of pools associated with the DEX
    uint64_t liquidity = 0;       // Total liquidity across all pools
    uint64_t tvl = 0;             // Total Value Locked in the DEX
    uint64_t volume_24h = 0;      // 24-hour trading volume
    uint64_t tx_count_24h = 0;    // Number of transactions in the last 24 hours
};

// Structure for performance statistics
struct struct_function_stats {
    double execution_time_ms = 0.0;   // Execution time of a function in milliseconds
    size_t virtual_memory_kb = 0;     // Virtual memory used in kilobytes
    size_t disk_usage_bytes = 0;      // Disk usage in bytes
    double latency_ms = 0.0;          // Network latency in milliseconds
    double cpu_usage_percent = 0.0;   // CPU usage percentage
    size_t outbound_traffic = 0;      // Outbound traffic in bytes
    size_t inbound_traffic = 0;       // Inbound traffic in bytes
};

// Time units for Time Measure
enum ENUM_TIME_UNITS {NANOSECONDS, MICROSECONDS, MILLISECONDS, SECONDS};

// Function to find arbitrage opportunities
void find_arbitrage_opportunity();

// Start Time Measure
void start_time_measure();

// Stop Time Measure
void stop_time_measure(const ENUM_TIME_UNITS time_units);

#endif // MAIN_H
