//
//  blockchain.h
//  NeoZorKDEXArb
//
//  Created by Rostyslav S. on 18.03.2025.
//

#ifndef BLOCKCHAIN_H
#define BLOCKCHAIN_H

#include "../main.h"       // Include main header for shared structures
#include <string>       // For string handling in function parameters



// Function to get default RPC endpoints for a blockchain
std::vector<RpcEndpoint> get_default_endpoints(BlockchainType chain);

// Function to convert string to BlockchainType
BlockchainType string_to_blockchain(const std::string& chain);

#endif // BLOCKCHAIN_H
