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
   
    // Limitations 30 req/s
    
    // Free RPC
    switch (chain) {
        case BlockchainType::Ethereum:
            return {
                {"https://rpc.ankr.com/eth", 20},
                {"https://rpc.ankr.com/eth_goerli", 30},
                {"https://rpc.ankr.com/eth_ropsten", 30},
                {"https://rpc.ankr.com/eth_rinkeby", 30},
                {"https://rpc.ankr.com/eth_kotti", 30},
                {"https://rpc.ankr.com/eth_sepolia", 30},
                {"https://rpc.ankr.com/eth_goerli", 30},
                {"https://eth.llamarpc.com", 25},
                {"https://eth-mainnet.public.blastapi.io", 30},
                {"https://eth-goerli.public.blastapi.io", 30},
                {"https://eth-ropsten.public.blastapi.io", 30},
                {"https://eth-rinkeby.public.blastapi.io", 30},
                {"https://eth-kotti.public.blastapi.io", 30},
                {"https://eth-sepolia.public.blastapi.io", 30}
            };
        case BlockchainType::Fantom:
            return {
                {"https://rpc.ftm.tools", 3},
                {"https://rpc.ankr.com/fantom", 30},// need api key
                {"https://rpc.ankr.com/fantom_testnet", 30},
                {"https://rpc.testnet.fantom.network", 30},
                {"https://fantom.publicnode.com", 3},
                {"https://fantom-mainnet-rpc.coinsdo.com",3},
                {"https://fantom-mainnet.public.blastapi.io",25},
                {"https://1rpc.io/ftm",1}, // ban 1 month if exceeded
                {"https://rpc.ankr.com/sonic",10}
            };
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

// Convert Blockchain to string
std::string blockchain_to_string(const BlockchainType& blockchain) {

    // Convert enum to string
    switch (blockchain) {
        case BlockchainType::Ethereum:
            return "Ethereum";
        case BlockchainType::Fantom:
            return "Fantom";
        case BlockchainType::BSC:
            return "BSC";
        case BlockchainType::Polygon:
            return "Polygon";
        case BlockchainType::Avalanche:
            return "Avalanche";
        case BlockchainType::Solana:
            return "Solana";
        default:
            return "Unknown";
    }
   
    };


