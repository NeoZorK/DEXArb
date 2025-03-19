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
                config_file << "      {\"url\": \"" << endpoints[i].url << "\", \"limit\": " << endpoints[i].request_limit << "}"
                            << (i < endpoints.size() - 1 ? "," : "") << "\n";
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
            size_t limit_end = content.find_first_of(",}", limit_start);
            
            // Extract limit
            int limit = std::stoi(content.substr(limit_start, limit_end - limit_start));
            
            // Add endpoint to list
            endpoints.push_back({url, limit});
            
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
void show_scan_config() {
    
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
    for (const auto& chain : {"ethereum", "fantom", "bsc", "polygon", "avalanche", "solana"}) {
        size_t chain_pos = content.find("\"" + std::string(chain) + "\": {"); // Find blockchain section
        if (chain_pos != std::string::npos) { // Check if section exists
            std::cout << YELLOW << chain << ":" << RESET << '\n'; // Print blockchain name
            size_t thread_start = content.find("\"threads\": ", chain_pos) + 11; // Find threads field
            size_t thread_end = content.find_first_of(",\n", thread_start); // Find end of threads value
            std::string threads = content.substr(thread_start, thread_end - thread_start); // Extract threads
            std::cout << BLUE << "  Threads: " << RED << threads << RESET << '\n'; // Print thread count

            size_t rpc_pos = content.find("\"rpc\": [", chain_pos); // Find RPC array
            if (rpc_pos != std::string::npos) { // Check if RPC section exists
                size_t rpc_end = content.find("]", rpc_pos); // Find end of RPC array
                size_t pos = rpc_pos + 8; // Move past "rpc": [
                while (pos < rpc_end) { // Loop through RPC entries
                    size_t url_start = content.find("\"url\": \"", pos) + 8; // Find URL field
                    size_t url_end = content.find('"', url_start); // Find end of URL
                    std::string url = content.substr(url_start, url_end - url_start); // Extract URL
                    size_t limit_start = content.find("\"limit\": ", url_end) + 9; // Find limit field
                    size_t limit_end = content.find_first_of(",}", limit_start); // Find end of limit
                    std::string limit = content.substr(limit_start, limit_end - limit_start); // Extract limit
                    std::cout << BLUE << "  RPC URL: " << RED << url << RESET << ", " << YELLOW << "Limit: " << RED << limit << RESET << '\n'; // Print RPC details
                    pos = content.find("{", pos + 1); // Move to next entry
                    if (pos == std::string::npos) break; // Exit if no more entries
                }
            }
        }
    }
}

void show_scan_results(const std::vector<DexInfo>& dex_list) {
    // Display scan results
    std::cout << GREEN << "DEX found after scanning:" << RESET << '\n'; // Header
    for (const auto& dex : dex_list) { // Loop through DEXes
        std::cout << YELLOW << "Name: " << dex.name << ", Address: " << dex.factory_address << ", Pools: " << dex.pool_count << RESET << '\n'; // Print DEX details
    }
}

void save_scan_stats(const std::vector<std::pair<std::string, FunctionStats>>& stats_list) {
    // Open stats file for writing
    std::ofstream stat_file("neozork-scan-stat"); // Open stats file
    if (stat_file.is_open()) { // Check if file opened successfully
        stat_file << "{\n"; // Start JSON object
        for (size_t i = 0; i < stats_list.size(); ++i) { // Loop through stats entries
            const auto& [name, stats] = stats_list[i]; // Unpack name and stats
            stat_file << "  \"" << name << "\": {\n"; // Start function stats section
            stat_file << "    \"execution_time_ms\": " << stats.execution_time_ms << ",\n"; // Write execution time
            stat_file << "    \"virtual_memory_kb\": " << stats.virtual_memory_kb << ",\n"; // Write virtual memory
            stat_file << "    \"disk_usage_bytes\": " << stats.disk_usage_bytes << ",\n"; // Write disk usage
            stat_file << "    \"latency_ms\": " << stats.latency_ms << ",\n"; // Write latency
            stat_file << "    \"cpu_usage_percent\": " << stats.cpu_usage_percent << ",\n"; // Write CPU usage
            stat_file << "    \"outbound_traffic\": " << stats.outbound_traffic << ",\n"; // Write outbound traffic
            stat_file << "    \"inbound_traffic\": " << stats.inbound_traffic << "\n"; // Write inbound traffic
            stat_file << "  }" << (i < stats_list.size() - 1 ? "," : "") << "\n"; // Close section with comma if not last
        }
        stat_file << "}\n"; // Close JSON object
        stat_file.close(); // Close file
        std::cout << GREEN << "Stats saved to neozork-scan-stat" << RESET << '\n'; // Report success
    }
}

void show_scan_stats() {
    // Open stats file
    std::ifstream stat_file("neozork-scan-stat"); // Open stats file
    if (!stat_file.is_open()) { // Check if file exists
        std::cerr << RED << "neozork-scan-stat not found" << RESET << '\n'; // Report error
        return; // Exit function
    }
    std::stringstream buffer; // Buffer for file content
    buffer << stat_file.rdbuf(); // Read file into buffer
    std::string content = buffer.str(); // Convert to string
    stat_file.close(); // Close file

    // Display scan statistics
    std::cout << GREEN << "Scan statistics:" << RESET << '\n'; // Header
    size_t pos = 0; // Position in content string
    while ((pos = content.find('{', pos)) != std::string::npos) { // Loop through stats objects
        pos = content.find('"', pos); // Find function name start
        size_t end = content.find('"', pos + 1); // Find function name end
        std::string func_name = content.substr(pos + 1, end - pos - 1); // Extract function name
        std::cout << YELLOW << func_name << ":" << RESET << '\n'; // Print function name

        for (const auto& field : {"execution_time_ms", "virtual_memory_kb", "disk_usage_bytes", "latency_ms", "cpu_usage_percent", "outbound_traffic", "inbound_traffic"}) { // Loop through fields
            std::string key = "\"" + std::string(field) + "\": "; // Build field key
            size_t field_pos = content.find(key, end); // Find field position
            if (field_pos != std::string::npos) { // Check if field exists
                size_t val_start = field_pos + key.length(); // Start of value
                size_t val_end = content.find(',', val_start); // End of value (comma)
                if (val_end == std::string::npos) val_end = content.find('\n', val_start); // Use newline if no comma
                std::string value = content.substr(val_start, val_end - val_start); // Extract value
                std::cout << BLUE << "  " << field << ": " << RED << value << RESET << '\n'; // Print field and value
            }
        }
        pos = end; // Move to next stats object
    }
}
