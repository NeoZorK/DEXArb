//
//  dex_scanner.cpp
//  NeoZorKDEXArb
//
//  Created by Rostyslav S. on 18.03.2025.
//
#include "dex_scanner.h"    // Include own header for declarations
#include "rpc_core.h"       // For get_latest_block_number, make_rpc_call
#include "dex_pools.h"      // For get_pool_count
#include "measure.h"        // For update_stats
#include <thread>           // For multi-threading
#include <iostream>         // For console output
#include <iomanip>          // For hex formatting
#include <sstream>          // For stringstream
#include <curl/curl.h>      // For CURL HTTP requests
#include <set>              // For unique factory addresses
#include <chrono>           // For timing measurements
#include <string>
#include <unordered_map>

// Function to test RPC endpoint availability
// Parameters:
// - endpoint: RPC endpoint to test
// - stats: Performance statistics
// Returns: True if the endpoint is available, false otherwise
bool test_rpc_endpoint(const RpcEndpoint& endpoint, FunctionStats& stats) {
    // Construct a simple RPC payload to test connectivity
    std::string payload = "{\"jsonrpc\":\"2.0\",\"method\":\"eth_chainId\",\"params\":[],\"id\":1}";
    // Attempt to make an RPC call
    std::string result = make_rpc_call(endpoint.url, payload, endpoint.request_limit, stats);
    // Check if the result is non-empty (indicating success)
    return !result.empty();
}

// Blockchain to string mapping
std::string blockchain_to_string(BlockchainType blockchain) {
    static const std::unordered_map<BlockchainType, std::string> blockchain_strings = {
        {BlockchainType::Ethereum, "Ethereum"},
        {BlockchainType::BSC, "Binance Smart Chain"},
        {BlockchainType::Solana, "Solana"},
        // ...
    };

    auto it = blockchain_strings.find(blockchain);
    return it != blockchain_strings.end() ? it->second : "Unknown blockchain";
}

// Function to scan a blockchain for factory contracts of decentralized exchanges (DEXes)
// Parameters:
// - rpc_endpoints: List of RPC endpoints to query the blockchain
// - chain: Type of blockchain to scan
// - scan_range: Number of blocks to scan backwards from the latest block
// - thread_count: Number of threads to use for parallel scanning
// - mtx: Mutex for synchronizing access to shared dex_list
// - dex_list: Vector to store discovered DEX factory contracts
// - stats: Reference to FunctionStats for performance tracking
void find_factory_contracts(const std::vector<RpcEndpoint>& rpc_endpoints, BlockchainType chain, uint64_t scan_range,
                            int thread_count, std::mutex& mtx, std::vector<DexInfo>& dex_list, FunctionStats& stats) {
    // Record the start time for performance measurement
    auto start = std::chrono::high_resolution_clock::now();

    // Log the blockchain being scanned
    std::cout << "Scanning " << blockchain_to_string(chain) << " for factory contracts...\n";

    // Check if RPC endpoints are provided
    if (rpc_endpoints.empty()) {
        std::cerr << RED << "No RPC endpoints provided for " << blockchain_to_string(chain) << RESET << '\n';
        return;
    }

    // Test and select a working RPC endpoint
    RpcEndpoint selected_endpoint = rpc_endpoints[0]; // Default to first endpoint
    bool endpoint_found = false;
    for (const auto& endpoint : rpc_endpoints) {
        std::cout << "Testing RPC endpoint: " << endpoint.url << "\n";
        if (test_rpc_endpoint(endpoint, stats)) {
            selected_endpoint = endpoint;
            endpoint_found = true;
            std::cout << GREEN << "Selected RPC endpoint: " << endpoint.url << RESET << '\n';
            break;
        } else {
            std::cerr << RED << "RPC endpoint " << endpoint.url << " is unavailable" << RESET << '\n';
        }
    }
    // Exit if no endpoints are available
    if (!endpoint_found) {
        std::cerr << RED << "Failed to load RPC endpoints for " << blockchain_to_string(chain) << RESET << '\n';
        return;
    }

    // Temporary stats object for fetching the latest block number
    FunctionStats block_stats;
    // Fetch the latest block number in hex format from the selected RPC endpoint
    std::string latest_block_hex = get_latest_block_number(selected_endpoint.url, selected_endpoint.request_limit, block_stats);
    // Check if the block number fetch failed
    if (latest_block_hex.empty()) {
        // Print error message to console
        std::cerr << RED << "Failed to fetch latest block on " << blockchain_to_string(chain) << " using " << selected_endpoint.url << RESET << '\n';
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
    uint64_t blocks_per_thread = scan_range / thread_count + 1;

    // List of known factory contract event signatures (e.g., PairCreated)
    std::vector<std::string> factory_signatures = {
        "0x0d3648bd0f6ba80134a33ba9275ac585d9d315f0ad8355cddefde31afa28d0e9", // Uniswap-like PairCreated
        "0x783cca1c0412dd0d695e784568c96da2e9c22ff989357afebc285893228c0d3d", // Additional signature
        "0xb4d9b203a63fc5e69007c33e27f88f7104df62db39e5f846d3da0d2cf255a00e", // Additional signature
        "0x112c256902bf554d6b36ed07033bc67cf5e7f7a8e02d0c08d0a66c90a7d3c6f6"  // Additional signature
    };

    // Launch threads for parallel block scanning
    for (int t = 0; t < thread_count; ++t) {
        // Calculate the start block for this thread
        uint64_t start_block = from_block + (t * blocks_per_thread);
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
                curl_easy_setopt(curl, CURLOPT_URL, selected_endpoint.url.c_str());
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
                    // Record the size of the outbound payload
                    size_t outbound_size = payload.size();
                    // Set the payload for the POST request
                    curl_easy_setopt(curl, CURLOPT_POSTFIELDS, payload.c_str());

                    // Perform the RPC request
                    CURLcode res = curl_easy_perform(curl);
                    // Check if the request failed
                    if (res != CURLE_OK) {
                        // Print error message with block number
                        std::cerr << RED << "RPC failed for block " << block << " on " << blockchain_to_string(chain) << ": "
                                  << curl_easy_strerror(res) << RESET << '\n';
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
                        while (pos < tx_end) {
                            // Find the "to" field in the transaction
                            size_t to_start = json.find("\"to\": \"", pos);
                            // Break if "to" field not found or beyond transactions
                            if (to_start == std::string::npos || to_start > tx_end) break;
                            // Move past "to": "
                            to_start += 7;
                            // Find the end of the "to" address
                            size_t to_end = json.find('"', to_start);
                            // Extract the destination address
                            std::string to_addr = json.substr(to_start, to_end - to_start);

                            // Find the logs array in the transaction
                            size_t logs_pos = json.find("\"logs\": [", pos);
                            // Check if logs were found within the transaction
                            if (logs_pos != std::string::npos && logs_pos < tx_end) {
                                // Find the end of the logs array
                                size_t logs_end = json.find("]", logs_pos);
                                // Start position after "logs": [
                                size_t log_pos = logs_pos + 9;
                                // Iterate through logs
                                while (log_pos < logs_end) {
                                    // Find the topics array in the log
                                    size_t topic_start = json.find("\"topics\": [", log_pos);
                                    // Break if topics not found or beyond logs
                                    if (topic_start == std::string::npos || topic_start > logs_end) break;
                                    // Move past "topics": [
                                    topic_start += 11;
                                    // Find the end of the first topic
                                    size_t topic_end = json.find(',', topic_start);
                                    // Extract the topic (event signature)
                                    std::string topic = json.substr(topic_start + 1, topic_end - topic_start - 2);
                                    // Check if the topic matches a known factory signature
                                    if (std::find(factory_signatures.begin(), factory_signatures.end(), topic) != factory_signatures.end()) {
                                        // Add the address to the local factory set
                                        local_factories.insert(to_addr);
                                        // Exit the logs loop since a match was found
                                        break;
                                    }
                                    // Move to the next log entry
                                    log_pos = json.find("{", log_pos + 1);
                                    // Break if no more logs
                                    if (log_pos == std::string::npos) break;
                                }
                            }
                            // Move to the next transaction
                            pos = json.find("{", pos + 1);
                            // Break if no more transactions
                            if (pos == std::string::npos) break;
                        }
                    }
                    // Clear the response buffer for the next request
                    read_buffer.clear();
                    // Update outbound traffic stats
                    local_stats.outbound_traffic += outbound_size;
                    // Update inbound traffic stats
                    local_stats.inbound_traffic += read_buffer.size();
                    // Increment the progress counter
                    progress++;
                    // Display the progress bar with atomic value explicitly loaded
                    print_progress_bar(progress.load(), total_tasks, "Scanning blocks on " + blockchain_to_string(chain));
                    // Throttle requests to respect the RPC limit
                    std::this_thread::sleep_for(std::chrono::milliseconds(1000 / selected_endpoint.request_limit));
                }
                // Free the CURL headers
                curl_slist_free_all(headers);
                // Clean up the CURL handle
                curl_easy_cleanup(curl);
            }

            // Lock the mutex to safely update the shared dex_list
            std::lock_guard<std::mutex> lock(mtx);
            // Iterate over found factory addresses
            for (const auto& addr : local_factories) {
                // Check if this address is already in the dex_list
                if (std::find_if(dex_list.begin(), dex_list.end(), [&](const DexInfo& d) { return d.factory_address == addr; }) == dex_list.end()) {
                    // Create a new DexInfo object
                    DexInfo dex;
                    // Set the factory address
                    dex.factory_address = addr;
                    // Generate a temporary name based on the address and chain
                    dex.name = "Unknown_" + blockchain_to_string(chain) + "_" + addr.substr(2, 6);
                    // Fetch the pool count for this factory
                    dex.pool_count = get_pool_count(selected_endpoint.url, dex.factory_address, selected_endpoint.request_limit, local_stats);
                    // Add the DEX to the list if it has pools
                    if (dex.pool_count > 0) dex_list.push_back(dex);
                }
            }
            // Aggregate latency stats from this thread
            stats.latency_ms += local_stats.latency_ms;
            // Aggregate outbound traffic stats
            stats.outbound_traffic += local_stats.outbound_traffic;
            // Aggregate inbound traffic stats
            stats.inbound_traffic += local_stats.inbound_traffic;
        });
    }

    // Wait for all threads to complete
    for (auto& thread : threads) thread.join();
    // Print a newline after the progress bar
    std::cout << std::endl;

    // Record the end time for performance measurement
    auto end = std::chrono::high_resolution_clock::now();
    // Calculate the total execution time in milliseconds
    stats.execution_time_ms = std::chrono::duration_cast<std::chrono::microseconds>(end - start).count() / 1000.0;
    // Print the number of factory contracts found
    std::cout << GREEN << "Found " << dex_list.size() << " factory contracts on " << blockchain_to_string(chain)
              << " in " << stats.execution_time_ms << " ms" << RESET << '\n';
}
