//
//  blockchain.cpp
//  NeoZorKDEXArb
//
//  Created by Rostyslav S. on 18.03.2025.
//

#include "blockchain.h"     // Include own header for declarations
#include <algorithm>        // For std::transform in string_to_blockchain

// Return free RPC endpoints based on blockchain type
std::vector<RpcEndpoint> get_default_endpoints(BlockchainType chain) {
   
    // Free RPC
    switch (chain) {
        case BlockchainType::Ethereum:
            return {{"https://rpc.ankr.com/eth", 20}, {"https://eth.llamarpc.com", 25}};
        case BlockchainType::Fantom:
            return {{"https://rpc.ftm.tools", 25}, {"https://rpc.ankr.com/fantom", 30}};
        case BlockchainType::BSC:
            return {{"https://bsc-dataseed.binance.org", 50}, {"https://rpc.ankr.com/bsc", 30}};
        case BlockchainType::Polygon:
            return {{"https://polygon-rpc.com", 50}, {"https://rpc.ankr.com/polygon", 30}};
        case BlockchainType::Avalanche:
            return {{"https://rpc.ankr.com/avalanche", 20}, {"https://api.avax.network/ext/bc/C/rpc", 25}};
        case BlockchainType::Solana:
            return {{"https://api.mainnet-beta.solana.com", 100}, {"https://solana-mainnet.rpc.extrnode.com", 50}};
        default:
            return {};  // Return empty vector for unsupported chains
    }
}

// Convert input string to lowercase for case-insensitive comparison
BlockchainType string_to_blockchain(const std::string& chain) {

    // Create chain
    std::string lower_chain = chain;
    
    // Convert to lowercase
    std::transform(lower_chain.begin(), lower_chain.end(), lower_chain.begin(), ::tolower);
    
    // Check which blockchain matches the input
    if (lower_chain == "ethereum") return BlockchainType::Ethereum;
    if (lower_chain == "fantom") return BlockchainType::Fantom;
    if (lower_chain == "bsc") return BlockchainType::BSC;
    if (lower_chain == "polygon") return BlockchainType::Polygon;
    if (lower_chain == "avalanche") return BlockchainType::Avalanche;
    if (lower_chain == "solana") return BlockchainType::Solana;
    return BlockchainType::Ethereum;
}
