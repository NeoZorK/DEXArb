//
//  dex_pools.h
//  NeoZorKDEXArb
//
//  Created by Rostyslav S. on 18.03.2025.
//

#ifndef DEX_POOLS_H
#define DEX_POOLS_H

#include "main.h"       // Include main header for shared structures
#include <string>       // For string handling in functions

// Function to fetch the number of pools from a factory contract
uint64_t get_pool_count(const std::string& rpc_url, const std::string& factory_address, int request_limit, FunctionStats& stats);

// Function to fetch pool address by index from a factory
std::string get_pool_address(const std::string& rpc_url, const std::string& factory_address, uint64_t index, int request_limit, FunctionStats& stats);

// Function to fetch liquidity of a pool
uint64_t get_pool_liquidity(const std::string& rpc_url, const std::string& pool_address, int request_limit, FunctionStats& stats);

#endif // DEX_POOLS_H
