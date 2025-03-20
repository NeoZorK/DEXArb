//
//  config_manager.h
//  NeoZorKDEXArb
//
//  Created by Rostyslav S. on 18.03.2025.
//

#ifndef CONFIG_MANAGER_H
#define CONFIG_MANAGER_H

#include "main.h"       // For DexInfo, RpcEndpoint, FunctionStats

// Function to load DEXes from config file
std::vector<struct_dex_info> load_dexes_from_config();

// Function to update config with DEX data
void update_config_with_dex(const std::vector<struct_rpc_endpoint>& rpc_endpoints,
                            std::vector<struct_dex_info>& dex_list,
                            struct_function_stats& stats);

#endif // CONFIG_MANAGER_H
