//
//  rpc_core.h
//  NeoZorKDEXArb
//
//  Created by Rostyslav S. on 18.03.2025.
//
#ifndef RPC_CORE_H
#define RPC_CORE_H

#include "main.h"       // Include main header for shared structures

// CURL write callback to handle response data
size_t write_callback(char* data, size_t size, size_t nmemb, std::string& buffer);

// Function to parse "result" field from JSON RPC response
std::string parse_json_result(const std::string& json);

// Function to print a progress bar in the console
void print_progress_bar(int current, int total, const std::string& label);

// Function to fetch the latest block number from a blockchain
std::string get_latest_block_number(const std::string& rpc_url, int request_limit, FunctionStats& stats);

// Generic RPC call function
std::string make_rpc_call(const std::string& rpc_url, const std::string& payload, int request_limit, FunctionStats& stats);

#endif // RPC_CORE_H
