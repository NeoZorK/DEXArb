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
std::vector<DexInfo> load_dexes_from_config();

// Function to update config with DEX data
void update_config_with_dex(const std::vector<RpcEndpoint>& rpc_endpoints, std::vector<DexInfo>& dex_list, FunctionStats& stats);

#endif // CONFIG_MANAGER_H
