//
//  dex_stats.cpp
//  NeoZorKDEXArb
//
//  Created by Rostyslav S. on 18.03.2025.
//
#include "dex_stats.h"
#include "rpc_core.h"       // For make_rpc_call
#include "measure.h"        // For update_stats
#include <curl/curl.h>      // For CURL requests
#include <sstream>          // For payload construction

void get_pool_swap_stats_thread(const std::string& rpc_url, const std::string& pool_address, uint64_t from_block, uint64_t to_block,
                                int request_limit, uint64_t& volume, uint64_t& tx_count, std::mutex& mtx, std::atomic<int>& progress, int total_pools) {
    // Start timing the function
    FunctionStats stats; // Local stats for this thread
    auto start = std::chrono::high_resolution_clock::now();

    // Prepare payload for eth_getLogs to fetch Swap events
    std::stringstream from_block_hex; // Stream for from block hex
    from_block_hex << "0x" << std::hex << from_block; // Convert from block to hex
    std::stringstream to_block_hex; // Stream for to block hex
    to_block_hex << "0x" << std::hex << to_block; // Convert to block to hex
    std::string payload = "{\"jsonrpc\":\"2.0\",\"method\":\"eth_getLogs\",\"params\":[{\"fromBlock\":\"" +
                          from_block_hex.str() + "\",\"toBlock\":\"" + to_block_hex.str() +
                          "\",\"address\":\"" + pool_address + "\",\"topics\":[\"0xd78ad95fa46c994b6551d0da85fc275fe613ce37657fb8d5e3d130840159d822\"]}],\"id\":1}";
    std::string result = make_rpc_call(rpc_url, payload, request_limit, stats); // Make RPC call

    // Parse logs to calculate volume and transaction count
    size_t pos = 0; // Position in the result string
    uint64_t local_volume = 0; // Local volume accumulator
    uint64_t local_tx_count = 0; // Local transaction counter
    while ((pos = result.find("\"data\":", pos)) != std::string::npos) { // Loop through log data
        pos += 8; // Move past "data": "
        size_t end = result.find('"', pos); // Find end of data field
        std::string data = result.substr(pos, end - pos); // Extract data
        if (data.size() >= 130) { // Check if data is long enough
            std::string amount0In = data.substr(2, 64); // Extract amount0In
            std::string amount1In = data.substr(66, 64); // Extract amount1In
            local_volume += std::stoull(amount0In, nullptr, 16) + std::stoull(amount1In, nullptr, 16); // Add to volume
            local_tx_count++; // Increment transaction count
        }
        pos = end; // Move to next log
    }

    // Update shared variables with mutex protection
    std::lock_guard<std::mutex> lock(mtx); // Lock mutex
    volume += local_volume; // Add local volume to shared volume
    tx_count += local_tx_count; // Add local tx count to shared count

    // Update progress bar
    progress++; // Increment progress
    print_progress_bar(progress, total_pools, "Fetching swap stats"); // Show progress

    // Finalize stats
    auto end = std::chrono::high_resolution_clock::now(); // End timing
    update_stats(stats, start, end, payload.size(), result.size()); // Update stats
}
