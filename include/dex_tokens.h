//
//  dex_tokens.h
//  NeoZorKDEXArb
//
//  Created by Rostyslav S. on 18.03.2025.
//

#ifndef DEX_TOKENS_H
#define DEX_TOKENS_H

#include "main.h"       // Include main header for shared structures
#include <string>       // For string handling

// Function to fetch token addresses for a pool
std::pair<std::string, std::string> get_pool_tokens(const std::string& rpc_url,
                                                    const std::string& pool_address,
                                                    int request_limit,
                                                    struct_function_stats& stats);

#endif // DEX_TOKENS_H
