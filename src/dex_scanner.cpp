//
//  dex_scanner.cpp
//  NeoZorKDEXArb
//
//  Created by Rostyslav S. on 18.03.2025.
//
#include <sstream>
#include "dex_scanner.h"    // Include own header for declarations
#include "rpc_core.h"       // For get_latest_block_number, make_rpc_call
#include "dex_pools.h"      // For get_pool_count
#include "measure.h"        // For update_stats
#include <thread>           // For multi-threading
#include <iostream>         // For console output
#include <iomanip>          // For hex formatting
#include <curl/curl.h>      // For CURL HTTP requests
#include <set>              // For unique factory addresses

void find_factory_contracts(const std::vector<RpcEndpoint>& rpc_endpoints, BlockchainType chain, uint64_t scan_range,
                            int thread_count, std::mutex& mtx, std::vector<DexInfo>& dex_list, FunctionStats& stats) {
    // Start timing the entire scanning process
    auto start = std::chrono::high_resolution_clock::now();

    // Fetch the latest block number
    FunctionStats block_stats; // Stats for block number fetch
    std::string latest_block_hex = get_latest_block_number(rpc_endpoints[0].url, rpc_endpoints[0].request_limit, block_stats);
    if (latest_block_hex.empty()) { // Check if fetch failed
        std::cerr << RED << "Failed to fetch latest block" << RESET << '\n'; // Report error
        return; // Exit function
    }
    uint64_t latest_block = std::stoull(latest_block_hex.substr(2), nullptr, 16); // Convert hex to integer
    uint64_t from_block = latest_block - scan_range; // Calculate starting block

    // Initialize progress tracking
    std::atomic<int> progress(0); // Atomic counter for progress
    int total_tasks = scan_range; // Total blocks to scan
    std::vector<std::thread> threads; // Vector to hold threads
    uint64_t blocks_per_thread = scan_range / thread_count + 1; // Blocks per thread

    // Define event signatures for factory detection (Uniswap V2/V3, PancakeSwap, SushiSwap)
    std::vector<std::string> factory_signatures = {
        "0x0d3648bd0f6ba80134a33ba9275ac585d9d315f0ad8355cddefde31afa28d0e9", // Uniswap V2 PairCreated
        "0x783cca1c0412dd0d695e784568c96da2e9c22ff989357afebc285893228c0d3d", // Uniswap V3 PoolCreated
        "0xb4d9b203a63fc5e69007c33e27f88f7104df62db39e5f846d3da0d2cf255a00e", // PancakeSwap PairCreated
        "0x112c256902bf554d6b36ed07033bc67cf5e7f7a8e02d0c08d0a66c90a7d3c6f6"  // SushiSwap PairCreated
    };

    // Launch scanning threads
    for (int t = 0; t < thread_count; ++t) { // Loop over thread count
        uint64_t start_block = from_block + (t * blocks_per_thread); // Calculate start block for thread
        uint64_t end_block = std::min(start_block + blocks_per_thread, latest_block); // Calculate end block
        if (start_block >= latest_block) break; // Skip if start exceeds latest block

        threads.emplace_back([&, start_block, end_block]() { // Create and add thread
            // Thread-local variables
            FunctionStats local_stats; // Stats for this thread
            std::set<std::string> local_factories; // Set to store unique factories
            CURL* curl = curl_easy_init(); // Initialize CURL for this thread
            std::string read_buffer; // Buffer for RPC responses

            if (curl) { // Check if CURL initialized
                // Set up CURL options
                curl_slist* headers = curl_slist_append(nullptr, "Content-Type: application/json"); // Set JSON header
                curl_easy_setopt(curl, CURLOPT_URL, rpc_endpoints[0].url.c_str()); // Set RPC URL
                curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, write_callback); // Set write callback
                curl_easy_setopt(curl, CURLOPT_WRITEDATA, &read_buffer); // Set response buffer
                curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headers); // Apply headers

                // Scan each block in the thread's range
                for (uint64_t block = start_block; block < end_block; ++block) { // Loop over blocks
                    std::stringstream block_hex; // Stream to build block hex
                    block_hex << "0x" << std::hex << block; // Convert block to hex
                    std::string payload = "{\"jsonrpc\":\"2.0\",\"method\":\"eth_getBlockByNumber\",\"params\":[\"" +
                                          block_hex.str() + "\", true],\"id\":1}"; // Build RPC payload
                    size_t outbound_size = payload.size(); // Calculate payload size
                    curl_easy_setopt(curl, CURLOPT_POSTFIELDS, payload.c_str()); // Set payload

                    // Perform RPC call
                    CURLcode res = curl_easy_perform(curl);
                    if (res != CURLE_OK) { // Check for errors
                        std::cerr << RED << "RPC failed for block " << block << RESET << '\n'; // Report error
                        continue; // Skip to next block
                    }

                    // Parse block data
                    std::string json = read_buffer; // Copy response to local string
                    size_t tx_pos = json.find("\"transactions\": ["); // Find transactions array
                    if (tx_pos != std::string::npos) { // Check if transactions found
                        size_t tx_end = json.find("]", tx_pos); // Find end of transactions
                        size_t pos = tx_pos + 17; // Move past "transactions": [
                        while (pos < tx_end) { // Loop through transactions
                            size_t to_start = json.find("\"to\": \"", pos); // Find "to" field
                            if (to_start == std::string::npos || to_start > tx_end) break; // Exit if not found
                            to_start += 7; // Move past "to": "
                            size_t to_end = json.find('"', to_start); // Find end of address
                            std::string to_addr = json.substr(to_start, to_end - to_start); // Extract address

                            // Check logs for factory events
                            size_t logs_pos = json.find("\"logs\": [", pos); // Find logs array
                            if (logs_pos != std::string::npos && logs_pos < tx_end) { // Check if logs exist
                                size_t logs_end = json.find("]", logs_pos); // Find end of logs
                                size_t log_pos = logs_pos + 9; // Move past "logs": [
                                while (log_pos < logs_end) { // Loop through logs
                                    size_t topic_start = json.find("\"topics\": [", log_pos); // Find topics array
                                    if (topic_start == std::string::npos || topic_start > logs_end) break; // Exit if not found
                                    topic_start += 11; // Move past "topics": [
                                    size_t topic_end = json.find(',', topic_start); // Find end of first topic
                                    std::string topic = json.substr(topic_start + 1, topic_end - topic_start - 2); // Extract topic
                                    if (std::find(factory_signatures.begin(), factory_signatures.end(), topic) != factory_signatures.end()) { // Check if topic matches
                                        local_factories.insert(to_addr); // Add factory address
                                        break; // Move to next transaction
                                    }
                                    log_pos = json.find("{", log_pos + 1); // Move to next log
                                    if (log_pos == std::string::npos) break; // Exit if no more logs
                                }
                            }
                            pos = json.find("{", pos + 1); // Move to next transaction
                            if (pos == std::string::npos) break; // Exit if no more transactions
                        }
                    }
                    read_buffer.clear(); // Clear buffer for next block
                    local_stats.outbound_traffic += outbound_size; // Update outbound traffic
                    local_stats.inbound_traffic += read_buffer.size(); // Update inbound traffic
                    progress++; // Increment progress
                    print_progress_bar(progress, total_tasks, "Scanning blocks"); // Update progress bar
                    std::this_thread::sleep_for(std::chrono::milliseconds(1000 / rpc_endpoints[0].request_limit)); // Respect rate limit
                }
                curl_slist_free_all(headers); // Free headers
                curl_easy_cleanup(curl); // Clean up CURL
            }

            // Merge local factories into global list
            std::lock_guard<std::mutex> lock(mtx); // Lock mutex
            for (const auto& addr : local_factories) { // Loop through local factories
                if (std::find_if(dex_list.begin(), dex_list.end(), [&](const DexInfo& d) { return d.factory_address == addr; }) == dex_list.end()) { // Check for duplicates
                    DexInfo dex; // Create new DEX entry
                    dex.factory_address = addr; // Set factory address
                    dex.name = "Unknown_" + addr.substr(2, 6); // Generate name from address
                    dex.pool_count = get_pool_count(rpc_endpoints[0].url, dex.factory_address, rpc_endpoints[0].request_limit, local_stats); // Get pool count
                    if (dex.pool_count > 0) dex_list.push_back(dex); // Add to list if valid
                }
            }
            stats.latency_ms += local_stats.latency_ms; // Aggregate latency
            stats.outbound_traffic += local_stats.outbound_traffic; // Aggregate outbound traffic
            stats.inbound_traffic += local_stats.inbound_traffic; // Aggregate inbound traffic
        });
    }

    // Wait for all threads to complete
    for (auto& thread : threads) thread.join(); // Join each thread
    std::cout << std::endl; // New line after progress bar

    // Finalize timing and output results
    auto end = std::chrono::high_resolution_clock::now(); // End timing
    stats.execution_time_ms = std::chrono::duration_cast<std::chrono::microseconds>(end - start).count() / 1000.0; // Calculate execution time
    std::cout << GREEN << "Found " << dex_list.size() << " factory contracts" << RESET << '\n'; // Report number of factories found
}
