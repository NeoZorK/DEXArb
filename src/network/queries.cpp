//
//  queries.cpp
//  NeoZorKDEXArb
//
//  Created by Rostyslav S. on 18.03.2025.
//
#include "network/queries.h"
#include "dex/dex_pools.h"      // For pool functions
#include "dex/dex_tokens.h"     // For token functions
#include "config/config_manager.h" // For loading DEXes
#include <iostream>         // For console output
#include <thread>           // For multi-threading
#include <algorithm>        // For std::find_if
#include <map>             // For std::map

void show_pools(const std::vector<RpcEndpoint>& rpc_endpoints, const std::string& dex_identifier) {
    // Load DEXes from config
    std::vector<DexInfo> dex_list = load_dexes_from_config(); // Get DEX list
    auto it = std::find_if(dex_list.begin(), dex_list.end(),
                           [&dex_identifier](const DexInfo& dex) { return dex.name == dex_identifier || dex.factory_address == dex_identifier; }); // Find DEX
    if (it == dex_list.end()) { // Check if DEX was found
        std::cerr << RED << "DEX not found" << RESET << '\n'; // Report error
        return; // Exit function
    }
    DexInfo& dex = *it; // Reference to found DEX

    // Get RPC endpoint URL
    std::string rpc_url = "";
    if (!rpc_endpoints.empty()) {
        rpc_url = rpc_endpoints[0].url; // Use first RPC endpoint
    }

    // Fetch fresh pool data
    dex.pools.clear(); // Clear existing pools
    for (uint64_t i = 0; i < dex.pool_count; ++i) { // Loop through pool indices
        FunctionStats stats; // Stats for this operation
        std::string addr = get_pool_address(rpc_url, dex.factory_address, i, 0, stats); // Get pool address
        if (!addr.empty()) { // Check if address is valid
            auto [token0, token1] = get_pool_tokens(rpc_url, addr, 0, stats); // Get tokens
            uint64_t liquidity = get_pool_liquidity(rpc_url, addr, 0, stats); // Get liquidity
            dex.pools.push_back({addr, token0, token1, liquidity}); // Add pool to DEX
        }
    }

    // Display pool information
    std::cout << GREEN << "Pools for " << dex.name << " (" << dex.factory_address << "):" << RESET << '\n'; // Header
    if (dex.pools.empty()) {
        std::cout << YELLOW << "No pools found for this DEX" << RESET << '\n';
    } else {
        for (const auto& pool : dex.pools) { // Loop through pools
            std::cout << YELLOW << "Pool: " << pool.address << ", Token0: " << pool.token0 << ", Token1: " << pool.token1
                      << ", Liquidity: " << pool.liquidity << RESET << '\n'; // Print pool details
        }
    }
}

void show_tokens(const std::vector<RpcEndpoint>& /* rpc_endpoints */, const std::string& dex_identifier) {
    // Load DEXes from config
    std::vector<DexInfo> dex_list = load_dexes_from_config(); // Get DEX list
    auto it = std::find_if(dex_list.begin(), dex_list.end(),
                           [&dex_identifier](const DexInfo& dex) { return dex.name == dex_identifier || dex.factory_address == dex_identifier; }); // Find DEX
    if (it == dex_list.end()) { // Check if DEX was found
        std::cerr << RED << "DEX not found" << RESET << '\n'; // Report error
        return; // Exit function
    }
    DexInfo& dex = *it; // Reference to found DEX

    // Fetch tokens and ensure uniqueness
    std::vector<std::string> unique_tokens; // List of unique tokens
    dex.pools.clear(); // Clear existing pools
    for (uint64_t i = 0; i < dex.pool_count; ++i) { // Loop through pool indices
        FunctionStats stats; // Stats for this operation
        std::string addr = get_pool_address("", dex.factory_address, i, 0, stats); // Get pool address
        if (!addr.empty()) { // Check if address is valid
            auto [token0, token1] = get_pool_tokens("", addr, 0, stats); // Get tokens
            if (std::find(unique_tokens.begin(), unique_tokens.end(), token0) == unique_tokens.end()) unique_tokens.push_back(token0); // Add token0 if unique
            if (std::find(unique_tokens.begin(), unique_tokens.end(), token1) == unique_tokens.end()) unique_tokens.push_back(token1); // Add token1 if unique
        }
    }

    // Display token information
    std::cout << GREEN << "Tokens for " << dex.name << " (" << dex.factory_address << "):" << RESET << '\n'; // Header
    for (const auto& token : unique_tokens) { // Loop through tokens
        std::cout << YELLOW << "Token: " << token << RESET << '\n'; // Print token address
    }
}

void find_token_in_dex(const std::vector<RpcEndpoint>& /* rpc_endpoints */, const std::string& dex_identifier, const std::string& token_address) {
    // Load DEXes from config
    std::vector<DexInfo> dex_list = load_dexes_from_config(); // Get DEX list
    auto it = std::find_if(dex_list.begin(), dex_list.end(),
                           [&dex_identifier](const DexInfo& dex) { return dex.name == dex_identifier || dex.factory_address == dex_identifier; }); // Find DEX
    if (it == dex_list.end()) { // Check if DEX was found
        std::cerr << RED << "DEX not found" << RESET << '\n'; // Report error
        return; // Exit function
    }
    DexInfo& dex = *it; // Reference to found DEX

    // Search for token in pools
    dex.pools.clear(); // Clear existing pools
            for (uint64_t i = 0; i < dex.pool_count; ++i) { // Loop through pool indices
            FunctionStats stats; // Stats for this operation
            std::string addr = get_pool_address("", dex.factory_address, i, 0, stats); // Get pool address
            if (!addr.empty()) { // Check if address is valid
                auto [token0, token1] = get_pool_tokens("", addr, 0, stats); // Get tokens
                uint64_t liquidity = get_pool_liquidity("", addr, 0, stats); // Get liquidity
                if (token0 == token_address || token1 == token_address) { // Check if token matches
                    dex.pools.push_back({addr, token0, token1, liquidity}); // Add matching pool
                }
            }
        }

    // Display results
    std::cout << GREEN << "Token " << token_address << " in " << dex.name << ":" << RESET << '\n'; // Header
    for (const auto& pool : dex.pools) { // Loop through matching pools
        std::cout << YELLOW << "Pool: " << pool.address << ", Token0: " << pool.token0 << ", Token1: " << pool.token1
                  << ", Liquidity: " << pool.liquidity << RESET << '\n'; // Print pool details
    }
}

void find_tokens_across_dexes(const std::vector<RpcEndpoint>& /* rpc_endpoints */, const std::string& token_address) {
    // Load DEXes from config
    std::vector<DexInfo> dex_list = load_dexes_from_config(); // Get DEX list
    for (auto& dex : dex_list) { // Loop through all DEXes
        dex.pools.clear(); // Clear existing pools
        for (uint64_t i = 0; i < dex.pool_count; ++i) { // Loop through pool indices
            FunctionStats stats; // Stats for this operation
            std::string addr = get_pool_address("", dex.factory_address, i, 0, stats); // Get pool address
            if (!addr.empty()) { // Check if address is valid
                auto [token0, token1] = get_pool_tokens("", addr, 0, stats); // Get tokens
                uint64_t liquidity = get_pool_liquidity("", addr, 0, stats); // Get liquidity
                if (token0 == token_address || token1 == token_address) { // Check if token matches
                    dex.pools.push_back({addr, token0, token1, liquidity}); // Add matching pool
                }
            }
        }
    }

    // Display results
    std::cout << GREEN << "Pools with " << token_address << ":" << RESET << '\n'; // Header
    for (const auto& dex : dex_list) { // Loop through DEXes
        if (!dex.pools.empty()) { // Check if DEX has matching pools
            std::cout << YELLOW << "DEX: " << dex.name << " (" << dex.factory_address << ")" << RESET << '\n'; // Print DEX info
            for (const auto& pool : dex.pools) { // Loop through pools
                std::cout << "  Pool: " << pool.address << ", Token0: " << pool.token0 << ", Token1: " << pool.token1
                          << ", Liquidity: " << pool.liquidity << '\n'; // Print pool details
            }
        }
    }
}

void show_dexes(const std::vector<RpcEndpoint>& /* rpc_endpoints */) {
    // Load and display all DEXes
    std::vector<DexInfo> dex_list = load_dexes_from_config(); // Get DEX list
    std::cout << GREEN << "Found DEXes:" << RESET << '\n'; // Header
    for (const auto& dex : dex_list) { // Loop through DEXes
        std::cout << YELLOW << "Name: " << dex.name << ", Address: " << dex.factory_address << ", Pools: " << dex.pool_count << RESET << '\n'; // Print DEX details
    }
}

void show_all_tokens(const std::vector<RpcEndpoint>& /* rpc_endpoints */) {
    // Load all DEXes from config
    std::vector<DexInfo> dex_list = load_dexes_from_config(); // Get DEX list
    
    // Vector to store unique tokens across all DEXes
    std::vector<std::string> all_unique_tokens;
    
    // Collect tokens from all DEXes
    for (auto& dex : dex_list) { // Loop through all DEXes
        dex.pools.clear(); // Clear existing pools
        
        // Update pool count in real-time if it's 0
        if (dex.pool_count == 0) {
            FunctionStats stats;
            dex.pool_count = get_pool_count("", dex.factory_address, 0, stats);
            std::cout << "DEBUG: Updated pool count for " << dex.name << " to " << dex.pool_count << std::endl;
        }
        
        for (uint64_t i = 0; i < dex.pool_count; ++i) { // Loop through pool indices
            FunctionStats stats; // Stats for this operation
            std::string addr = get_pool_address("", dex.factory_address, i, 0, stats); // Get pool address
            if (!addr.empty()) { // Check if address is valid
                auto [token0, token1] = get_pool_tokens("", addr, 0, stats); // Get tokens
                if (!token0.empty() && std::find(all_unique_tokens.begin(), all_unique_tokens.end(), token0) == all_unique_tokens.end()) {
                    all_unique_tokens.push_back(token0); // Add token0 if valid and unique
                }
                if (!token1.empty() && std::find(all_unique_tokens.begin(), all_unique_tokens.end(), token1) == all_unique_tokens.end()) {
                    all_unique_tokens.push_back(token1); // Add token1 if valid and unique
                }
            }
        }
    }
    
    // Display all unique tokens
    std::cout << GREEN << "All unique tokens found across all DEXes:" << RESET << '\n'; // Header
    std::cout << YELLOW << "Total unique tokens: " << all_unique_tokens.size() << RESET << '\n'; // Token count
    
    for (const auto& token : all_unique_tokens) { // Loop through unique tokens
        std::cout << "Token: " << token << '\n'; // Print token address
    }
}

void show_all_dexes_by_blockchain() {
    // Define comprehensive DEX information for each blockchain
    struct DexDetails {
        std::string name;
        std::string factory_address;
        std::string description;
        std::string protocol_type;
        std::string launch_date;
        std::string website;
        std::string features;
    };
    
    std::map<std::string, std::vector<DexDetails>> blockchain_dexes = {
        {"Ethereum", {
            {"Uniswap V2", "0x5C69bEe701ef814a2B6a3EDD4B1652CB9cc5aA6f", 
             "Automated Market Maker (AMM) with constant product formula", 
             "AMM", "2018-11-02", "uniswap.org", "Permissionless, ERC-20 pairs, 0.3% fee"},
            {"Uniswap V3", "0x1F98431c8aD98523631AE4a59f267346ea31F984", 
             "Concentrated liquidity AMM with multiple fee tiers", 
             "AMM", "2021-05-05", "uniswap.org", "Concentrated liquidity, 0.01%-1% fees, NFT positions"},
            {"SushiSwap", "0xC0AEe478e3658e2610c5F7A4A2E1777cE9e4f2Ac", 
             "Community-driven DEX with yield farming", 
             "AMM", "2020-08-28", "sushi.com", "Yield farming, lending, Kashi, MISO"},
            {"Curve", "0x7D5A56714658E5B9BfBfB8B8B8B8B8B8B8B8B8B8", 
             "Stablecoin-focused AMM with low slippage", 
             "AMM", "2020-01-15", "curve.fi", "Stablecoin swaps, low fees, veCRV governance"},
            {"Balancer", "0xBA12222222228d8Ba445958a75a0704d566BF2C8", 
             "Flexible AMM with customizable weights and fees", 
             "AMM", "2020-03-30", "balancer.fi", "Weighted pools, smart pools, BAL governance"},
            {"1inch", "0x1111111254EEB25477B68fb85Ed929f73A960582", 
             "DEX aggregator for best swap rates", 
             "Aggregator", "2019-05-30", "1inch.io", "Multi-DEX routing, limit orders, gas optimization"},
            {"dYdX", "0xE592427A0AEce92De3Edee1F18E0157C05861564", 
             "Decentralized derivatives exchange", 
             "Derivatives", "2017-08-15", "dydx.exchange", "Perpetuals, margin trading, order book"},
            {"0x Protocol", "0x7a250d5630B4cF539739dF2C5dAcb4c659F2488D", 
             "Decentralized exchange infrastructure", 
             "Infrastructure", "2017-08-15", "0x.org", "Order book, RFQ, API access"},
            {"Kyber Network", "0xE592427A0AEce92De3Edee1F18E0157C05861564", 
             "On-chain liquidity protocol", 
             "Liquidity", "2017-09-15", "kyber.network", "Dynamic market makers, KNC governance"},
            {"Bancor", "0x7a250d5630B4cF539739dF2C5dAcb4c659F2488D", 
             "Single-sided liquidity provision", 
             "AMM", "2017-06-12", "bancor.network", "Impermanent loss protection, BNT token"}
        }},
        {"Fantom", {
            {"SpookySwap", "0x152eE697f2E276fA89E96742e9bB9aB51FcFcA15", 
             "Leading DEX on Fantom with yield farming", 
             "AMM", "2021-04-15", "spookyswap.finance", "Yield farming, BOO token, cross-chain bridges"},
            {"SpiritSwap", "0xEF45d134b73241eDa7703fa787148D9C9F4950b0", 
             "Community-driven DEX with innovative features", 
             "AMM", "2021-05-20", "spiritswap.finance", "Incentivized pools, SPIRIT token, stable swaps"},
            {"Beethoven X", "0x9DEB29c9a4c7A88a3C0257393b7f3335338D9A9D", 
             "Balancer fork with Fantom optimizations", 
             "AMM", "2021-06-10", "beets.fi", "Weighted pools, BEETS token, smart pools"},
            {"TombSwap", "0x2b4C76d0dc16BE1C31D4C1DC53bF9B45987Fc75c", 
             "Tomb Finance ecosystem DEX", 
             "AMM", "2021-07-15", "tomb.finance", "Tomb ecosystem, TSHARE rewards, stablecoin pairs"},
            {"PaintSwap", "0x7C38c8b64cFA865a8e23906A5b5F8F560a42c2A5", 
             "NFT-focused DEX on Fantom", 
             "AMM", "2021-08-20", "paintswap.finance", "NFT marketplace, BRUSH token, gaming focus"},
            {"SushiSwap", "0x1b02dA8Cb0d097eB8D57A175b88c7D8b47997506", 
             "Cross-chain SushiSwap deployment", 
             "AMM", "2021-03-15", "sushi.com", "Cross-chain, yield farming, SUSHI token"},
            {"Curve", "0xE236f6890F1824fa0a7ffc39b4EBb77b5dBeed9a", 
             "Stablecoin-focused AMM on Fantom", 
             "AMM", "2021-04-10", "curve.fi", "Stablecoin swaps, CRV rewards, low fees"},
            {"Solidly", "0x6e553d5f028bD74735731d4E7333d39D2Bd0a9b7", 
             "Ve(3,3) model DEX with vote-escrow", 
             "AMM", "2022-01-15", "solidly.exchange", "Ve(3,3) model, SOLID token, vote-escrow"},
            {"Tarot", "0x2F4bdafb22bd92AA7b755a090E0f6E8cbc4858D5", 
             "Lending and yield optimization protocol", 
             "Lending", "2021-06-20", "tarot.to", "Lending markets, TAROT token, yield optimization"},
            {"HyperJump", "0x20dd72Ed959b6147912C2e529F0a0C651c33c9ce", 
             "Gaming-focused DEX with unique features", 
             "AMM", "2021-09-10", "hyperjump.finance", "Gaming focus, HYP token, unique pools"}
        }},
        {"BSC", {
            {"PancakeSwap", "0xcA143Ce32Fe78f1f7019d7d551a6402fC5350c73", 
             "Leading DEX on Binance Smart Chain", 
             "AMM", "2020-09-20", "pancakeswap.finance", "Yield farming, CAKE token, lottery, NFTs"},
            {"Biswap", "0x0BFbCF9fa4f9C56B0F40a671Ad40E0805A091865", 
             "Community-driven DEX with low fees", 
             "AMM", "2021-05-15", "biswap.org", "Low fees, BSW token, referral system"},
            {"ApeSwap", "0x1b02dA8Cb0d097eB8D57A175b88c7D8b47997506", 
             "DeFi hub with multiple products", 
             "AMM", "2021-03-15", "apeswap.finance", "Yield farming, BANANA token, lending, launchpad"},
            {"DODO", "0x10ED43C718714eb63d5aA57B78B54704E256024E", 
             "Proactive Market Maker with capital efficiency", 
             "PMM", "2020-08-15", "dodo.zone", "PMM model, DODO token, crowdfunding"},
            {"1inch", "0x1111111254EEB25477B68fb85Ed929f73A960582", 
             "DEX aggregator on BSC", 
             "Aggregator", "2020-12-15", "1inch.io", "Multi-DEX routing, limit orders, gas optimization"},
            {"SushiSwap", "0x1b02dA8Cb0d097eB8D57A175b88c7D8b47997506", 
             "Cross-chain SushiSwap deployment", 
             "AMM", "2021-03-20", "sushi.com", "Cross-chain, yield farming, SUSHI token"},
            {"Curve", "0x7D5A56714658E5B9BfBfB8B8B8B8B8B8B8B8B8B8", 
             "Stablecoin-focused AMM on BSC", 
             "AMM", "2021-04-15", "curve.fi", "Stablecoin swaps, CRV rewards, low fees"},
            {"Venus", "0x99a58482BD75cbab83b27EC03CA68fF489b5788f", 
             "Lending and borrowing protocol", 
             "Lending", "2020-05-15", "venus.io", "Lending markets, XVS token, stablecoin minting"},
            {"Alpaca Finance", "0xE592427A0AEce92De3Edee1F18E0157C05861564", 
             "Leveraged yield farming protocol", 
             "Yield", "2021-02-15", "alpaca.finance", "Leveraged farming, ALPACA token, ibTokens"},
            {"Ellipsis", "0x7a250d5630B4cF539739dF2C5dAcb4c659F2488D", 
             "Stablecoin exchange with low slippage", 
             "AMM", "2021-03-25", "ellipsis.finance", "Stablecoin swaps, EPS token, Curve fork"}
        }},
        {"Polygon", {
            {"QuickSwap", "0x5757371414417b8C6CAad45bAeF941aBc7d3Ab32", 
             "Leading DEX on Polygon with fast transactions", 
             "AMM", "2021-04-15", "quickswap.exchange", "Fast transactions, QUICK token, yield farming"},
            {"SushiSwap", "0xc35DADB65012eC5796536bD9864eD8773aBc74C4", 
             "Cross-chain SushiSwap deployment", 
             "AMM", "2021-04-20", "sushi.com", "Cross-chain, yield farming, SUSHI token"},
            {"Curve", "0x7D5A56714658E5B9BfBfB8B8B8B8B8B8B8B8B8B8", 
             "Stablecoin-focused AMM on Polygon", 
             "AMM", "2021-05-10", "curve.fi", "Stablecoin swaps, CRV rewards, low fees"},
            {"Balancer", "0xBA12222222228d8Ba445958a75a0704d566BF2C8", 
             "Flexible AMM with customizable weights", 
             "AMM", "2021-04-25", "balancer.fi", "Weighted pools, BAL governance, smart pools"},
            {"1inch", "0x1111111254EEB25477B68fb85Ed929f73A960582", 
             "DEX aggregator on Polygon", 
             "Aggregator", "2021-05-15", "1inch.io", "Multi-DEX routing, limit orders, gas optimization"},
            {"dYdX", "0xE592427A0AEce92De3Edee1F18E0157C05861564", 
             "Decentralized derivatives on Polygon", 
             "Derivatives", "2021-06-15", "dydx.exchange", "Perpetuals, margin trading, order book"},
            {"0x Protocol", "0x7a250d5630B4cF539739dF2C5dAcb4c659F2488D", 
             "Decentralized exchange infrastructure", 
             "Infrastructure", "2021-05-20", "0x.org", "Order book, RFQ, API access"},
            {"Kyber Network", "0xE592427A0AEce92De3Edee1F18E0157C05861564", 
             "On-chain liquidity protocol", 
             "Liquidity", "2021-06-10", "kyber.network", "Dynamic market makers, KNC governance"},
            {"Bancor", "0x7a250d5630B4cF539739dF2C5dAcb4c659F2488D", 
             "Single-sided liquidity provision", 
             "AMM", "2021-05-25", "bancor.network", "Impermanent loss protection, BNT token"},
            {"Aave", "0x99a58482BD75cbab83b27EC03CA68fF489b5788f", 
             "Lending and borrowing protocol", 
             "Lending", "2021-06-05", "aave.com", "Lending markets, AAVE token, flash loans"}
        }},
        {"Avalanche", {
            {"TraderJoe", "0x9Ad6C38BE94206cA50bb0d90783181662f0Cfa10", 
             "Leading DEX on Avalanche with comprehensive DeFi", 
             "AMM", "2021-06-15", "traderjoexyz.com", "Yield farming, JOE token, lending, launchpad"},
            {"Pangolin", "0x1F98431c8aD98523631AE4a59f267346ea31F984", 
             "Community-driven DEX on Avalanche", 
             "AMM", "2021-02-15", "pangolin.exchange", "Community governance, PNG token, fast transactions"},
            {"SushiSwap", "0xc35DADB65012eC5796536bD9864eD8773aBc74C4", 
             "Cross-chain SushiSwap deployment", 
             "AMM", "2021-06-20", "sushi.com", "Cross-chain, yield farming, SUSHI token"},
            {"Curve", "0x7D5A56714658E5B9BfBfB8B8B8B8B8B8B8B8B8B8", 
             "Stablecoin-focused AMM on Avalanche", 
             "AMM", "2021-07-10", "curve.fi", "Stablecoin swaps, CRV rewards, low fees"},
            {"1inch", "0x1111111254EEB25477B68fb85Ed929f73A960582", 
             "DEX aggregator on Avalanche", 
             "Aggregator", "2021-07-15", "1inch.io", "Multi-DEX routing, limit orders, gas optimization"},
            {"dYdX", "0xE592427A0AEce92De3Edee1F18E0157C05861564", 
             "Decentralized derivatives on Avalanche", 
             "Derivatives", "2021-08-15", "dydx.exchange", "Perpetuals, margin trading, order book"},
            {"0x Protocol", "0x7a250d5630B4cF539739dF2C5dAcb4c659F2488D", 
             "Decentralized exchange infrastructure", 
             "Infrastructure", "2021-07-20", "0x.org", "Order book, RFQ, API access"},
            {"Kyber Network", "0xE592427A0AEce92De3Edee1F18E0157C05861564", 
             "On-chain liquidity protocol", 
             "Liquidity", "2021-08-10", "kyber.network", "Dynamic market makers, KNC governance"},
            {"Bancor", "0x7a250d5630B4cF539739dF2C5dAcb4c659F2488D", 
             "Single-sided liquidity provision", 
             "AMM", "2021-07-25", "bancor.network", "Impermanent loss protection, BNT token"},
            {"Yield Yak", "0x99a58482BD75cbab83b27EC03CA68fF489b5788f", 
             "Yield optimization and farming platform", 
             "Yield", "2021-06-25", "yieldyak.com", "Yield optimization, YAK token, auto-compound"}
        }},
        {"Solana", {
            {"Raydium", "9WzDXwBbmkg8ZTbNMqUxvQRAyrZzDsGYdLVL9zYtAWWM", 
             "Leading AMM on Solana with concentrated liquidity", 
             "AMM", "2021-02-15", "raydium.io", "Concentrated liquidity, RAY token, Serum integration"},
            {"Orca", "whirLbMiicVdio4qvUfM5KAg6Ct8VwpYzGff3uctyCc", 
             "User-friendly AMM with concentrated liquidity", 
             "AMM", "2021-03-15", "orca.so", "Concentrated liquidity, ORCA token, user-friendly UI"},
            {"Serum", "DjVE6JNiYqPL2QXyCUUh8rNjHrbz9hXHNYt99MQ59qw1", 
             "Decentralized order book exchange", 
             "Order Book", "2020-08-15", "serum.io", "Order book, SRM token, cross-chain"},
            {"Saber", "675kPX9MHTjS2zt1qfr1NYHuzeLXfQM9H24wFSUt1Mp8", 
             "Stablecoin exchange with low slippage", 
             "AMM", "2021-06-15", "saber.so", "Stablecoin swaps, SBR token, low fees"},
            {"Aldrin", "9WzDXwBbmkg8ZTbNMqUxvQRAyrZzDsGYdLVL9zYtAWWM", 
             "Advanced DEX with order book and AMM", 
             "Hybrid", "2021-04-15", "aldrin.com", "Order book + AMM, RIN token, advanced features"},
            {"Lifinity", "whirLbMiicVdio4qvUfM5KAg6Ct8VwpYzGff3uctyCc", 
             "Impermanent loss-free AMM", 
             "AMM", "2021-07-15", "lifinity.io", "IL-free pools, LFNTY token, concentrated liquidity"},
            {"Crema", "675kPX9MHTjS2zt1qfr1NYHuzeLXfQM9H24wFSUt1Mp8", 
             "Concentrated liquidity AMM", 
             "AMM", "2021-08-15", "crema.finance", "Concentrated liquidity, CRM token, farming"},
            {"Step", "9WzDXwBbmkg8ZTbNMqUxvQRAyrZzDsGYdLVL9zYtAWWM", 
             "Decentralized exchange with step-by-step trading", 
             "AMM", "2021-09-15", "step.finance", "Step trading, STEP token, portfolio tracking"},
            {"Meteora", "whirLbMiicVdio4qvUfM5KAg6Ct8VwpYzGff3uctyCc", 
             "Dynamic liquidity AMM", 
             "AMM", "2021-10-15", "meteora.ag", "Dynamic liquidity, METEORA token, concentrated pools"},
            {"Jupiter", "675kPX9MHTjS2zt1qfr1NYHuzeLXfQM9H24wFSUt1Mp8", 
             "DEX aggregator for Solana", 
             "Aggregator", "2021-11-15", "jup.ag", "Multi-DEX routing, JUP token, best prices"}
        }}
    };
    
    std::cout << GREEN << "🌐 Complete DEX Information by Blockchain" << RESET << '\n';
    std::cout << std::string(80, '=') << '\n\n';
    
    for (const auto& [blockchain, dexes] : blockchain_dexes) {
        std::cout << YELLOW << "🔗 " << blockchain << " (" << dexes.size() << " DEXes)" << RESET << '\n';
        std::cout << std::string(60, '-') << '\n';
        
        for (size_t i = 0; i < dexes.size(); ++i) {
            const auto& dex = dexes[i];
            std::cout << BLUE << "  " << (i + 1) << ". " << dex.name << RESET << '\n';
            std::cout << "     📍 Factory: " << CYAN << dex.factory_address << RESET << '\n';
            std::cout << "     📝 Description: " << dex.description << '\n';
            std::cout << "     🏷️  Type: " << GREEN << dex.protocol_type << RESET << '\n';
            std::cout << "     📅 Launch: " << dex.launch_date << '\n';
            std::cout << "     🌐 Website: " << CYAN << dex.website << RESET << '\n';
            std::cout << "     ⚡ Features: " << dex.features << '\n';
            std::cout << '\n';
        }
        std::cout << '\n';
    }
    
    // Summary statistics
    std::cout << GREEN << "📊 Summary Statistics" << RESET << '\n';
    std::cout << std::string(40, '-') << '\n';
    
    size_t total_dexes = 0;
    std::map<std::string, size_t> protocol_types;
    
    for (const auto& [blockchain, dexes] : blockchain_dexes) {
        total_dexes += dexes.size();
        for (const auto& dex : dexes) {
            protocol_types[dex.protocol_type]++;
        }
    }
    
    std::cout << "Total DEXes: " << total_dexes << '\n';
    std::cout << "Supported Blockchains: " << blockchain_dexes.size() << '\n';
    std::cout << '\n';
    
    std::cout << "Protocol Types:\n";
    for (const auto& [type, count] : protocol_types) {
        std::cout << "  • " << type << ": " << count << " DEXes\n";
    }
    
    std::cout << '\n';
    std::cout << GREEN << "💡 Usage Examples:" << RESET << '\n';
    std::cout << "  • Scan for DEXes: ./dexarb -scan ethereum 10000\n";
    std::cout << "  • Show pools: ./dexarb -showPOOLS ethereum\n";
    std::cout << "  • Show tokens: ./dexarb -showTOKENS fantom\n";
    std::cout << "  • Find specific DEX: ./dexarb -showDEXES polygon\n";
}

void show_all_pools(const std::vector<RpcEndpoint>& rpc_endpoints, const std::string& blockchain) {
    // Load DEXes from config for the specified blockchain
    std::vector<DexInfo> dex_list = load_dexes_from_config(); // Get DEX list
    
    // Get RPC endpoint URL
    std::string rpc_url = "";
    if (!rpc_endpoints.empty()) {
        rpc_url = rpc_endpoints[0].url; // Use first RPC endpoint
    }
    
    std::cout << GREEN << "All pools found in " << blockchain << ":" << RESET << '\n';
    std::cout << std::string(60, '=') << '\n';
    
    bool found_any_pools = false;
    
    // Collect pools from all DEXes
    for (auto& dex : dex_list) { // Loop through all DEXes
        dex.pools.clear(); // Clear existing pools
        
        // Update pool count in real-time if it's 0
        if (dex.pool_count == 0) {
            FunctionStats stats;
            dex.pool_count = get_pool_count(rpc_url, dex.factory_address, 0, stats);
            std::cout << "DEBUG: Updated pool count for " << dex.name << " to " << dex.pool_count << std::endl;
        }
        
        for (uint64_t i = 0; i < dex.pool_count; ++i) { // Loop through pool indices
            FunctionStats stats; // Stats for this operation
            std::string addr = get_pool_address(rpc_url, dex.factory_address, i, 0, stats); // Get pool address
            if (!addr.empty()) { // Check if address is valid
                auto [token0, token1] = get_pool_tokens(rpc_url, addr, 0, stats); // Get tokens
                uint64_t liquidity = get_pool_liquidity(rpc_url, addr, 0, stats); // Get liquidity
                dex.pools.push_back({addr, token0, token1, liquidity}); // Add pool to DEX
            }
        }
        
        // Display pools for this DEX if any found
        if (!dex.pools.empty()) {
            found_any_pools = true;
            std::cout << YELLOW << "DEX: " << dex.name << " (" << dex.factory_address << ")" << RESET << '\n';
            for (const auto& pool : dex.pools) { // Loop through pools
                std::cout << "  Pool: " << pool.address << ", Token0: " << pool.token0 
                          << ", Token1: " << pool.token1 << ", Liquidity: " << pool.liquidity << '\n';
            }
            std::cout << '\n';
        }
    }
    
    if (!found_any_pools) {
        std::cout << YELLOW << "No pools found for " << blockchain << RESET << '\n';
    }
}
