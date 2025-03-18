//
//  blockchain.cpp
//  NeoZorKDEXArb
//
//  Created by Rostyslav S. on 18.03.2025.
//

#include "main.h"

std::vector<RpcEndpoint> get_default_endpoints(BlockchainType chain) {
    // Return free RPC endpoints based on blockchain type
    switch (chain) {
        case BlockchainType::Ethereum:
            return {{"https://rpc.ankr.com/eth", 20}, {"https://eth.llamarpc.com", 25}}; // Free Ethereum RPCs
        case BlockchainType::Fantom:
            return {{"https://rpc.ftm.tools", 25}, {"https://rpc.ankr.com/fantom", 30}}; // Free Fantom RPCs
        case BlockchainType::BSC:
            return {{"https://bsc-dataseed.binance.org", 50}, {"https://rpc.ankr.com/bsc", 30}}; // Free BSC RPCs
        case BlockchainType::Polygon:
            return {{"https://polygon-rpc.com", 50}, {"https://rpc.ankr.com/polygon", 30}}; // Free Polygon RPCs
        case BlockchainType::Avalanche:
            return {{"https://rpc.ankr.com/avalanche", 20}, {"https://api.avax.network/ext/bc/C/rpc", 25}}; // Free Avalanche RPCs
        case BlockchainType::Solana:
            return {{"https://api.mainnet-beta.solana.com", 100}, {"https://solana-mainnet.rpc.extrnode.com", 50}}; // Free Solana RPCs
        default:
            return {};  // Return empty vector for unsupported chains
    }
}

BlockchainType string_to_blockchain(const std::string& chain) {
    // Convert input string to lowercase for case-insensitive comparison
    std::string lower_chain = chain;
    std::transform(lower_chain.begin(), lower_chain.end(), lower_chain.begin(), ::tolower);
    // Check which blockchain matches the input
    if (lower_chain == "ethereum") return BlockchainType::Ethereum; // Match Ethereum
    if (lower_chain == "fantom") return BlockchainType::Fantom;     // Match Fantom
    if (lower_chain == "bsc") return BlockchainType::BSC;           // Match BSC
    if (lower_chain == "polygon") return BlockchainType::Polygon;   // Match Polygon
    if (lower_chain == "avalanche") return BlockchainType::Avalanche; // Match Avalanche
    if (lower_chain == "solana") return BlockchainType::Solana;     // Match Solana
    return BlockchainType::Ethereum; // Default to Ethereum if no match
}
