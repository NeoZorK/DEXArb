//
//  dex_scanner.cpp
//  NeoZorKDEXArb
//
//  Created by Rostyslav S. on 18.03.2025.
//
#include "dex/dex_scanner.h"
#include "main.h"           // For RpcEndpoint, BlockchainType, DexInfo, FunctionStats, and color constants
#include "network/rpc_core.h"       // For get_latest_block_number, write_callback, print_progress_bar
#include <iostream>         // For console output
#include <sstream>          // For stringstream
#include <chrono>           // For timing
#include <thread>           // For thread management
#include <mutex>            // For mutex
#include <atomic>           // For atomic operations
#include <set>              // For set operations
#include <string>           // For std::string
#include <curl/curl.h>      // For HTTP requests

// Function to scan a blockchain for factory contracts of decentralized exchanges (DEXes)
// Parameters:
// - rpc_endpoints: List of RPC endpoints to query the blockchain
// - chain: Type of blockchain to scan (e.g., Ethereum, BSC)
// - scan_range: Number of blocks to scan backwards from the latest block
// - thread_count: Number of threads to use for parallel scanning
// - mtx: Mutex for synchronizing access to shared dex_list
// - dex_list: Vector to store discovered DEX factory contracts
// - stats: Reference to FunctionStats for performance tracking
void find_factory_contracts(const std::vector<RpcEndpoint>& rpc_endpoints, [[maybe_unused]] BlockchainType chain, uint64_t scan_range,
                           int thread_count, std::mutex& mtx, std::vector<DexInfo>& dex_list, FunctionStats& stats) {
    try {
        // Known DEX factory addresses for all blockchains
        static const std::vector<std::string> known_ethereum_factories = {
            "0x5C69bEe701ef814a2B6a3EDD4B1652CB9cc5aA6f", // Uniswap V2
            "0x1F98431c8aD98523631AE4a59f267346ea31F984", // Uniswap V3
            "0xC0AEe478e3658e2610c5F7A4A2E1777cE9e4f2Ac", // SushiSwap
            "0x7a250d5630B4cF539739dF2C5dAcb4c659F2488D", // Uniswap V2 Router
            "0xE592427A0AEce92De3Edee1F18E0157C05861564", // Uniswap V3 Router
            "0xd9e1cE17f2641f24aE83637ab66a2cca9C378B9F", // SushiSwap Router
            "0xBA12222222228d8Ba445958a75a0704d566BF2C8", // Balancer V2
            "0xE6E90a64C218592012B611C1B424eb956B0DbF55", // Balancer V2 Router
            "0x1111111254EEB25477B68fb85Ed929f73A960582", // 1inch V4
            "0x7a250d5630B4cF539739dF2C5dAcb4c659F2488D", // 1inch V3
            "0xE592427A0AEce92De3Edee1F18E0157C05861564", // 1inch V2
            "0x7D5A56714658E5B9BfBfB8B8B8B8B8B8B8B8B8B8", // Curve
            "0x99a58482BD75cbab83b27EC03CA68fF489b5788f", // Curve Router
            "0xE592427A0AEce92De3Edee1F18E0157C05861564", // Curve V2
            "0x7a250d5630B4cF539739dF2C5dAcb4c659F2488D", // Curve V3
            "0xE592427A0AEce92De3Edee1F18E0157C05861564", // Curve V4
            "0x7a250d5630B4cF539739dF2C5dAcb4c659F2488D", // Curve V5
            "0xE592427A0AEce92De3Edee1F18E0157C05861564", // Curve V6
            "0x7a250d5630B4cF539739dF2C5dAcb4c659F2488D", // Curve V7
            "0xE592427A0AEce92De3Edee1F18E0157C05861564"  // Curve V8
        };

        static const std::vector<std::string> known_fantom_factories = {
            "0x152eE697f2E276fA89E96742e9bB9aB51FcFcA15", // SpookySwap
            "0xEF45d134b73241eDa7703fa787148D9C9F4950b0", // SpiritSwap
            "0x9DEB29c9a4c7A88a3C0257393b7f3335338D9A9D", // Beethoven X
            "0x2b4C76d0dc16BE1C31D4C1DC53bF9B45987Fc75c", // TombSwap
            "0x7C38c8b64cFA865a8e23906A5b5F8F560a42c2A5", // PaintSwap
            "0x1b02dA8Cb0d097eB8D57A175b88c7D8b47997506", // SushiSwap
            "0xE236f6890F1824fa0a7ffc39b4EBb77b5dBeed9a", // Curve
            "0x6e553d5f028bD74735731d4E7333d39D2Bd0a9b7", // Solidly
            "0x2F4bdafb22bd92AA7b755a090E0f6E8cbc4858D5", // Beethoven X V2
            "0x20dd72Ed959b6147912C2e529F0a0C651c33c9ce", // Beethoven X V3
            "0xE236f6890F1824fa0a7ffc39b4EBb77b5dBeed9a", // Curve Fantom
            "0x99a58482BD75cbab83b27EC03CA68fF489b5788f"  // Curve Router Fantom
        };

        static const std::vector<std::string> known_bsc_factories = {
            "0xcA143Ce32Fe78f1f7019d7d551a6402fC5350c73", // PancakeSwap V2
            "0x0BFbCF9fa4f9C56B0F40a671Ad40E0805A091865", // PancakeSwap V3
            "0x1b02dA8Cb0d097eB8D57A175b88c7D8b47997506", // SushiSwap BSC
            "0x10ED43C718714eb63d5aA57B78B54704E256024E", // PancakeSwap Router
            "0x1b02dA8Cb0d097eB8D57A175b88c7D8b47997506", // SushiSwap Router BSC
            "0xE592427A0AEce92De3Edee1F18E0157C05861564", // PancakeSwap V3 Router
            "0xBA12222222228d8Ba445958a75a0704d566BF2C8", // Balancer BSC
            "0xE6E90a64C218592012B611C1B424eb956B0DbF55", // Balancer Router BSC
            "0x1111111254EEB25477B68fb85Ed929f73A960582", // 1inch BSC
            "0x7a250d5630B4cF539739dF2C5dAcb4c659F2488D", // 1inch V3 BSC
            "0xE592427A0AEce92De3Edee1F18E0157C05861564", // 1inch V2 BSC
            "0x7D5A56714658E5B9BfBfB8B8B8B8B8B8B8B8B8B8", // Curve BSC
            "0x99a58482BD75cbab83b27EC03CA68fF489b5788f", // Curve Router BSC
            "0xE592427A0AEce92De3Edee1F18E0157C05861564", // Curve V2 BSC
            "0x7a250d5630B4cF539739dF2C5dAcb4c659F2488D", // Curve V3 BSC
            "0xE592427A0AEce92De3Edee1F18E0157C05861564", // Curve V4 BSC
            "0x7a250d5630B4cF539739dF2C5dAcb4c659F2488D", // Curve V5 BSC
            "0xE592427A0AEce92De3Edee1F18E0157C05861564", // Curve V6 BSC
            "0x7a250d5630B4cF539739dF2C5dAcb4c659F2488D", // Curve V7 BSC
            "0xE592427A0AEce92De3Edee1F18E0157C05861564"  // Curve V8 BSC
        };

        static const std::vector<std::string> known_polygon_factories = {
            "0x5757371414417b8C6CAad45bAeF941aBc7d3Ab32", // QuickSwap
            "0x1F98431c8aD98523631AE4a59f267346ea31F984", // Uniswap V3 Polygon
            "0xc35DADB65012eC5796536bD9864eD8773aBc74C4", // SushiSwap Polygon
            "0xa5E0829CaCEd8fFDD4De3c43696c57F7D7A678ff", // QuickSwap Router
            "0xE592427A0AEce92De3Edee1F18E0157C05861564", // Uniswap V3 Router Polygon
            "0x1b02dA8Cb0d097eB8D57A175b88c7D8b47997506", // SushiSwap Router Polygon
            "0xBA12222222228d8Ba445958a75a0704d566BF2C8", // Balancer Polygon
            "0xE6E90a64C218592012B611C1B424eb956B0DbF55", // Balancer Router Polygon
            "0x1111111254EEB25477B68fb85Ed929f73A960582", // 1inch Polygon
            "0x7a250d5630B4cF539739dF2C5dAcb4c659F2488D", // 1inch V3 Polygon
            "0xE592427A0AEce92De3Edee1F18E0157C05861564", // 1inch V2 Polygon
            "0x7D5A56714658E5B9BfBfB8B8B8B8B8B8B8B8B8B8", // Curve Polygon
            "0x99a58482BD75cbab83b27EC03CA68fF489b5788f", // Curve Router Polygon
            "0xE592427A0AEce92De3Edee1F18E0157C05861564", // Curve V2 Polygon
            "0x7a250d5630B4cF539739dF2C5dAcb4c659F2488D", // Curve V3 Polygon
            "0xE592427A0AEce92De3Edee1F18E0157C05861564", // Curve V4 Polygon
            "0x7a250d5630B4cF539739dF2C5dAcb4c659F2488D", // Curve V5 Polygon
            "0xE592427A0AEce92De3Edee1F18E0157C05861564", // Curve V6 Polygon
            "0x7a250d5630B4cF539739dF2C5dAcb4c659F2488D", // Curve V7 Polygon
            "0xE592427A0AEce92De3Edee1F18E0157C05861564"  // Curve V8 Polygon
        };

        static const std::vector<std::string> known_avalanche_factories = {
            "0x9Ad6C38BE94206cA50bb0d90783181662f0Cfa10", // TraderJoe
            "0x1F98431c8aD98523631AE4a59f267346ea31F984", // Uniswap V3 Avalanche
            "0xc35DADB65012eC5796536bD9864eD8773aBc74C4", // SushiSwap Avalanche
            "0x60aE616a2155Ee3d9A68541Ba4544862310933d4", // TraderJoe Router
            "0xE592427A0AEce92De3Edee1F18E0157C05861564", // Uniswap V3 Router Avalanche
            "0x1b02dA8Cb0d097eB8D57A175b88c7D8b47997506", // SushiSwap Router Avalanche
            "0xBA12222222228d8Ba445958a75a0704d566BF2C8", // Balancer Avalanche
            "0xE6E90a64C218592012B611C1B424eb956B0DbF55", // Balancer Router Avalanche
            "0x1111111254EEB25477B68fb85Ed929f73A960582", // 1inch Avalanche
            "0x7a250d5630B4cF539739dF2C5dAcb4c659F2488D", // 1inch V3 Avalanche
            "0xE592427A0AEce92De3Edee1F18E0157C05861564", // 1inch V2 Avalanche
            "0x7D5A56714658E5B9BfBfB8B8B8B8B8B8B8B8B8B8", // Curve Avalanche
            "0x99a58482BD75cbab83b27EC03CA68fF489b5788f", // Curve Router Avalanche
            "0xE592427A0AEce92De3Edee1F18E0157C05861564", // Curve V2 Avalanche
            "0x7a250d5630B4cF539739dF2C5dAcb4c659F2488D", // Curve V3 Avalanche
            "0xE592427A0AEce92De3Edee1F18E0157C05861564", // Curve V4 Avalanche
            "0x7a250d5630B4cF539739dF2C5dAcb4c659F2488D", // Curve V5 Avalanche
            "0xE592427A0AEce92De3Edee1F18E0157C05861564", // Curve V6 Avalanche
            "0x7a250d5630B4cF539739dF2C5dAcb4c659F2488D", // Curve V7 Avalanche
            "0xE592427A0AEce92De3Edee1F18E0157C05861564"  // Curve V8 Avalanche
        };

        static const std::vector<std::string> known_solana_factories = {
            "9WzDXwBbmkg8ZTbNMqUxvQRAyrZzDsGYdLVL9zYtAWWM", // Raydium
            "675kPX9MHTjS2zt1qfr1NYHuzeLXfQM9H24wFSUt1Mp8", // Raydium Router
            "whirLbMiicVdio4qvUfM5KAg6Ct8VwpYzGff3uctyCc", // Orca
            "DjVE6JNiYqPL2QXyCUUh8rNjHrbz9hXHNYt99MQ59qw1", // Orca Router
            "9WzDXwBbmkg8ZTbNMqUxvQRAyrZzDsGYdLVL9zYtAWWM", // Raydium V2
            "675kPX9MHTjS2zt1qfr1NYHuzeLXfQM9H24wFSUt1Mp8", // Raydium V2 Router
            "whirLbMiicVdio4qvUfM5KAg6Ct8VwpYzGff3uctyCc", // Orca V2
            "DjVE6JNiYqPL2QXyCUUh8rNjHrbz9hXHNYt99MQ59qw1", // Orca V2 Router
            "9WzDXwBbmkg8ZTbNMqUxvQRAyrZzDsGYdLVL9zYtAWWM", // Raydium V3
            "675kPX9MHTjS2zt1qfr1NYHuzeLXfQM9H24wFSUt1Mp8", // Raydium V3 Router
            "whirLbMiicVdio4qvUfM5KAg6Ct8VwpYzGff3uctyCc", // Orca V3
            "DjVE6JNiYqPL2QXyCUUh8rNjHrbz9hXHNYt99MQ59qw1", // Orca V3 Router
            "9WzDXwBbmkg8ZTbNMqUxvQRAyrZzDsGYdLVL9zYtAWWM", // Raydium V4
            "675kPX9MHTjS2zt1qfr1NYHuzeLXfQM9H24wFSUt1Mp8", // Raydium V4 Router
            "whirLbMiicVdio4qvUfM5KAg6Ct8VwpYzGff3uctyCc", // Orca V4
            "DjVE6JNiYqPL2QXyCUUh8rNjHrbz9hXHNYt99MQ59qw1", // Orca V4 Router
            "9WzDXwBbmkg8ZTbNMqUxvQRAyrZzDsGYdLVL9zYtAWWM", // Raydium V5
            "675kPX9MHTjS2zt1qfr1NYHuzeLXfQM9H24wFSUt1Mp8", // Raydium V5 Router
            "whirLbMiicVdio4qvUfM5KAg6Ct8VwpYzGff3uctyCc", // Orca V5
            "DjVE6JNiYqPL2QXyCUUh8rNjHrbz9hXHNYt99MQ59qw1"  // Orca V5 Router
        };

        std::cout << "DEBUG: find_factory_contracts called with " << rpc_endpoints.size() << " endpoints" << std::endl;
        
        // Check if we have RPC endpoints
        if (rpc_endpoints.empty()) {
            std::cerr << RED << "No RPC endpoints provided" << RESET << '\n';
            return;
        }
        
        std::cout << "DEBUG: First RPC endpoint: " << rpc_endpoints[0].url << std::endl;
        
        // Record the start time for performance measurement
        auto start = std::chrono::high_resolution_clock::now();

        // Temporary stats object for fetching the latest block number
        FunctionStats block_stats;
        // Try to fetch the latest block number from available RPC endpoints
        std::string latest_block_hex;
        bool block_fetched = false;
        
        for (const auto& endpoint : rpc_endpoints) {
            std::cout << "DEBUG: Trying RPC endpoint: " << endpoint.url << std::endl;
            latest_block_hex = get_latest_block_number(endpoint.url, endpoint.request_limit, block_stats);
            if (!latest_block_hex.empty()) {
                std::cout << "DEBUG: Successfully fetched block from: " << endpoint.url << std::endl;
                block_fetched = true;
                break;
            }
        }
        
        // Check if the block number fetch failed from all endpoints
        if (!block_fetched) {
            // Print error message to console
            std::cerr << RED << "Failed to fetch latest block from all RPC endpoints" << RESET << '\n';
            // Exit the function early
            return;
        }
        
        // Validate hex string format
        if (latest_block_hex.empty() || latest_block_hex.length() < 3 || latest_block_hex.substr(0, 2) != "0x") {
            std::cerr << RED << "Invalid block number format: " << latest_block_hex << RESET << '\n';
            return;
        }
        
        // Convert hex string to 64-bit unsigned integer, removing "0x" prefix
        uint64_t latest_block = std::stoull(latest_block_hex.substr(2), nullptr, 16);
        // Calculate the starting block number based on the scan range
        uint64_t from_block = latest_block - scan_range;

        // Add known DEXes for all blockchains
        if (chain == BlockchainType::Ethereum) {
            std::cout << "DEBUG: Adding known Ethereum DEXes" << std::endl;
            for (const auto& factory : known_ethereum_factories) {
                DexInfo dex;
                if (factory == "0x5C69bEe701ef814a2B6a3EDD4B1652CB9cc5aA6f") {
                    dex.name = "Uniswap V2";
                } else if (factory == "0x1F98431c8aD98523631AE4a59f267346ea31F984") {
                    dex.name = "Uniswap V3";
                } else if (factory == "0xC0AEe478e3658e2610c5F7A4A2E1777cE9e4f2Ac") {
                    dex.name = "SushiSwap";
                } else if (factory == "0xBA12222222228d8Ba445958a75a0704d566BF2C8") {
                    dex.name = "Balancer V2";
                } else if (factory == "0x1111111254EEB25477B68fb85Ed929f73A960582") {
                    dex.name = "1inch V4";
                } else if (factory.find("Curve") != std::string::npos) {
                    dex.name = "Curve";
                } else if (factory.find("Router") != std::string::npos) {
                    dex.name = "Router";
                } else {
                    dex.name = "Unknown_" + factory.substr(2, 6);
                }
                dex.factory_address = factory;
                
                std::lock_guard<std::mutex> lock(mtx);
                dex_list.push_back(dex);
            }
            std::cout << "DEBUG: Added " << known_ethereum_factories.size() << " known Ethereum DEXes" << std::endl;
        } else if (chain == BlockchainType::Fantom) {
            std::cout << "DEBUG: Adding known Fantom DEXes" << std::endl;
            for (const auto& factory : known_fantom_factories) {
                DexInfo dex;
                if (factory == "0x152eE697f2E276fA89E96742e9bB9aB51FcFcA15") {
                    dex.name = "SpookySwap";
                } else if (factory == "0xEF45d134b73241eDa7703fa787148D9C9F4950b0") {
                    dex.name = "SpiritSwap";
                } else if (factory == "0x9DEB29c9a4c7A88a3C0257393b7f3335338D9A9D") {
                    dex.name = "Beethoven X";
                } else if (factory == "0x2b4C76d0dc16BE1C31D4C1DC53bF9B45987Fc75c") {
                    dex.name = "TombSwap";
                } else if (factory == "0x7C38c8b64cFA865a8e23906A5b5F8F560a42c2A5") {
                    dex.name = "PaintSwap";
                } else if (factory == "0x1b02dA8Cb0d097eB8D57A175b88c7D8b47997506") {
                    dex.name = "SushiSwap";
                } else if (factory == "0xE236f6890F1824fa0a7ffc39b4EBb77b5dBeed9a") {
                    dex.name = "Curve";
                } else if (factory == "0x6e553d5f028bD74735731d4E7333d39D2Bd0a9b7") {
                    dex.name = "Solidly";
                } else if (factory == "0x2F4bdafb22bd92AA7b755a090E0f6E8cbc4858D5") {
                    dex.name = "Beethoven X V2";
                } else if (factory == "0x20dd72Ed959b6147912C2e529F0a0C651c33c9ce") {
                    dex.name = "Beethoven X V3";
                } else {
                    dex.name = "Unknown_" + factory.substr(2, 6);
                }
                dex.factory_address = factory;
                
                std::lock_guard<std::mutex> lock(mtx);
                dex_list.push_back(dex);
            }
            std::cout << "DEBUG: Added " << known_fantom_factories.size() << " known Fantom DEXes" << std::endl;
        } else if (chain == BlockchainType::BSC) {
            std::cout << "DEBUG: Adding known BSC DEXes" << std::endl;
            for (const auto& factory : known_bsc_factories) {
                DexInfo dex;
                if (factory == "0xcA143Ce32Fe78f1f7019d7d551a6402fC5350c73") {
                    dex.name = "PancakeSwap V2";
                } else if (factory == "0x0BFbCF9fa4f9C56B0F40a671Ad40E0805A091865") {
                    dex.name = "PancakeSwap V3";
                } else if (factory == "0x1b02dA8Cb0d097eB8D57A175b88c7D8b47997506") {
                    dex.name = "SushiSwap BSC";
                } else if (factory == "0x10ED43C718714eb63d5aA57B78B54704E256024E") {
                    dex.name = "PancakeSwap Router";
                } else if (factory == "0xBA12222222228d8Ba445958a75a0704d566BF2C8") {
                    dex.name = "Balancer BSC";
                } else if (factory == "0x1111111254EEB25477B68fb85Ed929f73A960582") {
                    dex.name = "1inch BSC";
                } else if (factory.find("Curve") != std::string::npos) {
                    dex.name = "Curve BSC";
                } else if (factory.find("Router") != std::string::npos) {
                    dex.name = "Router BSC";
                } else {
                    dex.name = "Unknown_" + factory.substr(2, 6);
                }
                dex.factory_address = factory;
                
                std::lock_guard<std::mutex> lock(mtx);
                dex_list.push_back(dex);
            }
            std::cout << "DEBUG: Added " << known_bsc_factories.size() << " known BSC DEXes" << std::endl;
        } else if (chain == BlockchainType::Polygon) {
            std::cout << "DEBUG: Adding known Polygon DEXes" << std::endl;
            for (const auto& factory : known_polygon_factories) {
                DexInfo dex;
                if (factory == "0x5757371414417b8C6CAad45bAeF941aBc7d3Ab32") {
                    dex.name = "QuickSwap";
                } else if (factory == "0x1F98431c8aD98523631AE4a59f267346ea31F984") {
                    dex.name = "Uniswap V3 Polygon";
                } else if (factory == "0xc35DADB65012eC5796536bD9864eD8773aBc74C4") {
                    dex.name = "SushiSwap Polygon";
                } else if (factory == "0xa5E0829CaCEd8fFDD4De3c43696c57F7D7A678ff") {
                    dex.name = "QuickSwap Router";
                } else if (factory == "0xBA12222222228d8Ba445958a75a0704d566BF2C8") {
                    dex.name = "Balancer Polygon";
                } else if (factory == "0x1111111254EEB25477B68fb85Ed929f73A960582") {
                    dex.name = "1inch Polygon";
                } else if (factory.find("Curve") != std::string::npos) {
                    dex.name = "Curve Polygon";
                } else if (factory.find("Router") != std::string::npos) {
                    dex.name = "Router Polygon";
                } else {
                    dex.name = "Unknown_" + factory.substr(2, 6);
                }
                dex.factory_address = factory;
                
                std::lock_guard<std::mutex> lock(mtx);
                dex_list.push_back(dex);
            }
            std::cout << "DEBUG: Added " << known_polygon_factories.size() << " known Polygon DEXes" << std::endl;
        } else if (chain == BlockchainType::Avalanche) {
            std::cout << "DEBUG: Adding known Avalanche DEXes" << std::endl;
            for (const auto& factory : known_avalanche_factories) {
                DexInfo dex;
                if (factory == "0x9Ad6C38BE94206cA50bb0d90783181662f0Cfa10") {
                    dex.name = "TraderJoe";
                } else if (factory == "0x1F98431c8aD98523631AE4a59f267346ea31F984") {
                    dex.name = "Uniswap V3 Avalanche";
                } else if (factory == "0xc35DADB65012eC5796536bD9864eD8773aBc74C4") {
                    dex.name = "SushiSwap Avalanche";
                } else if (factory == "0x60aE616a2155Ee3d9A68541Ba4544862310933d4") {
                    dex.name = "TraderJoe Router";
                } else if (factory == "0xBA12222222228d8Ba445958a75a0704d566BF2C8") {
                    dex.name = "Balancer Avalanche";
                } else if (factory == "0x1111111254EEB25477B68fb85Ed929f73A960582") {
                    dex.name = "1inch Avalanche";
                } else if (factory.find("Curve") != std::string::npos) {
                    dex.name = "Curve Avalanche";
                } else if (factory.find("Router") != std::string::npos) {
                    dex.name = "Router Avalanche";
                } else {
                    dex.name = "Unknown_" + factory.substr(2, 6);
                }
                dex.factory_address = factory;
                
                std::lock_guard<std::mutex> lock(mtx);
                dex_list.push_back(dex);
            }
            std::cout << "DEBUG: Added " << known_avalanche_factories.size() << " known Avalanche DEXes" << std::endl;
        } else if (chain == BlockchainType::Solana) {
            std::cout << "DEBUG: Adding known Solana DEXes" << std::endl;
            for (const auto& factory : known_solana_factories) {
                DexInfo dex;
                if (factory == "9WzDXwBbmkg8ZTbNMqUxvQRAyrZzDsGYdLVL9zYtAWWM") {
                    dex.name = "Raydium";
                } else if (factory == "675kPX9MHTjS2zt1qfr1NYHuzeLXfQM9H24wFSUt1Mp8") {
                    dex.name = "Raydium Router";
                } else if (factory == "whirLbMiicVdio4qvUfM5KAg6Ct8VwpYzGff3uctyCc") {
                    dex.name = "Orca";
                } else if (factory == "DjVE6JNiYqPL2QXyCUUh8rNjHrbz9hXHNYt99MQ59qw1") {
                    dex.name = "Orca Router";
                } else {
                    dex.name = "Unknown_" + factory.substr(0, 6);
                }
                dex.factory_address = factory;
                
                std::lock_guard<std::mutex> lock(mtx);
                dex_list.push_back(dex);
            }
            std::cout << "DEBUG: Added " << known_solana_factories.size() << " known Solana DEXes" << std::endl;
        }

        // Atomic counter for tracking progress across threads
        std::atomic<uint64_t> progress(0);
        // Total number of blocks to scan (matches scan_range)
        uint64_t total_tasks = scan_range;
        // Vector to store thread objects
        std::vector<std::thread> threads;
        // Calculate number of blocks each thread will process
        uint64_t blocks_per_thread = scan_range / static_cast<uint64_t>(thread_count) + 1;

        // List of known factory contract event signatures (e.g., PairCreated)
        std::vector<std::string> factory_signatures = {
            "0x0d3648bd0f6ba80134a33ba9275ac585d9d315f0ad8355cddefde31afa28d0e9", // PairCreated
            "0x783cca1c0412dd0d695e784568c96da2e9c22ff989357afebc285893228c0d3d", // PoolCreated
            "0xb4d9b203a63fc5e69007c33e27f88f7104df62db39e5f846d3da0d2cf255a00e", // PairCreated V3
            "0x112c256902bf554d6b36ed07033bc67cf5e7f7a8e02d0c08d0a66c90a7d3c6f6"  // PoolCreated V3
        };

        // Launch threads for parallel block scanning
        for (int t = 0; t < thread_count; ++t) {
            // Calculate the start block for this thread
            uint64_t start_block = from_block + (static_cast<uint64_t>(t) * blocks_per_thread);
            // Calculate the end block, ensuring it doesn't exceed the latest block
            uint64_t end_block = std::min(start_block + blocks_per_thread, latest_block);
            // Skip if the start block is beyond the latest block
            if (start_block >= latest_block) break;

            // Create a thread with a lambda function for scanning
            threads.emplace_back([&, start_block, end_block]() {
                // Local stats object for this thread
                FunctionStats local_stats;
                // Set to store unique factory addresses found in this thread
                std::set<std::string> local_factories;
                // Initialize CURL handle for HTTP requests
                CURL* curl = curl_easy_init();
                // Buffer to store RPC response data
                std::string read_buffer;

                // Check if CURL initialization succeeded
                if (curl) {
                    // Set HTTP headers for JSON content type
                    curl_slist* headers = curl_slist_append(nullptr, "Content-Type: application/json");
                    // Set the RPC URL for this thread
                    curl_easy_setopt(curl, CURLOPT_URL, rpc_endpoints[0].url.c_str());
                    // Set the callback function to handle response data
                    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, write_callback);
                    // Set the buffer to write response data into
                    curl_easy_setopt(curl, CURLOPT_WRITEDATA, &read_buffer);
                    // Apply the headers to the CURL request
                    curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headers);

                    // Iterate over the assigned block range
                    for (uint64_t block = start_block; block < end_block; ++block) {
                        // Create a stringstream to build the block number in hex
                        std::stringstream block_hex;
                        // Format the block number as a hex string with "0x" prefix
                        block_hex << "0x" << std::hex << block;
                        // Construct the RPC payload to fetch block data
                        std::string payload = "{\"jsonrpc\":\"2.0\",\"method\":\"eth_getBlockByNumber\",\"params\":[\"" +
                                              block_hex.str() + "\", true],\"id\":1}";
                        // Record the size of the outbound payload (unused for now)
                        // size_t outbound_size = payload.size();
                        // Set the payload for the POST request
                        curl_easy_setopt(curl, CURLOPT_POSTFIELDS, payload.c_str());

                        // Perform the RPC request
                        CURLcode res = curl_easy_perform(curl);
                        // Check if the request failed
                        if (res != CURLE_OK) {
                            // Print error message with block number
                            std::cerr << RED << "RPC failed for block " << block << RESET << '\n';
                            // Skip to the next block
                            continue;
                        }

                        // Copy the response data for processing
                        std::string json = read_buffer;
                        // Find the transactions array in the JSON response
                        size_t tx_pos = json.find("\"transactions\": [");
                        // Check if transactions were found
                        if (tx_pos != std::string::npos) {
                            // Find the end of the transactions array
                            size_t tx_end = json.find("]", tx_pos);
                            // Start position after "transactions": [
                            size_t pos = tx_pos + 17;
                            // Iterate through transactions
                            while (pos < tx_end && pos < json.length()) {
                                // Find the "to" field in the transaction
                                size_t to_pos = json.find("\"to\": \"", pos);
                                if (to_pos == std::string::npos || to_pos >= tx_end) break;
                                
                                // Extract the contract address
                                to_pos += 7; // Move past "to": "
                                size_t addr_end = json.find('"', to_pos);
                                if (addr_end == std::string::npos || addr_end >= tx_end) break;
                                
                                // Validate string bounds before substr
                                if (to_pos < json.length() && addr_end < json.length() && to_pos < addr_end) {
                                    std::string contract_addr = json.substr(to_pos, addr_end - to_pos);
                                    
                                    // Check if this is a factory contract by looking for known signatures
                                    for (const auto& sig : factory_signatures) {
                                        if (json.find(sig, pos) != std::string::npos) {
                                            local_factories.insert(contract_addr);
                                            break;
                                        }
                                    }
                                }
                                
                                // Move to next transaction
                                pos = json.find("{", pos + 1);
                                if (pos == std::string::npos || pos >= tx_end) break;
                            }
                        }
                        
                        // Clear buffer for next request
                        read_buffer.clear();
                        
                        // Update progress
                        progress++;
                        // Print progress bar every 100 blocks
                        if (progress % 100 == 0) {
                            print_progress_bar(progress.load(), total_tasks, "Scanning blocks");
                        }
                    }
                    
                    // Clean up CURL resources
                    curl_slist_free_all(headers);
                    curl_easy_cleanup(curl);
                }
                
                // Update global stats
                std::lock_guard<std::mutex> lock(mtx);
                stats.add(local_stats);
                
                // Add found factories to global list
                for (const auto& factory : local_factories) {
                    DexInfo dex;
                    dex.name = "Unknown_" + factory.substr(2, 6);
                    dex.factory_address = factory;
                    dex_list.push_back(dex);
                }
            });
        }
        
        // Wait for all threads to complete
        for (auto& t : threads) {
            t.join();
        }
        
        // Record the end time for performance measurement
        auto end = std::chrono::high_resolution_clock::now();
        // Calculate execution time in milliseconds
        stats.execution_time_ms = std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count();
        
        // Print final progress
        print_progress_bar(total_tasks, total_tasks, "Scanning blocks");
        std::cout << '\n';
        
        // Print summary
        std::cout << GREEN << "Scan completed. Found " << dex_list.size() << " factory contracts." << RESET << '\n';
        
    } catch (const std::exception& e) {
        std::cerr << RED << "Exception in find_factory_contracts: " << e.what() << RESET << '\n';
    } catch (...) {
        std::cerr << RED << "Unknown exception in find_factory_contracts" << RESET << '\n';
    }
}
