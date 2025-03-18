//
//  queries.h
//  NeoZorKDEXArb
//
//  Created by Rostyslav S. on 18.03.2025.
//

#ifndef QUERIES_H
#define QUERIES_H

#include "main.h"       // For RpcEndpoint

// Function to show pools for a DEX
void show_pools(const std::vector<RpcEndpoint>& rpc_endpoints, const std::string& dex_identifier);

// Function to show tokens for a DEX
void show_tokens(const std::vector<RpcEndpoint>& rpc_endpoints, const std::string& dex_identifier);

// Function to find a token in a specific DEX
void find_token_in_dex(const std::vector<RpcEndpoint>& rpc_endpoints, const std::string& dex_identifier, const std::string& token_address);

// Function to find a token across all DEXes
void find_tokens_across_dexes(const std::vector<RpcEndpoint>& rpc_endpoints, const std::string& token_address);

// Function to show all DEXes
void show_dexes(const std::vector<RpcEndpoint>& rpc_endpoints);

#endif // QUERIES_H
