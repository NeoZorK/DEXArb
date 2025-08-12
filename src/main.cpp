//
//  main.cpp
//  NeoZorKDEXArb
//
//  Created by Rostyslav S. on 26.02.2025.
//
#include "../include/main.h"           // For shared structures and constants
#include "../include/utils/modern_utils.h"    // For modern logging and utilities
#include "../include/core/blockchain.h"     // For BlockchainType and functions
#include "../include/dex/dex_scanner.h"    // For find_factory_contracts
#include "../include/config/config_manager.h" // For load_dexes_from_config
#include "../include/network/queries.h"        // For query functions
#include "../include/utils/input.h"          // For input/output functions
#include "../include/cli/help_display.h"     // For help display
#include "../include/cli/command_parser.h"   // For command parsing and network ID support
#include "../include/utils/measure.h"        // For timing functions
#include <iostream>         // For console I/O
#include <algorithm>        // For std::transform
#include <vector>           // For std::vector
#include <string>           // For std::string

// Global Project Version
const std::string PROJECT_VERSION = "1.0.7";

// Global variables for stats
std::vector<std::pair<std::string, FunctionStats>> stats_list;
FunctionStats scan_stats, config_update_stats;

// Forward declarations
void show_help();
void show_version();

// Helper functions
std::vector<RpcEndpoint> load_rpc_endpoints_from_config(const std::string& blockchain) {
    // TODO: Implement actual config loading
    std::vector<RpcEndpoint> endpoints;
    if (blockchain == "fantom") {
        endpoints.push_back(RpcEndpoint("https://fantom.publicnode.com", 15));
        endpoints.push_back(RpcEndpoint("https://rpc.fantom.network", 20));
        endpoints.push_back(RpcEndpoint("https://fantom.blockpi.network/v1/rpc/public", 25));
    } else if (blockchain == "ethereum") {
        endpoints.push_back(RpcEndpoint("https://rpc.ankr.com/eth", 20));
        endpoints.push_back(RpcEndpoint("https://eth.llamarpc.com", 25));
    } else if (blockchain == "bsc") {
        endpoints.push_back(RpcEndpoint("https://bsc-dataseed.binance.org", 20));
        endpoints.push_back(RpcEndpoint("https://bsc-dataseed1.defibit.io", 25));
    } else if (blockchain == "polygon") {
        endpoints.push_back(RpcEndpoint("https://polygon-rpc.com", 20));
        endpoints.push_back(RpcEndpoint("https://rpc-mainnet.matic.network", 25));
    } else if (blockchain == "avalanche") {
        endpoints.push_back(RpcEndpoint("https://api.avax.network/ext/bc/C/rpc", 20));
        endpoints.push_back(RpcEndpoint("https://rpc.ankr.com/avalanche", 25));
    }
    return endpoints;
}

int get_thread_count_from_config() {
    // TODO: Implement actual config loading
    return 3; // Default thread count
}

// Function to display usage instructions
void show_help() {
    std::cout << "DEBUG: show_help() called" << std::endl;
    modern_utils::Logger::info("Displaying help information");
    
    std::cout << "\n";
    std::cout << GREEN << "🚀 DEX Arbitrage Scanner v" << PROJECT_VERSION << RESET << '\n';
    std::cout << "\n";
    
    std::cout << YELLOW << "📖 USAGE" << RESET << '\n';
    std::cout << "   neozork [flag] <blockchain> [value]\n\n";
    
    std::cout << YELLOW << "🌐 SUPPORTED BLOCKCHAINS" << RESET << '\n';
    std::cout << "   " << BLUE << "•" << RESET << " Ethereum    " << BLUE << "•" << RESET << " Fantom      " << BLUE << "•" << RESET << " BSC\n";
    std::cout << "   " << BLUE << "•" << RESET << " Polygon     " << BLUE << "•" << RESET << " Avalanche   " << BLUE << "•" << RESET << " Solana\n\n";
    
    std::cout << YELLOW << "🎯 COMMAND FLAGS" << RESET << '\n';
    std::cout << "   " << GREEN << "📋 Help & Info" << RESET << '\n';
    std::cout << "      " << BLUE << "-h, -help" << RESET << "     Show this help message\n";
    std::cout << "      " << BLUE << "-v, -version" << RESET << "  Show application version\n\n";
    
    std::cout << "   " << GREEN << "🔍 Scanning & Discovery" << RESET << '\n';
    std::cout << "      " << BLUE << "-scan" << RESET << "        <blockchain> <blocks>     Scan last <blocks> (1000-1000000)\n";
    std::cout << "      " << BLUE << "-showSCAN-CONFIG" << RESET << " <blockchain>           Show config settings\n";
    std::cout << "      " << BLUE << "-showSCAN" << RESET << "     <blockchain>              Show scan results\n";
    std::cout << "      " << BLUE << "-showSCAN-STAT" << RESET << " <blockchain>              Show scan stats\n\n";
    
    std::cout << "   " << GREEN << "🏦 DEX Analysis" << RESET << '\n';
    std::cout << "      " << BLUE << "-showDEXES" << RESET << "    <blockchain>              Show all DEXes\n";
    std::cout << "      " << BLUE << "-showPOOLS" << RESET << "    <blockchain> <DEX>         Show DEX pools\n";
    std::cout << "      " << BLUE << "-showTOKENS" << RESET << "   <blockchain> <DEX>         Show DEX tokens\n\n";
    
    std::cout << "   " << GREEN << "🔎 Token Search" << RESET << '\n';
    std::cout << "      " << BLUE << "-findTOKEN" << RESET << "    <blockchain> <DEX> <token> Find token in DEX\n";
    std::cout << "      " << BLUE << "-findTOKENS" << RESET << "   <blockchain> <token>       Find token across DEXes\n\n";
    
    std::cout << YELLOW << "💡 EXAMPLES" << RESET << '\n';
    std::cout << "   " << BLUE << "•" << RESET << " neozork -h                              # Show this help\n";
    std::cout << "   " << BLUE << "•" << RESET << " neozork -v                              # Show version\n";
    std::cout << "   " << BLUE << "•" << RESET << " neozork -scan ethereum 5000            # Scan Ethereum\n";
    std::cout << "   " << BLUE << "•" << RESET << " neozork -showDEXES bsc                 # Show BSC DEXes\n\n";
    
    std::cout << YELLOW << "⚡ PERFORMANCE TIPS" << RESET << '\n';
    std::cout << "   " << BLUE << "•" << RESET << " Start with small block ranges (1000-5000) for testing\n";
    std::cout << "   " << BLUE << "•" << RESET << " Use 10,000-50,000 blocks for discovery\n";
    std::cout << "   " << BLUE << "•" << RESET << " 100,000+ blocks for deep analysis\n\n";
    
    std::cout << GREEN << "🔗 Built for blockchain arbitrage opportunities" << RESET << '\n';
    std::cout << "\n";
}

// Function to display version information
void show_version() {
    modern_utils::Logger::info("Displaying version information");
    
    std::cout << "\n";
    std::cout << GREEN << "🚀 DEX Arbitrage Scanner v" << PROJECT_VERSION << RESET << '\n';
    std::cout << "\n";
    std::cout << YELLOW << "📋 BUILD INFORMATION" << RESET << '\n';
    std::cout << "   " << BLUE << "•" << RESET << " Version: " << GREEN << PROJECT_VERSION << RESET << '\n';
    std::cout << "   " << BLUE << "•" << RESET << " Purpose: Blockchain arbitrage opportunities detection\n";
    std::cout << "   " << BLUE << "•" << RESET << " Language: C++20 (Modern C++)\n";
    std::cout << "   " << BLUE << "•" << RESET << " Platform: Cross-platform\n";
    std::cout << "   " << BLUE << "•" << RESET << " Features: Modern logging, JSON parsing, Error handling\n\n";
    std::cout << GREEN << "🔗 Built for blockchain arbitrage opportunities" << RESET << '\n';
    std::cout << "\n";
}

//
//          MAIN FUNCTION
//
int main(int argc, char* argv[]) {
    std::cout << "DEBUG: main() called with argc=" << argc << std::endl;
    for (int i = 0; i < argc; i++) {
        std::cout << "DEBUG: argv[" << i << "] = '" << argv[i] << "'" << std::endl;
    }
    
    // Start the Main timer
    StartTimeMeasure();
    
    // Initialize logger
    // Logger level is set by default
    modern_utils::Logger::info("Starting DEX Arbitrage Scanner v" + PROJECT_VERSION);
   
    // Show the project version
    std::cout << "Starting DEX Arbitrage, PROJECT_VERSION: "<<PROJECT_VERSION<<"\n";
    
    
    // Check if there are any arguments
    if (argc == 1) {
        // Show help if no arguments provided (Exit)
        modern_utils::Logger::info("No arguments provided, showing help");
        cli::HelpDisplay::show_help();
        return 0;
    }
    
    // Handle special flags that don't require blockchain parameter
    if (argc == 2) {
        std::string flag(argv[1]);
        if (flag == "-help" || flag == "-h") {
            cli::HelpDisplay::show_help();
            return 0;
        } else if (flag == "-version" || flag == "-v") {
            cli::HelpDisplay::show_version();
            return 0;
        } else if (flag == "-examples") {
            cli::HelpDisplay::show_examples();
            return 0;
        } else if (flag == "-scan") {
            // Handle scan with default blockchain and block count
            modern_utils::Logger::info("Starting scan with default parameters (fantom, 1000 blocks)");
            std::string blockchain_str = "fantom";
            int scan_range = 1000;
            
            // Convert string to blockchain type
            BlockchainType blockchain;
            if (blockchain_str == "ethereum") {
                blockchain = BlockchainType::Ethereum;
            } else if (blockchain_str == "fantom") {
                blockchain = BlockchainType::Fantom;
            } else if (blockchain_str == "bsc") {
                blockchain = BlockchainType::BSC;
            } else if (blockchain_str == "polygon") {
                blockchain = BlockchainType::Polygon;
            } else if (blockchain_str == "avalanche") {
                blockchain = BlockchainType::Avalanche;
            } else if (blockchain_str == "solana") {
                blockchain = BlockchainType::Solana;
            } else {
                modern_utils::Logger::error("Unsupported blockchain: " + blockchain_str);
                std::cerr << RED << "Error: Unsupported blockchain" << RESET << '\n';
                return 1;
            }
            
            // Load configuration
            std::vector<RpcEndpoint> rpc_endpoints = load_rpc_endpoints_from_config(blockchain_str);
            if (rpc_endpoints.empty()) {
                modern_utils::Logger::error("No RPC endpoints found for " + blockchain_str);
                std::cerr << RED << "Error: No RPC endpoints configured" << RESET << '\n';
                return 1;
            }
            
            // Get thread count from config
            int thread_count = get_thread_count_from_config();
            modern_utils::Logger::info("Using " + std::to_string(thread_count) + " threads");
            
            // Announce scan
            modern_utils::Logger::info("Starting scan of " + blockchain_str + " with range " + std::to_string(scan_range));
            std::cout << GREEN << "Scanning " << blockchain_str << " with " << thread_count << " threads" << RESET << '\n';
            
            // Mutex for thread synchronization
            std::mutex mtx;
            
            // List to store found DEXes
            std::vector<DexInfo> dex_list;
            
            // Scan for factories
            find_factory_contracts(rpc_endpoints, blockchain, static_cast<uint64_t>(scan_range), thread_count, mtx, dex_list, scan_stats);
            
            // Add scan stats
            stats_list.emplace_back("find_factory_contracts", scan_stats);
            
            // Update config with results
            update_config_with_dex(rpc_endpoints, dex_list, config_update_stats);
            
            // Add update stats
            stats_list.emplace_back("update_config_with_dex", config_update_stats);
            
            // Save scan statistics
            save_scan_stats(stats_list);
            
            // Show scan results
            show_scan_results(dex_list);
            
            // End timing
            StopTimeMeasure(MICROSECONDS);
            return 0;
        } else {
            // Error for insufficient args (Exit)
            modern_utils::Logger::error("Insufficient arguments provided: " + flag);
            std::cerr << RED << "Error: Specify blockchain. Run without args for help." << RESET << '\n';
            return 1;
        }
    }
    
    if (argc < 3) {
        // Error for insufficient args (Exit)
        modern_utils::Logger::error("Insufficient arguments: " + std::to_string(argc));
        std::cerr << RED << "Error: Specify blockchain. Run without args for help." << RESET << '\n';
        return 1;
    }

    // Get the flag from arguments
    std::string flag(argv[1]);
    
    // Get the blockchain name from arguments
    std::string blockchain_str(argv[2]);
    
    modern_utils::Logger::info("Processing command: " + flag + " for blockchain: " + blockchain_str);
    
    // Use command parser to handle blockchain names and network IDs
    std::string normalized_blockchain;
    if (cli::CommandParser::is_network_id(blockchain_str)) {
        normalized_blockchain = cli::CommandParser::network_id_to_blockchain(blockchain_str);
        modern_utils::Logger::info("Network ID " + blockchain_str + " converted to: " + normalized_blockchain);
    } else {
        normalized_blockchain = blockchain_str;
    }
    
    // Convert string to blockchain type
    BlockchainType blockchain;
    if (normalized_blockchain == "ethereum") {
        blockchain = BlockchainType::Ethereum;
    } else if (normalized_blockchain == "fantom") {
        blockchain = BlockchainType::Fantom;
    } else if (normalized_blockchain == "bsc") {
        blockchain = BlockchainType::BSC;
    } else if (normalized_blockchain == "polygon") {
        blockchain = BlockchainType::Polygon;
    } else if (normalized_blockchain == "avalanche") {
        blockchain = BlockchainType::Avalanche;
    } else if (normalized_blockchain == "solana") {
        blockchain = BlockchainType::Solana;
    } else {
        modern_utils::Logger::error("Unsupported blockchain: " + blockchain_str);
        std::cerr << RED << "Error: Unsupported blockchain" << RESET << '\n';
        return 1;
    }
    
    // Load configuration
    std::vector<RpcEndpoint> rpc_endpoints = load_rpc_endpoints_from_config(normalized_blockchain);
    if (rpc_endpoints.empty()) {
        modern_utils::Logger::error("No RPC endpoints found for " + blockchain_str);
        std::cerr << RED << "Error: No RPC endpoints configured" << RESET << '\n';
        return 1;
    }
    
    // Get thread count from config
    int thread_count = get_thread_count_from_config();
    modern_utils::Logger::info("Using " + std::to_string(thread_count) + " threads");

    // Process command-line flags
    if (argc == 3) {
        if (flag == "-showSCAN-CONFIG") {
            modern_utils::Logger::info("Showing scan configuration");
            show_scan_config();
        } else if (flag == "-showSCAN-STAT") {
            modern_utils::Logger::info("Showing scan statistics");
            show_scan_stats();
        } else if (flag == "-showSCAN") {
            modern_utils::Logger::info("Loading and showing scan results");
            std::vector<DexInfo> dex_list = load_dexes_from_config();
            show_scan_results(dex_list);
        } else if (flag == "-showDEXES") {
            modern_utils::Logger::info("Showing DEXes for " + blockchain_str);
            show_dexes(rpc_endpoints);
        } else if (flag == "-showTOKENS") {
            // showTOKENS requires blockchain only (shows all tokens across all DEXes)
            modern_utils::Logger::info("Showing all tokens for " + blockchain_str);
            show_all_tokens(rpc_endpoints);
        } else {
            modern_utils::Logger::error("Invalid flag: " + flag);
            std::cerr << RED << "Invalid flag" << RESET << '\n';
            cli::HelpDisplay::show_help();
            return 1;
        }
    } else if (argc == 4) {
        if (flag == "-showPOOLS") {
            modern_utils::Logger::info("Showing pools for DEX: " + std::string(argv[3]));
            show_pools(rpc_endpoints, argv[3]);
        } else if (flag == "-showTOKENS") {
            // showTOKENS with DEX parameter shows tokens for specific DEX
            modern_utils::Logger::info("Showing tokens for DEX: " + std::string(argv[3]));
            show_tokens(rpc_endpoints, argv[3]);
        } else if (flag == "-findTOKENS") {
            modern_utils::Logger::info("Finding tokens across DEXes: " + std::string(argv[3]));
            find_tokens_across_dexes(rpc_endpoints, argv[3]);
        } else if (flag == "-scan") {
            
            // Convert scan range to integer
            int scan_range = std::stoi(argv[3]);
            if (scan_range < 1000 || scan_range > 1000000) {
                modern_utils::Logger::error("Invalid scan range: " + std::to_string(scan_range));
                std::cerr << RED << "Error: scan_range must be 1000-1000000" << RESET << '\n';
                return 1;
            }
            
            // Announce scan
            modern_utils::Logger::info("Starting scan of " + blockchain_str + " with range " + std::to_string(scan_range));
            std::cout << GREEN << "Scanning " << blockchain_str << " with " << thread_count << " threads" << RESET << '\n';
            
            // Mutex for thread synchronization
            std::mutex mtx;
            
            // List to store found DEXes
            std::vector<DexInfo> dex_list;
            
            // Scan for factories
            find_factory_contracts(rpc_endpoints, blockchain, static_cast<uint64_t>(scan_range), thread_count, mtx, dex_list, scan_stats);
            
            // Add scan stats
            stats_list.emplace_back("find_factory_contracts", scan_stats);
            
            // Update config with results - FIX: Save DEXes to config
            update_config_with_dex(rpc_endpoints, dex_list, config_update_stats);
            
            // Add update stats
            stats_list.emplace_back("update_config_with_dex", config_update_stats);
            
            // Save scan statistics
            save_scan_stats(stats_list);
            
            // Show scan results
            show_scan_results(dex_list);
            
            // End timing
            StopTimeMeasure(MICROSECONDS);
            return 0;
        } else {
            modern_utils::Logger::error("Invalid flag: " + flag);
            std::cerr << RED << "Invalid flag" << RESET << '\n';
            cli::HelpDisplay::show_help();
            return 1;
        }
    } else if (argc == 5) {
        if (flag == "-findTOKEN") {
            modern_utils::Logger::info("Finding token: " + std::string(argv[4]) + " in DEX: " + std::string(argv[3]));
            find_token_in_dex(rpc_endpoints, argv[3], argv[4]);
        } else {
            modern_utils::Logger::error("Invalid flag: " + flag);
            std::cerr << RED << "Invalid flag" << RESET << '\n';
            show_help();
            return 1;
        }
    } else {
        modern_utils::Logger::error("Too many arguments: " + std::to_string(argc));
        std::cerr << RED << "Error: Too many arguments" << RESET << '\n';
        cli::HelpDisplay::show_help();
        return 1;
    }
    
    // End timing
    StopTimeMeasure(MICROSECONDS);
    return 0;
}

