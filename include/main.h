//
//  main.h
//  NeoZorKDEXArb
//
//  Created by Rostyslav S. on 26.02.2025.
//
#ifndef MAIN_H
#define MAIN_H

// Standard C++ library includes
#include <string>           // For string handling
#include <vector>           // For dynamic arrays
#include <chrono>           // For timing and performance measurement
#include <mutex>            // For thread synchronization
#include <atomic>           // For thread-safe counters
#include <thread>           // For multi-threading support
#include <iostream>         // For console input/output
#include <fstream>          // For file input/output
#include <sstream>          // For string stream manipulation
#include <algorithm>        // For algorithms like transform
#include <iomanip>          // For formatted output (e.g., hex formatting)
#include <set>              // For unique collections (e.g., factory addresses)

// Internal library includes (13)
#include "arbitrage.h"
#include "blockchain.h"
#include "config_manager.h"
#include "dex_pools.h"
#include "dex_scanner.h"
#include "dex_stats.h"
#include "dex_tokens.h"
#include "input.h"
#include "measure.h"
#include "profit_analyzer.h"
#include "queries.h"
#include "rpc_core.h"
#include "wallet.h"

// External library includes
#include <curl/curl.h>      // For HTTP requests to RPC endpoints

// Platform-specific includes
#ifdef _WIN32
#include <windows.h>        // For Windows-specific file size and networking (Winsock)
#else
#include <sys/stat.h>       // For Unix-like file size measurement
#endif

// ANSI color codes for console output
const std::string GREEN = "\033[32m";    // Green color for success messages
const std::string YELLOW = "\033[33m";   // Yellow color for warnings or highlights
const std::string RED = "\033[31m";      // Red color for errors
const std::string BLUE = "\033[34m";     // Blue color for info
const std::string RESET = "\033[0m";     // Reset color to default
const std::string CYAN = "\033[36m";     // Cyan color for progress bars

// Structure for RPC endpoint details
struct RpcEndpoint {
    std::string url;       // URL of the RPC endpoint
    int request_limit;     // Maximum requests per second allowed by the endpoint
};

// Structure for pool information
struct PoolInfo {
    std::string address;   // Pool contract address
    std::string token0;    // Address of the first token in the pool
    std::string token1;    // Address of the second token in the pool
    uint64_t liquidity;    // Total liquidity in the pool
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
};

// Enum for supported blockchain types (moved from blockchain.h)
enum class BlockchainType {
    Ethereum,    // Ethereum blockchain
    Fantom,      // Fantom blockchain
    BSC,         // Binance Smart Chain
    Polygon,     // Polygon (Matic) blockchain
    Avalanche,   // Avalanche C-Chain
    Solana       // Solana blockchain
};

// Time units for Time Measure
enum ENUM_TIME_UNITS {NANOSECONDS, MICROSECONDS, MILLISECONDS, SECONDS};

// Function to find arbitrage opportunities
void findArbitrageOpportunity();

// Start Time Measure
void StartTimeMeasure();

// Stop Time Measure
void StopTimeMeasure(const ENUM_TIME_UNITS time_units);

#endif // MAIN_H
