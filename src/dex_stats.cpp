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

// Thread function to fetch Swap events and calculate volume and transaction count
void get_pool_swap_stats_thread(const std::string& rpc_url,
                                const std::string& pool_address,
                                uint64_t from_block,
                                uint64_t to_block,
                                int request_limit,
                                uint64_t& volume,
                                uint64_t& tx_count,
                                std::mutex& mtx,
                                std::atomic<int>& progress,
                                int total_pools) {
    
 // Local stats for this thread
    struct_function_stats stats;
    
    // Start timing the function
    auto start = std::chrono::high_resolution_clock::now();

    // Prepare payload for eth_getLogs to fetch Swap events (Stream for from block hex)
    std::stringstream from_block_hex;
    
    // Convert from block to hex
    from_block_hex << "0x" << std::hex << from_block;
    
    // Stream for to block hex
    std::stringstream to_block_hex;
    
    // Convert to block to hex
    to_block_hex << "0x" << std::hex << to_block;
    
    // Construct payload
    std::string payload = "{\"jsonrpc\":\"2.0\",\"method\":\"eth_getLogs\",\"params\":[{\"fromBlock\":\"" +
                          from_block_hex.str() + "\",\"toBlock\":\"" + to_block_hex.str() +
                          "\",\"address\":\"" + pool_address + "\",\"topics\":[\"0xd78ad95fa46c994b6551d0da85fc275fe613ce37657fb8d5e3d130840159d822\"]}],\"id\":1}";
    
    // Make RPC call
    std::string result = make_rpc_call(rpc_url, payload, request_limit, stats);

    // Parse logs to calculate volume and transaction count
    // Position in the result string
    size_t pos = 0;
    
    // Local volume accumulator
    uint64_t local_volume = 0;
    
    // Local transaction counter
    uint64_t local_tx_count = 0;
    
    // Loop through log data
    while ((pos = result.find("\"data\":", pos)) != std::string::npos) {
        
        // Move past "data": "
        pos += 8;
        
        // Find end of data field
        size_t end = result.find('"', pos);
        
        // Extract data
        std::string data = result.substr(pos, end - pos);
        
        // Check if data is long enough
        if (data.size() >= 130) {
            
            // Extract amount0In
            std::string amount0In = data.substr(2, 64);
            
            // Extract amount1In
            std::string amount1In = data.substr(66, 64);
            
            // Add to volume
            local_volume += std::stoull(amount0In, nullptr, 16) + std::stoull(amount1In, nullptr, 16);
            
            // Increment transaction count
            local_tx_count++;
        }
        // Move to next log
        pos = end;
    }

    // Update shared variables with mutex protection
    // Lock mutex
    std::lock_guard<std::mutex> lock(mtx);
    
    // Add local volume to shared volume
    volume += local_volume;
    
    // Add local tx count to shared count
    tx_count += local_tx_count;

    // Update progress bar
    progress++;
    
    // Show progress
    print_progress_bar(progress, total_pools, "Fetching swap stats");

    // Finalize stats
    auto end = std::chrono::high_resolution_clock::now();
    
    // Update stats
    update_stats(stats, start, end, payload.size(), result.size());
}
