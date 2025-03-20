//
//  config_manager.cpp
//  NeoZorKDEXArb
//
//  Created by Rostyslav S. on 18.03.2025.
//
#include "config_manager.h" // Include own header
#include "rpc_core.h"       // For get_latest_block_number
#include "dex_pools.h"      // For pool functions
#include "dex_tokens.h"     // For token functions
#include "dex_stats.h"      // For swap stats
#include "measure.h"        // For update_stats
#include <iostream>         // For console output
#include <fstream>          // For file I/O
#include <sstream>          // For string manipulation
#include <thread>           // For multi-threading
#ifdef _WIN32
#include <windows.h>        // For Windows file size
#else
#include <sys/stat.h>       // For Unix file size
#endif

// Function to load DEXes from config
std::vector<struct_dex_info> load_dexes_from_config() {
    
    // Read config file
    std::vector<struct_dex_info> dex_list;
    
    // Open config file
    std::ifstream config_file("neozork-config");
    
    // Return empty list if file doesn't exist
    if (!config_file.is_open()) return dex_list;

    // Buffer for file content
    std::stringstream buffer;
    
    // Read file into buffer
    buffer << config_file.rdbuf();
    
    // Convert to string
    std::string content = buffer.str();
    
    // Close file
    config_file.close();

    // Loop through each blockchain
    for (const auto& chain : {"ethereum", "fantom", "bsc", "polygon", "avalanche", "solana"}) {
        
        // Find blockchain section
        size_t chain_pos = content.find("\"" + std::string(chain) + "\": {");
        
        // Skip if not found
        if (chain_pos == std::string::npos) continue;

        // Find DEX array
        size_t dex_pos = content.find("\"dex\": [", chain_pos);
        
        // Check if DEX section exists
        if (dex_pos != std::string::npos) {
            
            // Find end of DEX array
            size_t dex_end = content.find("]", dex_pos);
            
            // Move past "dex": [
            size_t pos = dex_pos + 8;
            
            // Loop through DEX entries
            while (pos < dex_end) {
                
                // Find factory address field
                size_t addr_start = content.find("\"factory_address\": \"", pos);
                
                // Break if not found
                if (addr_start == std::string::npos || addr_start > dex_end) break;
                
                // Move past "factory_address": "
                addr_start += 19;
                
                // Find end of address
                size_t addr_end = content.find('"', addr_start);
                
                // Extract address
                std::string factory_address = content.substr(addr_start, addr_end - addr_start);

                // Fix: create object DexInfo и set only needed fields
                struct_dex_info dex;
                
                // Set name
                dex.name = "Unknown_" + factory_address.substr(2, 6);
                
                // Set factory address
                dex.factory_address = factory_address;
                
                // Add to list
                dex_list.push_back(dex);

                // Move to next entry
                pos = content.find("{", pos + 1);
                
                // Exit if no more entries
                if (pos == std::string::npos) break;
            }
        }
    }
    
    // Return list of DEXes
    return dex_list;
}

// Function to update config with DEXes
void update_config_with_dex(const std::vector<struct_rpc_endpoint>& rpc_endpoints,
                            std::vector<struct_dex_info>& dex_list,
                            struct_function_stats& stats) {
   
    // Start timing the function
    auto start = std::chrono::high_resolution_clock::now();

    // Load existing config content
    // Open config file
    std::ifstream in_file("neozork-config");
    
    // Buffer for file content
    std::stringstream buffer;
    
    // Read file into buffer
    buffer << in_file.rdbuf();
    
    // Convert to string
    std::string content = buffer.str();
    
    // Close the file
    in_file.close();

    // Fetch latest block number for 24-hour range
    // Stats for block fetch
    struct_function_stats block_stats;
    
    // Get latest block
    std::string latest_block = get_latest_block_number(rpc_endpoints[0].url, rpc_endpoints[0].request_limit, block_stats);
    
    // Convert to integer
    uint64_t latest_block_num = std::stoull(latest_block.substr(2), nullptr, 16);
    
    // Calculate block 24 hours ago (assuming 2s block time)
    uint64_t from_block = latest_block_num - (24 * 60 * 60 / 2);

    // Update each DEX with fresh data
    // Loop through DEXes
    for (auto& dex : dex_list) {
        
        // Update pool count
        dex.pool_count = get_pool_count(rpc_endpoints[0].url, dex.factory_address, rpc_endpoints[0].request_limit, stats);
        
        // Clear existing pools
        dex.pools.clear();
        
        // Loop through pool indices
        for (uint64_t i = 0; i < dex.pool_count; ++i) {
            
            // Get pool address
            std::string addr = get_pool_address(rpc_endpoints[0].url, dex.factory_address, i, rpc_endpoints[0].request_limit, stats);
           
            // Check if address is valid
            if (!addr.empty()) {
                
                // Get tokens
                auto [token0, token1] = get_pool_tokens(rpc_endpoints[0].url, addr, rpc_endpoints[0].request_limit, stats);
                
                // Get liquidity
                uint64_t liquidity = get_pool_liquidity(rpc_endpoints[0].url, addr, rpc_endpoints[0].request_limit, stats);
                
                // Add pool to DEX
                dex.pools.push_back({addr, token0, token1, liquidity});
                
                // Update total liquidity
                dex.liquidity += liquidity;
                
                // Update TVL (simplified)
                dex.tvl += liquidity;
            }
        }
        
        // Mutex for thread synchronization
        std::mutex mtx;
        
        // Progress counter
        std::atomic<int> progress(0);
        
        // Vector for threads
        std::vector<std::thread> threads;
        
        // Launch thread for each pool
        for (uint64_t i = 0; i < dex.pool_count; ++i) {
            
            // Add thread
            threads.emplace_back(get_pool_swap_stats_thread, rpc_endpoints[0].url, dex.pools[i].address, from_block,
                                 latest_block_num, rpc_endpoints[0].request_limit, std::ref(dex.volume_24h),
                                 std::ref(dex.tx_count_24h), std::ref(mtx), std::ref(progress), dex.pool_count);
        }
        // Wait for all threads to finish
        for (auto& t : threads) t.join();
    }

    // Update the DEX section in the config
    // Find DEX array
    size_t dex_pos = content.find("\"dex\": [");
    
    // Check if DEX section exists
    if (dex_pos != std::string::npos) {
        
        // Find end of DEX array
        size_t dex_end = content.find("]", dex_pos);
        
        // Start new DEX section
        std::string new_dex = "\"dex\": [\n";
        
        // Loop through DEXes
        for (size_t i = 0; i < dex_list.size(); ++i) {
            
            // Build DEX entry
            new_dex += "    {\"name\": \"" + dex_list[i].name + "\", \"address\": \"" + dex_list[i].factory_address +
                       "\", \"pools\": " + std::to_string(dex_list[i].pool_count) + "}";
            
            // Add comma if not last
            if (i < dex_list.size() - 1) new_dex += ",\n";
        }
        // Close DEX array
        new_dex += "\n  ]";
        
        // Replace old DEX section
        content = content.substr(0, dex_pos) + new_dex + content.substr(dex_end + 1);
    }

    // Write updated config to file
    // Open file for writing
    std::ofstream out_file("neozork-config");
    
    // Check if file opened
    if (out_file.is_open()) {
        
        // Write updated content
        out_file << content;
        
        // Close file
        out_file.close();
        
        // Report success
        std::cout << GREEN << "Config updated" << RESET << '\n';
        
        // Update disk usage
#ifdef _WIN32
        // Open file handle on Windows
        HANDLE hFile = CreateFile("neozork-config", GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
        
        // Check if handle is valid
        if (hFile != INVALID_HANDLE_VALUE) {
            
            // Structure for file size
            LARGE_INTEGER file_size;
            
            // Get file size
            GetFileSizeEx(hFile, &file_size);
            
            // Update disk usage
            stats.disk_usage_bytes = file_size.QuadPart;
            
            // Close handle
            CloseHandle(hFile);
        }
#else
        // Structure for file stats on Unix
        struct stat file_stat;
        
        // Get file stats
        stat("neozork-config", &file_stat);
        
        // Update disk usage
        stats.disk_usage_bytes = file_stat.st_size;
#endif
    }

    // Finalize timing
    auto end = std::chrono::high_resolution_clock::now(); 
    
    // Calculate execution time
    stats.execution_time_ms = std::chrono::duration_cast<std::chrono::microseconds>(end - start).count() / 1000.0;
}
