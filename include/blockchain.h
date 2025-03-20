//
//  blockchain.h
//  NeoZorKDEXArb
//
//  Created by Rostyslav S. on 18.03.2025.
//

#ifndef BLOCKCHAIN_H
#define BLOCKCHAIN_H

#include "main.h"       // Include main header for shared structures
#include <string>       // For string handling in function parameters

// Supported blockchain types as an enum
enum class BlockchainType {
    Ethereum,    // Ethereum blockchain
    Fantom,      // Fantom blockchain
    BSC,         // Binance Smart Chain
    Polygon,     // Polygon (Matic) blockchain
    Avalanche,   // Avalanche C-Chain
    Solana       // Solana blockchain
};

// Function to get default RPC endpoints for a blockchain
std::vector<struct_rpc_endpoint> get_default_endpoints(BlockchainType chain);

// Function to convert string to BlockchainType
BlockchainType string_to_blockchain(const std::string& chain);

// Function to convert BlockchainType to string
std::string blockchain_to_string(const BlockchainType& blockchain);

#endif // BLOCKCHAIN_H
