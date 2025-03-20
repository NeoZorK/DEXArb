//
//  input.cpp
//  NeoZorKDEXArb
//
//  Created by Rostyslav S. on 17.03.2025.
//
#include "input.h"          // Include input header
#include "blockchain.h"     // Include blockchain-specific functions
#include <iostream>         // Include iostream for console output
#include <fstream>          // Include fstream for file I/O
#include <sstream>          // Include stringstream for string manipulation
#include <sys/stat.h>       // Include stat for file size on Unix-like systems
#ifdef _WIN32
#include <windows.h>        // Include Windows API for file size on Windows
#endif


// Create Config File (neozork-config)
void create_config_file(int thread_count, FunctionStats& stats) {
    
    // Start timing the function
    auto start = std::chrono::high_resolution_clock::now();

    // Create and write default config file
    // Open config file for writing
    std::ofstream config_file("neozork-config");
    
    // Check if file opened successfully
    if (config_file.is_open()) {
        
        // Start JSON object
        config_file << "{\n";
        
        // Loop through blockchains
        for (const auto& chain : {"ethereum", "fantom", "bsc", "polygon", "avalanche", "solana"}) {
            
            // Start blockchain section
            config_file << "  \"" << chain << "\": {\n";
            
            // Set thread count
            config_file << "    \"threads\": " << thread_count << ",\n";
            
            // Start RPC array
            config_file << "    \"rpc\": [\n";
            
            // Get default endpoints
            auto endpoints = get_default_endpoints(string_to_blockchain(chain));
            
            // Loop through endpoints
            for (size_t i = 0; i < endpoints.size(); ++i) {
                
                // Write endpoint
                config_file << "      {\"url\": \""
                            << endpoints[i].url
                            << "\", \"limit\": "
                            << endpoints[i].request_limit
                            << ", \"active\": "
                            << endpoints[i].active
                            << "}"
                            << (i < endpoints.size() - 1 ? "," : "")
                            << "\n";
            }
            
            // Close RPC array
            config_file << "    ],\n";
            
            // Empty DEX array
            config_file << "    \"dex\": []\n";
            
            // Fix: transform chain into std::string for correct measure
            // Close blockchain section
            config_file << "  }" << (std::string(chain) != "solana" ? "," : "") << "\n";
        }
        
        // Close JSON object
        config_file << "}\n";
        
        // Close file
        config_file.close();
        
        // Report success
        std::cout << GREEN << "Created neozork-config" << RESET << '\n';

        // Update disk usage stats
#ifdef _WIN32
        // Open
        HANDLE hFile = CreateFile("neozork-config", GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);  file handle
        
        // Check if handle is valid
        if (hFile != INVALID_HANDLE_VALUE) {
            
            // Structure for file size
            LARGE_INTEGER file_size;
            
            // Get file size
            GetFileSizeEx(hFile, &file_size);
            
            // Update disk usage
            stats.disk_usage_bytes = file_size.QuadPart;
           
            // Close handle
            CloseHandle(hFile);
        }
#else
        // Structure for file stats
        struct stat file_stat;
        
        // Get file stats
        stat("neozork-config", &file_stat);
        
        // Update disk usage
        stats.disk_usage_bytes = file_stat.st_size;
#endif
    }

    // Finalize timing
    auto end = std::chrono::high_resolution_clock::now();
    
    // Calculate execution time
    stats.execution_time_ms = std::chrono::duration_cast<std::chrono::microseconds>(end - start).count() / 1000.0;
}

// Read Config File (neozork-config)
std::pair<std::vector<RpcEndpoint>, int> read_config_file(const std::string& blockchain, FunctionStats& stats) {
    
    // Start timing the function
    auto start = std::chrono::high_resolution_clock::now();
    
    // List to store RPC endpoints
    std::vector<RpcEndpoint> endpoints;
    
    // Variable to store thread count
    int thread_count = 0;

    // Open config file, create if it doesn't exist
    std::ifstream config_file("neozork-config");
    
    // Check if file NOT exists -> CREATE neozork-config file
    if (!config_file.is_open()) {
        
        // Create default config with 3 threads
        create_config_file(3, stats);
        
        // Reopen file
        config_file.open("neozork-config");
    }

    // Read file content
    // Buffer for file content
    std::stringstream buffer;
    
    // Read file into buffer
    buffer << config_file.rdbuf();
    
    // Convert to string
    std::string content = buffer.str();
    
    // Close file
    config_file.close();

    // Parse blockchain section
    // Find blockchain section
    size_t chain_pos = content.find("\"" + blockchain + "\": {");
    
    // Return empty if not found
    if (chain_pos == std::string::npos) return {endpoints, thread_count};

    
    // Find RPC array
    size_t rpc_pos = content.find("\"rpc\": [", chain_pos);
    
    // Check if RPC section exists
    if (rpc_pos != std::string::npos) {
        
        // Find end of RPC array
        size_t rpc_end = content.find("]", rpc_pos);
        
        // Move past "rpc": [
        size_t pos = rpc_pos + 8;
        
        // Loop through RPC entries
        while (pos < rpc_end) {
            
            // Find URL field
            size_t url_start = content.find("\"url\": \"", pos) + 8;
            
            // Find end of URL
            size_t url_end = content.find('"', url_start);
            
            // Extract URL
            std::string url = content.substr(url_start, url_end - url_start);
            
            // Find limit field
            size_t limit_start = content.find("\"limit\": ", url_end) + 9;
            
            // Find end of limit
            size_t limit_end = content.find_first_of(",", limit_start);
            
            // Extract limit
            int limit = std::stoi(content.substr(limit_start, limit_end - limit_start));
            
            // Find active field
            size_t active_start = content.find("\"active\": ", pos) + 10;
            
            // Find end of active
            size_t active_end = content.find_first_of("}", active_start);
            
            // Extract active string
            std::string active_str = content.substr(active_start, active_end - active_start);
            
            // Extract active value and convert to bool\
            // Remove leading and trailing spaces
            active_str.erase(0, active_str.find_first_not_of(" \t"));
            active_str.erase(active_str.find_last_not_of(" \t") + 1);
            
            // Convert to bool
            bool active;
            if (active_str == "true") {
                active = true;
            } else if (active_str == "false") {
                active = false;
            } else active = "unknown";
            
            // Add endpoint to list
            endpoints.push_back({url, limit, active});
            
            // Move to next entry
            pos = content.find("{", pos + 1);
            
            // Exit if no more entries
            if (pos == std::string::npos) break;
        }
    }
    
    // Find threads field
    size_t thread_pos = content.find("\"threads\": ", chain_pos);
    
    // Check if threads field exists
    if (thread_pos != std::string::npos) {
        
        // Move past "threads":
        size_t thread_start = thread_pos + 11;
        
        // Find end of threads value
        size_t thread_end = content.find_first_of(",\n", thread_start);
        
        // Extract thread count
        thread_count = std::stoi(content.substr(thread_start, thread_end - thread_start));
    }
    
    // Default to 3 if not specified
    if (thread_count == 0) thread_count = 3;

    // Finalize timing
    auto end = std::chrono::high_resolution_clock::now();
    
    // Calculate execution time
    stats.execution_time_ms = std::chrono::duration_cast<std::chrono::microseconds>(end - start).count() / 1000.0;
   
    // Return endpoints and thread count
    return {endpoints, thread_count};
}

// Show Scan Config
void show_config(std::string& blockchain_str) {
    
    // Open config file
    std::ifstream config_file("neozork-config");
    
    // Check if file exists
    if (!config_file.is_open()) {
        std::cerr << RED << "neozork-config not found" << RESET << '\n';
        return;
    }
    
    // Buffer for file content
    std::stringstream buffer;
    
    // Read file into buffer
    buffer << config_file.rdbuf();
    
    // Convert to string
    std::string content = buffer.str();
    
    // Close file
    config_file.close();

    // Display config settings
    // Header
    std::cout << GREEN << "neozork-config settings:" << RESET << '\n';
    
    // Loop through blockchains
    for (const auto& chain : {blockchain_str}) {
        
        // Find blockchain section
        size_t chain_pos = content.find("\"" + std::string(chain) + "\": {");
        
        // Check if section exists
        if (chain_pos != std::string::npos) {
            
            // Print blockchain name
            std::cout << YELLOW << chain << ":" << RESET << '\n';
            
            // Find threads field
            size_t thread_start = content.find("\"threads\": ", chain_pos) + 11;
            
            // Find end of threads value
            size_t thread_end = content.find_first_of(",\n", thread_start);
            
            // Extract threads
            std::string threads = content.substr(thread_start, thread_end - thread_start);
            
            // Print thread count
            std::cout << BLUE << "  Threads: " << RED << threads << RESET << '\n';

            // Find RPC array
            size_t rpc_pos = content.find("\"rpc\": [", chain_pos);
            
            // Check if RPC section exists
            if (rpc_pos != std::string::npos) {
                
                // Find end of RPC array
                size_t rpc_end = content.find("]", rpc_pos);
                
                // Move past "rpc": [
                size_t pos = rpc_pos + 8;
                
                // Loop through RPC entries
                while (pos < rpc_end) {
                    
                    // Find URL field
                    size_t url_start = content.find("\"url\": \"", pos) + 8;
                    
                    // Find end of URL
                    size_t url_end = content.find('"', url_start);
                    
                    // Extract URL
                    std::string url = content.substr(url_start, url_end - url_start);
                    
                    // Find limit field
                    size_t limit_start = content.find("\"limit\": ", url_end) + 9;
                    
                    // Find end of limit
                    size_t limit_end = content.find_first_of(",}", limit_start);
                    
                    // Extract limit
                    std::string limit = content.substr(limit_start, limit_end - limit_start);
                    
                    // Print RPC details
                    std::cout << BLUE << "  RPC URL: " << RED << url << RESET << ", " << YELLOW << "Limit: " << RED << limit << RESET << '\n';
                    
                    // Move to next entry
                    pos = content.find("{", pos + 1);
                    
                    // Exit if no more entries
                    if (pos == std::string::npos) break;
                }
            }
        }
    }
}

// Display scan results
void show_scan_results(const std::vector<DexInfo>& dex_list) {
    
    // Header
    std::cout << GREEN << "DEX found after scanning:" << RESET << '\n';
    
    // Loop through DEXes
    for (const auto& dex : dex_list) {
        
        // Print DEX details
        std::cout << YELLOW << "Name: " << dex.name << ", Address: " << dex.factory_address << ", Pools: " << dex.pool_count << RESET << '\n';
    }
}

// Open stats file for writing
void save_scan_stats(const std::vector<std::pair<std::string, FunctionStats>>& stats_list) {
    
    // Open stats file
    std::ofstream stat_file("neozork-scan-stat");
    
    // Check if file opened successfully
    if (stat_file.is_open()) {
        
        // Start JSON object
        stat_file << "{\n";
        
        // Loop through stats entries
        for (size_t i = 0; i < stats_list.size(); ++i) {
            
            // Unpack name and stats
            const auto& [name, stats] = stats_list[i];
            
            // Start function stats section
            stat_file << "  \"" << name << "\": {\n";
            
            // Write metrics:
            stat_file << "    \"execution_time_ms\": " << stats.execution_time_ms << ",\n";
            stat_file << "    \"virtual_memory_kb\": " << stats.virtual_memory_kb << ",\n";
            stat_file << "    \"disk_usage_bytes\": " << stats.disk_usage_bytes << ",\n";
            stat_file << "    \"latency_ms\": " << stats.latency_ms << ",\n";
            stat_file << "    \"cpu_usage_percent\": " << stats.cpu_usage_percent << ",\n";
            stat_file << "    \"outbound_traffic\": " << stats.outbound_traffic << ",\n";
            stat_file << "    \"inbound_traffic\": " << stats.inbound_traffic << "\n";
            
            // Close section with comma if not last
            stat_file << "  }" << (i < stats_list.size() - 1 ? "," : "") << "\n";
        }
        
        // Close JSON object
        stat_file << "}\n";
        
        // Close file
        stat_file.close();
        
        // Report success
        std::cout << GREEN << "Stats saved to neozork-scan-stat" << RESET << '\n';
    }
}

// Open stats file
void show_scan_stats() {
    
    // Open stats file
    std::ifstream stat_file("neozork-scan-stat");
    
    // Check if file exists
    if (!stat_file.is_open()) {
        std::cerr << RED << "neozork-scan-stat not found" << RESET << '\n';
        return;
    }
    
    // Buffer for file content
    std::stringstream buffer;
    
    // Read file into buffer
    buffer << stat_file.rdbuf();
    
    // Convert to string
    std::string content = buffer.str();
    
    // Close file
    stat_file.close();

    // Display scan statistics
    // Header
    std::cout << GREEN << "Scan statistics:" << RESET << '\n';
    
    // Position in content string
    size_t pos = 0;
    
    // Loop through stats objects
    while ((pos = content.find('{', pos)) != std::string::npos) {
        
        // Find function name start
        pos = content.find('"', pos);
        
        // Find function name end
        size_t end = content.find('"', pos + 1);
        
        // Extract function name
        std::string func_name = content.substr(pos + 1, end - pos - 1);
        
        // Print function name
        std::cout << YELLOW << func_name << ":" << RESET << '\n';

        // Loop through fields
        for (const auto& field : {"execution_time_ms", "virtual_memory_kb", "disk_usage_bytes", "latency_ms", "cpu_usage_percent", "outbound_traffic", "inbound_traffic"}) {
            
            // Build field key
            std::string key = "\"" + std::string(field) + "\": ";
           
            // Find field position
            size_t field_pos = content.find(key, end);
            
            // Check if field exists
            if (field_pos != std::string::npos) {
                
                // Start of value
                size_t val_start = field_pos + key.length();
                
                // End of value (comma)
                size_t val_end = content.find(',', val_start);
                
                // Use newline if no comma
                if (val_end == std::string::npos) val_end = content.find('\n', val_start);
                
                // Extract value
                std::string value = content.substr(val_start, val_end - val_start);
                
                // Print field and value
                std::cout << BLUE << "  " << field << ": " << RED << value << RESET << '\n';
            }
        }
        pos = end; // Move to next stats object
    }
}
