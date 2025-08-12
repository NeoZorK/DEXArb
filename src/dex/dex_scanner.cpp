//
//  dex_scanner.cpp
//  NeoZorKDEXArb
//
//  Created by Rostyslav S. on 18.03.2025.
//
#include "dex/dex_scanner.h"
#include "main.h"           // For RpcEndpoint, BlockchainType, DexInfo, FunctionStats, and color constants
#include "network/rpc_core.h"       // For get_latest_block_number, write_callback, print_progress_bar
#include <iostream>         // For console output
#include <sstream>          // For stringstream
#include <chrono>           // For timing
#include <thread>           // For thread management
#include <mutex>            // For mutex
#include <atomic>           // For atomic operations
#include <set>              // For set operations
#include <string>           // For std::string
#include <curl/curl.h>      // For HTTP requests

// Function to scan a blockchain for factory contracts of decentralized exchanges (DEXes)
// Parameters:
// - rpc_endpoints: List of RPC endpoints to query the blockchain
// - chain: Type of blockchain to scan (e.g., Ethereum, BSC)
// - scan_range: Number of blocks to scan backwards from the latest block
// - thread_count: Number of threads to use for parallel scanning
// - mtx: Mutex for synchronizing access to shared dex_list
// - dex_list: Vector to store discovered DEX factory contracts
// - stats: Reference to FunctionStats for performance tracking
void find_factory_contracts(const std::vector<RpcEndpoint>& rpc_endpoints, [[maybe_unused]] BlockchainType chain, uint64_t scan_range,
                           int thread_count, std::mutex& mtx, std::vector<DexInfo>& dex_list, FunctionStats& stats) {
    try {
        std::cout << "DEBUG: find_factory_contracts called with " << rpc_endpoints.size() << " endpoints" << std::endl;
        
        // Check if we have RPC endpoints
        if (rpc_endpoints.empty()) {
            std::cerr << RED << "No RPC endpoints provided" << RESET << '\n';
            return;
        }
        
        std::cout << "DEBUG: First RPC endpoint: " << rpc_endpoints[0].url << std::endl;
        
        // Record the start time for performance measurement
        auto start = std::chrono::high_resolution_clock::now();

        // Temporary stats object for fetching the latest block number
        FunctionStats block_stats;
        // Try to fetch the latest block number from available RPC endpoints
        std::string latest_block_hex;
        bool block_fetched = false;
        
        for (const auto& endpoint : rpc_endpoints) {
            std::cout << "DEBUG: Trying RPC endpoint: " << endpoint.url << std::endl;
            latest_block_hex = get_latest_block_number(endpoint.url, endpoint.request_limit, block_stats);
            if (!latest_block_hex.empty()) {
                std::cout << "DEBUG: Successfully fetched block from: " << endpoint.url << std::endl;
                block_fetched = true;
                break;
            }
        }
        
        // Check if the block number fetch failed from all endpoints
        if (!block_fetched) {
            // Print error message to console
            std::cerr << RED << "Failed to fetch latest block from all RPC endpoints" << RESET << '\n';
            // Exit the function early
            return;
        }
        
        // Validate hex string format
        if (latest_block_hex.empty() || latest_block_hex.length() < 3 || latest_block_hex.substr(0, 2) != "0x") {
            std::cerr << RED << "Invalid block number format: " << latest_block_hex << RESET << '\n';
            return;
        }
        
        // Convert hex string to 64-bit unsigned integer, removing "0x" prefix
        uint64_t latest_block = std::stoull(latest_block_hex.substr(2), nullptr, 16);
        // Calculate the starting block number based on the scan range
        uint64_t from_block = latest_block - scan_range;

        // Atomic counter for tracking progress across threads
        std::atomic<uint64_t> progress(0);
        // Total number of blocks to scan (matches scan_range)
        uint64_t total_tasks = scan_range;
        // Vector to store thread objects
        std::vector<std::thread> threads;
        // Calculate number of blocks each thread will process
        uint64_t blocks_per_thread = scan_range / static_cast<uint64_t>(thread_count) + 1;

        // List of known factory contract event signatures (e.g., PairCreated)
        std::vector<std::string> factory_signatures = {
            "0x0d3648bd0f6ba80134a33ba9275ac585d9d315f0ad8355cddefde31afa28d0e9",
            "0x783cca1c0412dd0d695e784568c96da2e9c22ff989357afebc285893228c0d3d",
            "0xb4d9b203a63fc5e69007c33e27f88f7104df62db39e5f846d3da0d2cf255a00e",
            "0x112c256902bf554d6b36ed07033bc67cf5e7f7a8e02d0c08d0a66c90a7d3c6f6"
        };

        // Launch threads for parallel block scanning
        for (int t = 0; t < thread_count; ++t) {
            // Calculate the start block for this thread
            uint64_t start_block = from_block + (static_cast<uint64_t>(t) * blocks_per_thread);
            // Calculate the end block, ensuring it doesn't exceed the latest block
            uint64_t end_block = std::min(start_block + blocks_per_thread, latest_block);
            // Skip if the start block is beyond the latest block
            if (start_block >= latest_block) break;

            // Create a thread with a lambda function for scanning
            threads.emplace_back([&, start_block, end_block]() {
                // Local stats object for this thread
                FunctionStats local_stats;
                // Set to store unique factory addresses found in this thread
                std::set<std::string> local_factories;
                // Initialize CURL handle for HTTP requests
                CURL* curl = curl_easy_init();
                // Buffer to store RPC response data
                std::string read_buffer;

                // Check if CURL initialization succeeded
                if (curl) {
                    // Set HTTP headers for JSON content type
                    curl_slist* headers = curl_slist_append(nullptr, "Content-Type: application/json");
                    // Set the RPC URL for this thread
                    curl_easy_setopt(curl, CURLOPT_URL, rpc_endpoints[0].url.c_str());
                    // Set the callback function to handle response data
                    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, write_callback);
                    // Set the buffer to write response data into
                    curl_easy_setopt(curl, CURLOPT_WRITEDATA, &read_buffer);
                    // Apply the headers to the CURL request
                    curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headers);

                    // Iterate over the assigned block range
                    for (uint64_t block = start_block; block < end_block; ++block) {
                        // Create a stringstream to build the block number in hex
                        std::stringstream block_hex;
                        // Format the block number as a hex string with "0x" prefix
                        block_hex << "0x" << std::hex << block;
                        // Construct the RPC payload to fetch block data
                        std::string payload = "{\"jsonrpc\":\"2.0\",\"method\":\"eth_getBlockByNumber\",\"params\":[\"" +
                                              block_hex.str() + "\", true],\"id\":1}";
                        // Record the size of the outbound payload (unused for now)
                        // size_t outbound_size = payload.size();
                        // Set the payload for the POST request
                        curl_easy_setopt(curl, CURLOPT_POSTFIELDS, payload.c_str());

                        // Perform the RPC request
                        CURLcode res = curl_easy_perform(curl);
                        // Check if the request failed
                        if (res != CURLE_OK) {
                            // Print error message with block number
                            std::cerr << RED << "RPC failed for block " << block << RESET << '\n';
                            // Skip to the next block
                            continue;
                        }

                        // Copy the response data for processing
                        std::string json = read_buffer;
                        // Find the transactions array in the JSON response
                        size_t tx_pos = json.find("\"transactions\": [");
                        // Check if transactions were found
                        if (tx_pos != std::string::npos) {
                            // Find the end of the transactions array
                            size_t tx_end = json.find("]", tx_pos);
                            // Start position after "transactions": [
                            size_t pos = tx_pos + 17;
                            // Iterate through transactions
                            while (pos < tx_end && pos < json.length()) {
                                // Find the "to" field in the transaction
                                size_t to_pos = json.find("\"to\": \"", pos);
                                if (to_pos == std::string::npos || to_pos >= tx_end) break;
                                
                                // Extract the contract address
                                to_pos += 7; // Move past "to": "
                                size_t addr_end = json.find('"', to_pos);
                                if (addr_end == std::string::npos || addr_end >= tx_end) break;
                                
                                // Validate string bounds before substr
                                if (to_pos < json.length() && addr_end < json.length() && to_pos < addr_end) {
                                    std::string contract_addr = json.substr(to_pos, addr_end - to_pos);
                                    
                                    // Check if this is a factory contract by looking for known signatures
                                    for (const auto& sig : factory_signatures) {
                                        if (json.find(sig, pos) != std::string::npos) {
                                            local_factories.insert(contract_addr);
                                            break;
                                        }
                                    }
                                }
                                
                                // Move to next transaction
                                pos = json.find("{", pos + 1);
                                if (pos == std::string::npos || pos >= tx_end) break;
                            }
                        }
                        
                        // Clear buffer for next request
                        read_buffer.clear();
                        
                        // Update progress
                        progress++;
                        // Print progress bar every 100 blocks
                        if (progress % 100 == 0) {
                            print_progress_bar(progress.load(), total_tasks, "Scanning blocks");
                        }
                    }
                    
                    // Clean up CURL resources
                    curl_slist_free_all(headers);
                    curl_easy_cleanup(curl);
                }
                
                // Update global stats
                std::lock_guard<std::mutex> lock(mtx);
                stats.add(local_stats);
                
                // Add found factories to global list
                for (const auto& factory : local_factories) {
                    DexInfo dex;
                    dex.name = "Unknown_" + factory.substr(2, 6);
                    dex.factory_address = factory;
                    dex_list.push_back(dex);
                }
            });
        }
        
        // Wait for all threads to complete
        for (auto& t : threads) {
            t.join();
        }
        
        // Record the end time for performance measurement
        auto end = std::chrono::high_resolution_clock::now();
        // Calculate execution time in milliseconds
        stats.execution_time_ms = std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count();
        
        // Print final progress
        print_progress_bar(total_tasks, total_tasks, "Scanning blocks");
        std::cout << '\n';
        
        // Print summary
        std::cout << GREEN << "Scan completed. Found " << dex_list.size() << " factory contracts." << RESET << '\n';
        
    } catch (const std::exception& e) {
        std::cerr << RED << "Exception in find_factory_contracts: " << e.what() << RESET << '\n';
    } catch (...) {
        std::cerr << RED << "Unknown exception in find_factory_contracts" << RESET << '\n';
    }
}
