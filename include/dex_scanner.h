//
//  dex_scanner.h
//  NeoZorKDEXArb
//
//  Created by Rostyslav S. on 18.03.2025.
//

#ifndef DEX_SCANNER_H
#define DEX_SCANNER_H

#include "main.h"       // For RpcEndpoint, DexInfo, FunctionStats
#include "blockchain.h" // For BlockchainType
#include <mutex>        // For thread synchronization

// Function to scan a blockchain for factory contracts of decentralized exchanges (DEXes)
// Parameters:
// - rpc_endpoints: List of RPC endpoints to query the blockchain
// - chain: Type of blockchain to scan (e.g., Ethereum, BSC)
// - scan_range: Number of blocks to scan backwards from the latest block
// - thread_count: Number of threads to use for parallel scanning
// - mtx: Mutex for synchronizing access to shared dex_list
// - dex_list: Vector to store discovered DEX factory contracts
// - stats: Reference to FunctionStats for performance tracking
void find_factory_contracts(const std::vector<RpcEndpoint>& rpc_endpoints, BlockchainType chain, uint64_t scan_range,
                            int thread_count, std::mutex& mtx, std::vector<DexInfo>& dex_list, FunctionStats& stats);

#endif // DEX_SCANNER_H
