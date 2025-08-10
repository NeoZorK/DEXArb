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

std::vector<DexInfo> load_dexes_from_config() {
    std::vector<DexInfo> dex_list;
    std::ifstream config_file("neozork-config"); // Open config file
    if (!config_file.is_open()) {
        std::cout << "DEBUG: Config file not found or cannot be opened" << std::endl;
        return dex_list; // Return empty list if file doesn't exist
    }

    std::stringstream buffer; // Buffer for file content
    buffer << config_file.rdbuf(); // Read file into buffer
    std::string content = buffer.str(); // Convert to string
    config_file.close(); // Close file

    std::cout << "DEBUG: Config file content length: " << content.length() << std::endl;
    std::cout << "DEBUG: Config file content preview: " << content.substr(0, 200) << "..." << std::endl;

    for (const auto& chain : {"ethereum", "fantom", "bsc", "polygon", "avalanche", "solana"}) {
        size_t chain_pos = content.find("\"" + std::string(chain) + "\": {"); // Find blockchain section
        if (chain_pos == std::string::npos) {
            std::cout << "DEBUG: Chain " << chain << " not found in config" << std::endl;
            continue; // Skip if not found
        }

        std::cout << "DEBUG: Found chain " << chain << " at position " << chain_pos << std::endl;

        size_t dex_pos = content.find("\"dex\": [", chain_pos); // Find DEX array
        if (dex_pos != std::string::npos) { // Check if DEX section exists
            size_t dex_end = content.find("]", dex_pos); // Find end of DEX array
            if (dex_end == std::string::npos) {
                std::cout << "DEBUG: DEX section end not found for " << chain << std::endl;
                continue;
            }
            
            std::cout << "DEBUG: DEX section for " << chain << " from " << dex_pos << " to " << dex_end << std::endl;
            
            size_t pos = dex_pos + 8; // Move past "dex": [
            while (pos < dex_end && pos < content.length()) { // Loop through DEX entries
                size_t addr_start = content.find("\"factory_address\": \"", pos); // Find factory address field
                if (addr_start == std::string::npos || addr_start > dex_end) break;
                
                addr_start += 19; // Move past "factory_address": "
                size_t addr_end = content.find('"', addr_start); // Find end of address
                if (addr_end == std::string::npos || addr_end > dex_end) break;
                
                // Validate string bounds before substr
                if (addr_start < content.length() && addr_end < content.length() && addr_start < addr_end) {
                    std::string factory_address = content.substr(addr_start, addr_end - addr_start); // Extract address
                    std::cout << "DEBUG: Found factory address: " << factory_address << std::endl;

                    // Fix: create DexInfo object and set only required fields
                    DexInfo dex;
                    if (factory_address.length() >= 8) {
                        dex.name = "Unknown_" + factory_address.substr(2, 6);
                    } else {
                        dex.name = "Unknown_" + factory_address;
                    }
                    dex.factory_address = factory_address;
                    dex_list.push_back(dex); // Add to list
                }

                pos = content.find("{", pos + 1); // Move to next entry
                if (pos == std::string::npos) break; // Exit if no more entries
            }
        } else {
            std::cout << "DEBUG: No DEX section found for " << chain << std::endl;
        }
    }
    
    std::cout << "DEBUG: Loaded " << dex_list.size() << " DEXes from config" << std::endl;
    return dex_list; // Return list of DEXes
}

void update_config_with_dex(const std::vector<RpcEndpoint>& rpc_endpoints, std::vector<DexInfo>& dex_list, FunctionStats& stats) {
    // Start timing the function
    auto start = std::chrono::high_resolution_clock::now();

    // Load existing config content
    std::ifstream in_file("neozork-config"); // Open config file
    std::stringstream buffer; // Buffer for file content
    buffer << in_file.rdbuf(); // Read file into buffer
    std::string content = buffer.str(); // Convert to string
    in_file.close(); // Close the file

    // Fetch latest block number for 24-hour range
    FunctionStats block_stats; // Stats for block fetch
    std::string latest_block = get_latest_block_number(rpc_endpoints[0].url, rpc_endpoints[0].request_limit, block_stats); // Get latest block
    
    // Check if block fetch failed
    if (latest_block.empty()) {
        std::cerr << "Failed to fetch latest block for config update" << std::endl;
        return; // Exit early if block fetch failed
    }
    
    // Validate hex string format
    if (latest_block.length() < 3 || latest_block.substr(0, 2) != "0x") {
        std::cerr << "Invalid block number format: " << latest_block << std::endl;
        return; // Exit early if format is invalid
    }
    
    uint64_t latest_block_num = std::stoull(latest_block.substr(2), nullptr, 16); // Convert to integer
    uint64_t from_block = latest_block_num - (24 * 60 * 60 / 2); // Calculate block 24 hours ago (assuming 2s block time)

    // Update each DEX with fresh data
    for (auto& dex : dex_list) { // Loop through DEXes
        dex.pool_count = get_pool_count(rpc_endpoints[0].url, dex.factory_address, rpc_endpoints[0].request_limit, stats); // Update pool count
        dex.pools.clear(); // Clear existing pools
        for (uint64_t i = 0; i < dex.pool_count; ++i) { // Loop through pool indices
            std::string addr = get_pool_address(rpc_endpoints[0].url, dex.factory_address, i, rpc_endpoints[0].request_limit, stats); // Get pool address
            if (!addr.empty()) { // Check if address is valid
                auto [token0, token1] = get_pool_tokens(rpc_endpoints[0].url, addr, rpc_endpoints[0].request_limit, stats); // Get tokens
                uint64_t liquidity = get_pool_liquidity(rpc_endpoints[0].url, addr, rpc_endpoints[0].request_limit, stats); // Get liquidity
                dex.pools.push_back({addr, token0, token1, liquidity}); // Add pool to DEX
                dex.liquidity += liquidity; // Update total liquidity
                dex.tvl += liquidity; // Update TVL (simplified)
            }
        }
        std::mutex mtx; // Mutex for thread synchronization
        std::atomic<int> progress(0); // Progress counter
        std::vector<std::thread> threads; // Vector for threads
        for (uint64_t i = 0; i < dex.pool_count; ++i) { // Launch thread for each pool
            threads.emplace_back(get_pool_swap_stats_thread, rpc_endpoints[0].url, dex.pools[i].address, from_block,
                                 latest_block_num, rpc_endpoints[0].request_limit, std::ref(dex.volume_24h),
                                 std::ref(dex.tx_count_24h), std::ref(mtx), std::ref(progress), dex.pool_count); // Add thread
        }
        for (auto& t : threads) t.join(); // Wait for all threads to finish
    }

    // Update the DEX section in the config
    size_t dex_pos = content.find("\"dex\": ["); // Find DEX array
    if (dex_pos != std::string::npos) { // Check if DEX section exists
        size_t dex_end = content.find("]", dex_pos); // Find end of DEX array
        std::string new_dex = "\"dex\": [\n"; // Start new DEX section
        for (size_t i = 0; i < dex_list.size(); ++i) { // Loop through DEXes
            new_dex += "    {\"name\": \"" + dex_list[i].name + "\", \"address\": \"" + dex_list[i].factory_address +
                       "\", \"pools\": " + std::to_string(dex_list[i].pool_count) + "}"; // Build DEX entry
            if (i < dex_list.size() - 1) new_dex += ",\n"; // Add comma if not last
        }
        new_dex += "\n  ]"; // Close DEX array
        content = content.substr(0, dex_pos) + new_dex + content.substr(dex_end + 1); // Replace old DEX section
    }

    // Write updated config to file
    std::ofstream out_file("neozork-config"); // Open file for writing
    if (out_file.is_open()) { // Check if file opened
        out_file << content; // Write updated content
        out_file.close(); // Close file
        std::cout << GREEN << "Config updated" << RESET << '\n'; // Report success
#ifdef _WIN32
        HANDLE hFile = CreateFile("neozork-config", GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL); // Open file handle on Windows
        if (hFile != INVALID_HANDLE_VALUE) { // Check if handle is valid
            LARGE_INTEGER file_size; // Structure for file size
            GetFileSizeEx(hFile, &file_size); // Get file size
            stats.disk_usage_bytes = file_size.QuadPart; // Update disk usage
            CloseHandle(hFile); // Close handle
        }
#else
        struct stat file_stat; // Structure for file stats on Unix
        stat("neozork-config", &file_stat); // Get file stats
        stats.disk_usage_bytes = static_cast<size_t>(file_stat.st_size); // Update disk usage
#endif
    }

    // Finalize timing
    auto end = std::chrono::high_resolution_clock::now(); // End timing
    stats.execution_time_ms = std::chrono::duration_cast<std::chrono::microseconds>(end - start).count() / 1000.0; // Calculate execution time
}
