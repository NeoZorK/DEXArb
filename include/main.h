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
#include <memory>           // For smart pointers
#include <functional>       // For function objects

// Supported blockchain types as an enum
enum class BlockchainType {
    Ethereum,    // Ethereum blockchain
    Fantom,      // Fantom blockchain
    BSC,         // Binance Smart Chain
    Polygon,     // Polygon (Matic) blockchain
    Avalanche,   // Avalanche C-Chain
    Solana       // Solana blockchain
};

// ANSI color codes for console output
const std::string GREEN = "\033[32m";    // Green color for success messages
const std::string YELLOW = "\033[33m";   // Yellow color for warnings or highlights
const std::string RED = "\033[31m";      // Red color for errors
const std::string BLUE = "\033[34m";     // Blue color for info
const std::string RESET = "\033[0m";     // Reset color to default
const std::string CYAN = "\033[36m";     // Cyan color for progress bars

// Structure for RPC endpoint details
struct RpcEndpoint {
    int request_limit;     // Maximum requests per second allowed by the endpoint
    std::string url;       // URL of the RPC endpoint
    
    // Modern constructor with validation
    RpcEndpoint(const std::string& endpoint_url, int limit) 
        : request_limit(limit), url(endpoint_url) {
        if (request_limit <= 0) request_limit = 10; // Default limit
    }
    
    // Validation method
    bool is_valid() const {
        return !url.empty() && request_limit > 0;
    }
};

// Structure for pool information
struct PoolInfo {
    std::string address;   // Pool contract address
    std::string token0;    // Address of the first token in the pool
    std::string token1;    // Address of the second token in the pool
    uint64_t liquidity;    // Total liquidity in the pool
    
    // Modern constructor
    PoolInfo(const std::string& pool_address = "", 
             const std::string& t0 = "", 
             const std::string& t1 = "", 
             uint64_t liq = 0)
        : address(pool_address), token0(t0), token1(t1), liquidity(liq) {}
    
    // Validation method
    bool is_valid() const {
        return !address.empty() && !token0.empty() && !token1.empty();
    }
};

// Structure for DEX details
struct DexInfo {
    std::string name;             // Name of the DEX (e.g., Uniswap)
    std::string factory_address;  // Factory contract address of the DEX
    uint64_t pool_count = 0;      // Number of pools managed by the factory
    std::vector<PoolInfo> pools;  // List of pools associated with the DEX
    uint64_t liquidity = 0;       // Total liquidity across all pools
    uint64_t tvl = 0;             // Total Value Locked in the DEX
    uint64_t volume_24h = 0;      // 24-hour trading volume
    uint64_t tx_count_24h = 0;    // Number of transactions in the last 24 hours
    
    // Modern constructor
    DexInfo(const std::string& dex_name = "", const std::string& factory = "")
        : name(dex_name), factory_address(factory) {}
    
    // Validation method
    bool is_valid() const {
        return !name.empty() && !factory_address.empty();
    }
    
    // Method to add pool
    void add_pool(const PoolInfo& pool) {
        if (pool.is_valid()) {
            pools.push_back(pool);
            pool_count = pools.size();
        }
    }
};

// Structure for performance statistics
struct FunctionStats {
    double execution_time_ms = 0.0;   // Execution time of a function in milliseconds
    size_t virtual_memory_kb = 0;     // Virtual memory used in kilobytes
    size_t disk_usage_bytes = 0;      // Disk usage in bytes
    double latency_ms = 0.0;          // Network latency in milliseconds
    double cpu_usage_percent = 0.0;   // CPU usage percentage
    size_t outbound_traffic = 0;      // Outbound traffic in bytes
    size_t inbound_traffic = 0;       // Inbound traffic in bytes
    
    // Method to reset stats
    void reset() {
        execution_time_ms = 0.0;
        virtual_memory_kb = 0;
        disk_usage_bytes = 0;
        latency_ms = 0.0;
        cpu_usage_percent = 0.0;
        outbound_traffic = 0;
        inbound_traffic = 0;
    }
    
    // Method to add another stats object
    void add(const FunctionStats& other) {
        execution_time_ms += other.execution_time_ms;
        virtual_memory_kb += other.virtual_memory_kb;
        disk_usage_bytes += other.disk_usage_bytes;
        latency_ms += other.latency_ms;
        cpu_usage_percent += other.cpu_usage_percent;
        outbound_traffic += other.outbound_traffic;
        inbound_traffic += other.inbound_traffic;
    }
};

// Time units for Time Measure
enum ENUM_TIME_UNITS {NANOSECONDS, MICROSECONDS, MILLISECONDS, SECONDS};

// Function to find arbitrage opportunities
void findArbitrageOpportunity();

// Start Time Measure
void StartTimeMeasure();

// Stop Time Measure
void StopTimeMeasure(const ENUM_TIME_UNITS time_units);

// Function to display help information
void show_help();

// Function to display version information
void show_version();

#endif // MAIN_H
