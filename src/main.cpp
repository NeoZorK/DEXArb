//
//  main.cpp
//  NeoZorKDEXArb
//
//  Created by Rostyslav S. on 26.02.2025.
//
#include "main.h"           // For shared structures and constants
#include "blockchain.h"     // For BlockchainType and functions
#include "dex_scanner.h"    // For find_factory_contracts
#include "config_manager.h" // For load_dexes_from_config
#include "queries.h"        // For query functions
#include "input.h"          // For input/output functions
#include <iostream>         // For console I/O

// Global Project Version
const std::string VERSION = "103";

// Function to display usage instructions
void show_help() {
    std::cout << GREEN << "Usage: neozork [flag] <blockchain> [value]" << RESET << '\n';
    std::cout << YELLOW << "Supported blockchains: Ethereum, Fantom, BSC, Polygon, Avalanche, Solana" << RESET << '\n';
    std::cout << YELLOW << "Flags:" << RESET << '\n';   // Introduce available flags
    std::cout << BLUE << "  -scan <blockchain> <blocks>" << RESET << ": Scan last <blocks> (1000-1000000)\n";
    std::cout << BLUE << "  -showSCAN-CONFIG <blockchain>" << RESET << ": Show config settings\n";
    std::cout << BLUE << "  -showSCAN <blockchain>" << RESET << ": Show scan results\n";
    std::cout << BLUE << "  -showSCAN-STAT <blockchain>" << RESET << ": Show scan stats\n";
    std::cout << BLUE << "  -showDEXES <blockchain>" << RESET << ": Show all DEXes\n";
    std::cout << BLUE << "  -showPOOLS <blockchain> <DEX>" << RESET << ": Show DEX pools\n";
    std::cout << BLUE << "  -showTOKENS <blockchain> <DEX>" << RESET << ": Show DEX tokens\n";
    std::cout << BLUE << "  -findTOKEN <blockchain> <DEX> <token>" << RESET << ": Find token in DEX\n";
    std::cout << BLUE << "  -findTOKENS <blockchain> <token>" << RESET << ": Find token across DEXes\n";
}

//
//          MAIN FUNCTION
//
int main(int argc, char* argv[]) {
    
    // Start the Main timer
    StartTimeMeasure();
   
    // Show the project version
    std::cout << "Starting DEX Arbitrage, VERSION: "<<VERSION<<"\n";
    
    
    // Check if there are any arguments
    if (argc == 1) {
        // Show help if no arguments provided (Exit)
        show_help();
        return 0;
    }
    if (argc < 3) {
        // Error for insufficient args (Exit)
        std::cerr << RED << "Error: Specify blockchain. Run without args for help." << RESET << '\n';
        return 1;
    }

    // Get the flag from arguments
    std::string flag(argv[1]);
    
    // Get the blockchain name from arguments
    std::string blockchain_str(argv[2]);
    
    // Convert string to blockchain type
    BlockchainType blockchain = string_to_blockchain(blockchain_str);
    
    // Check if Solana is fully supported
    if (blockchain == BlockchainType::Solana && flag != "-showSCAN-CONFIG") {
        
        // Warn about limited Solana support (Exit)
        std::cerr << RED << "Solana support is limited to config display" << RESET << '\n';
        return 1;
    }

    // Load configuration and initialize stats (List to store function stats)
    std::vector<std::pair<std::string, FunctionStats>> stats_list;
    
    // Stats for different operations
    FunctionStats config_stats, scan_stats, update_stats;
    
    // Read config file
    auto [rpc_endpoints, thread_count] = read_config_file(blockchain_str, config_stats);
    
    // Add config stats to list
    stats_list.emplace_back("read_config_file", config_stats);
    
    // Error if no endpoints loaded (Exit)
    if (rpc_endpoints.empty()) {
        std::cerr << RED << "Failed to load RPC endpoints for " << blockchain_str << RESET << '\n';
        return 1;
    }

    // Process command-line flags
    if (argc == 3) {
        if (flag == "-showSCAN-CONFIG") {
            show_scan_config();
        } else if (flag == "-showSCAN-STAT") {
            show_scan_stats();
        } else if (flag == "-showSCAN") {
            std::vector<DexInfo> dex_list = load_dexes_from_config();
            show_scan_results(dex_list);
        } else if (flag == "-showDEXES") {
            show_dexes(rpc_endpoints);
        } else {
            std::cerr << RED << "Invalid flag" << RESET << '\n';
            show_help();
            return 1;
        }
    } else if (argc == 4) {
        if (flag == "-showPOOLS") {
            show_pools(rpc_endpoints, argv[3]);
        } else if (flag == "-showTOKENS") {
            show_tokens(rpc_endpoints, argv[3]);
        } else if (flag == "-findTOKENS") {
            find_tokens_across_dexes(rpc_endpoints, argv[3]);
        } else if (flag == "-scan") {
            
            // Convert scan range to integer
            int scan_range = std::stoi(argv[3]);
            if (scan_range < 1000 || scan_range > 1000000) {
                std::cerr << RED << "Error: scan_range must be 1000-1000000" << RESET << '\n';
                return 1;
            }
            
            // Announce scan
            std::cout << GREEN << "Scanning " << blockchain_str << " with " << thread_count << " threads" << RESET << '\n';
            
            // Mutex for thread synchronization
            std::mutex mtx;
            
            // List to store found DEXes
            std::vector<DexInfo> dex_list;
            
            // Scan for factories
            find_factory_contracts(rpc_endpoints, blockchain, scan_range, thread_count, mtx, dex_list, scan_stats);
            
            // Add scan stats
            stats_list.emplace_back("find_factory_contracts", scan_stats);
            
            // Update config with results
            update_config_with_dex(rpc_endpoints, dex_list, update_stats);
            
            // Add update stats
            stats_list.emplace_back("update_config_with_dex", update_stats);
            save_scan_stats(stats_list);
            show_scan_results(dex_list);
        } else {
            // Error for invalid flag (EXIT)
            std::cerr << RED << "Invalid flag" << RESET << '\n';
            show_help();
            return 1;
        }
    } else if (argc == 5 && flag == "-findTOKEN") {
        // Find token in a specific DEX
        find_token_in_dex(rpc_endpoints, argv[3], argv[4]);
    } else {
        // Error for invalid usage (EXIT)
        std::cerr << RED << "Invalid usage" << RESET << '\n';
        show_help();
        return 1;
    }
    
    
    // Stop the Main timer
    StopTimeMeasure(MICROSECONDS);
    
    // Exit successfully
    return 0;
}

