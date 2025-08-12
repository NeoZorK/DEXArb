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
#include <fstream>          // For file operations
#include <sstream>          // For stringstream

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
    std::vector<RpcEndpoint> endpoints;
    
    // Read config file
    std::ifstream config_file("neozork-config");
    if (!config_file.is_open()) {
        std::cerr << "Error: Could not open neozork-config file" << std::endl;
        return endpoints;
    }
    
    std::stringstream buffer;
    buffer << config_file.rdbuf();
    std::string content = buffer.str();
    config_file.close();
    
    // Find blockchain section
    std::string blockchain_section = "\"" + blockchain + "\":";
    size_t blockchain_pos = content.find(blockchain_section);
    if (blockchain_pos == std::string::npos) {
        std::cerr << "Error: Blockchain " << blockchain << " not found in config" << std::endl;
        return endpoints;
    }
    
    // Find RPC section within blockchain
    size_t rpc_pos = content.find("\"rpc\": [", blockchain_pos);
    if (rpc_pos == std::string::npos) {
        std::cerr << "Error: RPC section not found for " << blockchain << std::endl;
        return endpoints;
    }
    
    // Parse RPC endpoints
    size_t rpc_end = content.find("]", rpc_pos);
    size_t pos = rpc_pos + 8; // Move past "rpc": [
    
    while (pos < rpc_end) {
        size_t url_start = content.find("\"url\": \"", pos) + 8;
        size_t url_end = content.find('"', url_start);
        std::string url = content.substr(url_start, url_end - url_start);
        
        size_t limit_start = content.find("\"limit\": ", url_end) + 9;
        size_t limit_end = content.find_first_of(",}", limit_start);
        std::string limit_str = content.substr(limit_start, limit_end - limit_start);
        
        try {
            int limit = std::stoi(limit_str);
            endpoints.push_back(RpcEndpoint(url, limit));
            std::cout << "DEBUG: Loaded RPC endpoint: " << url << " with limit: " << limit << std::endl;
        } catch (const std::exception& e) {
            std::cerr << "Error parsing limit for " << url << ": " << e.what() << std::endl;
        }
        
        pos = content.find("{", pos + 1);
        if (pos == std::string::npos) break;
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
        } else if (flag == "-dexes") {
            modern_utils::Logger::info("Showing all known DEXes by blockchain");
            show_all_dexes_by_blockchain();
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
            update_config_with_dex(rpc_endpoints, blockchain_str, dex_list, config_update_stats);
            
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
    
    // Parse command using the new CommandParser
    auto cmd = cli::CommandParser::parse(argc, const_cast<const char**>(argv));
    
    if (!cmd.is_valid) {
        modern_utils::Logger::error("Invalid command: " + cmd.error_message);
        cli::HelpDisplay::show_error(cmd.error_message);
        cli::HelpDisplay::show_help();
        return 1;
    }
    
    // Handle commands that don't require blockchain
    if (cmd.type == cli::CommandType::HELP) {
        cli::HelpDisplay::show_help();
        return 0;
    }
    
    if (cmd.type == cli::CommandType::VERSION_CMD) {
        cli::HelpDisplay::show_version();
        return 0;
    }
    
    if (cmd.type == cli::CommandType::EXAMPLES) {
        cli::HelpDisplay::show_examples();
        return 0;
    }
    
    if (cmd.type == cli::CommandType::SHOW_ALL_DEXES) {
        modern_utils::Logger::info("Showing all known DEXes by blockchain");
        show_all_dexes_by_blockchain();
        return 0;
    }
    
    // For other commands, check if blockchain is required
    if (cmd.blockchain.empty()) {
        modern_utils::Logger::error("Blockchain required for command: " + cmd.flag);
        std::cerr << RED << "Error: Specify blockchain. Run without args for help." << RESET << '\n';
        return 1;
    }

    modern_utils::Logger::info("Processing command: " + cmd.flag + " for blockchain: " + cmd.blockchain);
    
    // Use normalized blockchain from command parser
    std::string normalized_blockchain = cmd.blockchain;
    
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
        modern_utils::Logger::error("Unsupported blockchain: " + normalized_blockchain);
        std::cerr << RED << "Error: Unsupported blockchain" << RESET << '\n';
        return 1;
    }
    
    // Load configuration
    std::vector<RpcEndpoint> rpc_endpoints = load_rpc_endpoints_from_config(normalized_blockchain);
    if (rpc_endpoints.empty()) {
        modern_utils::Logger::error("No RPC endpoints found for " + normalized_blockchain);
        std::cerr << RED << "Error: No RPC endpoints configured" << RESET << '\n';
        return 1;
    }
    
    // Get thread count from config
    int thread_count = get_thread_count_from_config();
    modern_utils::Logger::info("Using " + std::to_string(thread_count) + " threads");

    // Process command using the parsed command structure
    switch (cmd.type) {
        case cli::CommandType::SCAN: {
            // Handle scan command
            int scan_range = cmd.value.empty() ? 1000 : std::stoi(cmd.value);
            if (scan_range < 1000 || scan_range > 1000000) {
                modern_utils::Logger::error("Invalid scan range: " + std::to_string(scan_range));
                std::cerr << RED << "Error: scan_range must be 1000-1000000" << RESET << '\n';
                return 1;
            }
            
            // Announce scan
            modern_utils::Logger::info("Starting scan of " + normalized_blockchain + " with range " + std::to_string(scan_range));
            std::cout << GREEN << "Scanning " << normalized_blockchain << " with " << thread_count << " threads" << RESET << '\n';
            
            // Mutex for thread synchronization
            std::mutex mtx;
            
            // List to store found DEXes
            std::vector<DexInfo> dex_list;
            
            // Scan for factories
            find_factory_contracts(rpc_endpoints, blockchain, static_cast<uint64_t>(scan_range), thread_count, mtx, dex_list, scan_stats);
            
            // Add scan stats
            stats_list.emplace_back("find_factory_contracts", scan_stats);
            
            // Update config with results
            update_config_with_dex(rpc_endpoints, normalized_blockchain, dex_list, config_update_stats);
            
            // Add update stats
            stats_list.emplace_back("update_config_with_dex", config_update_stats);
            
            // Save scan statistics
            save_scan_stats(stats_list);
            
            // Show scan results
            show_scan_results(dex_list);
            break;
        }
        
        case cli::CommandType::SHOW_SCAN_CONFIG: {
            modern_utils::Logger::info("Showing scan configuration");
            show_scan_config();
            break;
        }
        
        case cli::CommandType::SHOW_SCAN_STAT: {
            modern_utils::Logger::info("Showing scan statistics");
            show_scan_stats();
            break;
        }
        
        case cli::CommandType::SHOW_SCAN: {
            modern_utils::Logger::info("Loading and showing scan results");
            std::vector<DexInfo> dex_list = load_dexes_from_config();
            show_scan_results(dex_list);
            break;
        }
        
        case cli::CommandType::SHOW_DEXES: {
            modern_utils::Logger::info("Showing DEXes for " + normalized_blockchain);
            show_dexes(rpc_endpoints);
            break;
        }
        
        case cli::CommandType::SHOW_ALL_DEXES: {
            modern_utils::Logger::info("Showing all known DEXes by blockchain");
            show_all_dexes_by_blockchain();
            break;
        }
        
        case cli::CommandType::SHOW_POOLS: {
            if (cmd.dex_name.empty()) {
                // showPOOLS without DEX parameter shows all pools across all DEXes
                modern_utils::Logger::info("Showing all pools for " + normalized_blockchain);
                show_all_pools(rpc_endpoints, normalized_blockchain);
            } else {
                // showPOOLS with DEX parameter shows pools for specific DEX
                modern_utils::Logger::info("Showing pools for DEX: " + cmd.dex_name);
                show_pools(rpc_endpoints, cmd.dex_name);
            }
            break;
        }
        
        case cli::CommandType::SHOW_TOKENS: {
            if (cmd.dex_name.empty()) {
                // showTOKENS without DEX parameter shows all tokens across all DEXes
                modern_utils::Logger::info("Showing all tokens for " + normalized_blockchain);
                show_all_tokens(rpc_endpoints);
            } else {
                // showTOKENS with DEX parameter shows tokens for specific DEX
                modern_utils::Logger::info("Showing tokens for DEX: " + cmd.dex_name);
                show_tokens(rpc_endpoints, cmd.dex_name);
            }
            break;
        }
        
        case cli::CommandType::FIND_TOKEN: {
            modern_utils::Logger::info("Finding token: " + cmd.token_address + " in DEX: " + cmd.dex_name);
            find_token_in_dex(rpc_endpoints, cmd.dex_name, cmd.token_address);
            break;
        }
        
        case cli::CommandType::FIND_TOKENS: {
            modern_utils::Logger::info("Finding tokens across DEXes: " + cmd.token_address);
            find_tokens_across_dexes(rpc_endpoints, cmd.token_address);
            break;
        }
        
        default: {
            modern_utils::Logger::error("Unsupported command type: " + cmd.flag);
            std::cerr << RED << "Error: Unsupported command" << RESET << '\n';
            cli::HelpDisplay::show_help();
            return 1;
        }
    }
    
    // End timing
    StopTimeMeasure(MICROSECONDS);
    return 0;
}

