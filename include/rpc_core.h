//
//  rpc_core.h
//  NeoZorKDEXArb
//
//  Created by Rostyslav S. on 18.03.2025.
//
#ifndef RPC_CORE_H
#define RPC_CORE_H

#include "main.h"       // For FunctionStats structure
#include <string>       // For string handling in functions

// Callback function to handle data received from CURL
// Parameters:
// - data: Pointer to the received data
// - size: Size of each data element
// - nmemb: Number of data elements
// - buffer: Reference to the string buffer to store the data
// Returns: Number of bytes processed
size_t write_callback(char* data,
                      size_t size,
                      size_t nmemb,
                      std::string& buffer);

// Function to extract the "result" field from a JSON RPC response
// Parameters:
// - json: The JSON string to parse
// Returns: The value of the "result" field or an empty string if not found
std::string parse_json_result(const std::string& json);

// Function to display a progress bar in the console
// Parameters:
// - current: Current progress value
// - total: Total value to reach
// - label: Descriptive label for the progress bar
void print_progress_bar(uint64_t current,
                        uint64_t total,
                        const std::string& label);

// Function to fetch the latest block number from a blockchain
// Parameters:
// - rpc_url: URL of the RPC endpoint
// - request_limit: Maximum requests per second allowed by the endpoint
// - stats: Reference to FunctionStats for performance tracking
// Returns: Latest block number in hex format or empty string if failed
std::string get_latest_block_number(const std::string& rpc_url,
                                    int request_limit,
                                    FunctionStats& stats);

// Function to perform a generic RPC call to a blockchain
// Parameters:
// - rpc_url: URL of the RPC endpoint
// - payload: JSON payload for the RPC request
// - request_limit: Maximum requests per second allowed by the endpoint
// - stats: Reference to FunctionStats for performance tracking
// Returns: Parsed result from the RPC response
std::string make_rpc_call(const std::string& rpc_url,
                          const std::string& payload,
                          int request_limit,
                          FunctionStats& stats);

#endif // RPC_CORE_H
