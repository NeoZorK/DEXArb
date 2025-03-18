//
//  input.h
//  NeoZorKDEXArb
//
//  Created by Rostyslav S. on 18.03.2025.
//
#ifndef INPUT_H
#define INPUT_H

#include "main.h"           // Include main header for shared structures and definitions
#include "blockchain.h"     // Include blockchain header for blockchain-related types and functions
#include <vector>           // Include vector for dynamic arrays (used in function return types)

// Function to create a default configuration file if it doesn't exist
void create_config_file(int thread_count, FunctionStats& stats);
// Parameters:
// - thread_count: Number of threads to set in the config for parallel processing
// - stats: Reference to FunctionStats to track performance metrics (e.g., execution time, disk usage)
// Description: Generates a 'neozork-config' JSON file with default settings for all supported blockchains

// Function to read RPC endpoints and thread count from the config file for a specific blockchain
std::pair<std::vector<RpcEndpoint>, int> read_config_file(const std::string& blockchain, FunctionStats& stats);
// Parameters:
// - blockchain: Name of the blockchain (e.g., "ethereum") to read config for
// - stats: Reference to FunctionStats to track performance metrics
// Returns: Pair containing a vector of RpcEndpoint structures and the number of threads
// Description: Parses 'neozork-config' to extract RPC endpoints and thread count; creates default config if missing

// Function to display the current scan configuration from the config file
void show_scan_config();
// Description: Reads 'neozork-config' and prints the settings (threads, RPC URLs, limits) for all blockchains

// Function to display the results of a blockchain scan
void show_scan_results(const std::vector<DexInfo>& dex_list);
// Parameters:
// - dex_list: Vector of DexInfo structures containing discovered DEX factory contracts
// Description: Prints the list of found DEXes with their names, addresses, and pool counts

// Function to save performance statistics to a file
void save_scan_stats(const std::vector<std::pair<std::string, FunctionStats>>& stats_list);
// Parameters:
// - stats_list: Vector of pairs containing function names and their performance stats
// Description: Writes detailed performance metrics (time, memory, traffic) to 'neozork-scan-stat' in JSON format

// Function to display saved scan statistics
void show_scan_stats();
// Description: Reads 'neozork-scan-stat' and prints performance metrics for each tracked function

#endif // INPUT_H
