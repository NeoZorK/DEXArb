//
//  dex_scanner.h
//  NeoZorKDEXArb
//
//  Created by Rostyslav S. on 18.03.2025.
//

#ifndef DEX_SCANNER_H
#define DEX_SCANNER_H

#include "main.h"       // Include main header for shared structures
#include "blockchain.h" // For BlockchainType
#include <mutex>        // For thread synchronization

// Function to scan blockchain for factory contracts
void find_factory_contracts(const std::vector<RpcEndpoint>& rpc_endpoints, BlockchainType chain, uint64_t scan_range,
                            int thread_count, std::mutex& mtx, std::vector<DexInfo>& dex_list, FunctionStats& stats);

#endif // DEX_SCANNER_H
