//
//  rpc_core.cpp
//  NeoZorKDEXArb
//
//  Created by Rostyslav S. on 18.03.2025.
//
#include "main.h"
#include "rpc_core.h"       // Include own header for declarations
#include "measure.h"        // For update_stats function
#include <curl/curl.h>      // For CURL HTTP requests
#include <iostream>         // For console output
#include <thread>           // For std::this_thread::sleep_for
#include <fstream>
#include <string>
#include <vector>
#include <chrono>
#include <curl/curl.h>

// Callback function to write CURL response data and update progress bar
size_t WriteCallback(void* contents, size_t size, size_t nmemb, std::string* userp) {
    size_t realsize = size * nmemb;
    userp->append(static_cast<char*>(contents), realsize);

    static size_t total_downloaded = 0;
    total_downloaded += realsize;
    static size_t last_progress = 0;
    size_t progress = (total_downloaded * 100) / 102400; // Rough estimate (assume ~100KB file size)
    if (progress > last_progress && progress <= 100) {
        last_progress = progress;
        std::cout << "\r" << YELLOW << "Downloading chainlist-config: [";
        for (size_t i = 0; i < 50; ++i) {
            std::cout << (i < progress / 2 ? (std::string(GREEN) + "#") : " ");
        }
        std::cout << "] " << progress << "%" << RESET << std::flush;
    }
    return realsize;
}

// Function to fetch RPC/WSS from chainlist.org and update neozork-config
void update1(const std::string& blockchain) {
    // Start downloading
    auto start_download = std::chrono::high_resolution_clock::now();

    // Initialize CURL
    CURL* curl = curl_easy_init();
    if (!curl) {
        std::cerr << RED << "Failed to initialize CURL" << RESET << "\n";
        return;
    }

    // URL for rpcs.json from chainlist.org
    const std::string url = "https://chainlist.org/rpcs.json";
    std::string response;

    // Set CURL options
    curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteCallback);
    curl_easy_setopt(curl, CURLOPT_WRITEDATA, &response);
    curl_easy_setopt(curl, CURLOPT_USERAGENT, "Mozilla/5.0");

    // Perform CURL request
    CURLcode res = curl_easy_perform(curl);
    if (res != CURLE_OK) {
        std::cerr << RED << "CURL request failed: " << curl_easy_strerror(res) << RESET << "\n";
        curl_easy_cleanup(curl);
        return;
    }
    
    // Cleanup CURL
    curl_easy_cleanup(curl);
    std::cout << "\n"; // Newline after progress bar
    
    // Save downloaded content to chainlist-config
    std::ofstream chainlist_file("chainlist-config");
    if (!chainlist_file.is_open()) {
        std::cerr << RED << "Failed to create chainlist-config" << RESET << "\n";
        return;
    }
    
    
    chainlist_file << response;
    chainlist_file.close();

    auto end_download = std::chrono::high_resolution_clock::now();
    auto download_time = std::chrono::duration_cast<std::chrono::milliseconds>(end_download - start_download).count();

    std::cerr << GREEN << "CURL request performed: " << RESET << "\n";
    std::cerr << GREEN << "Created chainlist-config"  << RESET << "\n";
    
    // Start parsing
    auto start_parse = std::chrono::high_resolution_clock::now();

    // Open neozork-config for reading and writing
    std::fstream file("neozork-config", std::ios::in | std::ios::out);
    if (!file.is_open()) {
        std::cerr << RED << "Error: neozork-config not found" << RESET << "\n";
        return;
    } else     std::cerr << GREEN << "Opened neozork-config" << RESET << "\n";


    // Read neozork-config content
    std::string config_content((std::istreambuf_iterator<char>(file)), std::istreambuf_iterator<char>());

    // Find blockchain section in config
    size_t chain_pos = config_content.find("\"" + blockchain + "\": {");
    if (chain_pos == std::string::npos) {
        std::cerr << RED << "Blockchain " << blockchain << " not found in neozork-config" << RESET << "\n";
        file.close();
        return;
    } else std::cerr << GREEN << "Blockchain " << blockchain << " Found in neozork-config" << RESET << "\n";

    // Find RPC array in config
    size_t rpc_pos = config_content.find("\"rpc\": [", chain_pos);
    size_t rpc_end = config_content.find("]", rpc_pos);
    std::string rpc_section = config_content.substr(rpc_pos + 8, rpc_end - rpc_pos - 8);

    // Extract existing URLs from config
    std::vector<std::string> existing_urls;
    size_t pos = 0;
    while ((pos = rpc_section.find("\"url\": \"", pos)) != std::string::npos) {
        pos += 8;
        size_t url_end = rpc_section.find('"', pos);
        existing_urls.push_back(rpc_section.substr(pos, url_end - pos));
        pos = url_end + 1;
    }

    // Map blockchain name to chain_id
    std::string chain_id;
    if (blockchain == "fantom") chain_id = "250";
    else if (blockchain == "ethereum") chain_id = "1";
    else if (blockchain == "bsc") chain_id = "56";
    else if (blockchain == "polygon") chain_id = "137";
    else if (blockchain == "avalanche") chain_id = "43114";
    else if (blockchain == "solana") chain_id = "101";
    else {
        std::cerr << RED << "Unsupported blockchain: " << blockchain << RESET << "\n";
        file.close();
        return;
    }
    
    std::cerr << BLUE << "Blockchain " << RED << blockchain << YELLOW << " ID is: " << CYAN <<chain_id<< RESET << "\n";

    
    // Extract RPC and WSS from downloaded response with progress bar
    std::vector<struct_rpc_endpoint> new_endpoints;
    pos = response.find("\"" + chain_id + "\": [");
    
    
    if (pos != std::string::npos) {
        size_t rpcs_end = response.find("]", pos);
        std::string rpcs_section = response.substr(pos + chain_id.length() + 4, rpcs_end - pos - chain_id.length() - 4);
        size_t total_length = rpcs_section.length();
        size_t processed = 0;

        pos = 0;
        
        while ((pos = rpcs_section.find("\"", pos)) != std::string::npos) {
            pos += 1;
            size_t url_end = rpcs_section.find('"', pos);
            std::string url = rpcs_section.substr(pos, url_end - pos);
            if (url.find("wss://") == 0 || url.find("https://") == 0) {
                new_endpoints.push_back({url, 30, false});
            }
            
            processed = url_end;
            size_t progress = (processed * 100) / total_length;
            
            // Print progress bar
            std::cout << "\r" << BLUE << "Parsing RPCs: [";
            
            // Print progress bar
            for (size_t i = 0; i < 50; ++i) {
                std::cout << (i < progress / 2 ? (std::string(YELLOW) + "#") : " ");
            }
            std::cout << "] " << progress << "%" << RESET << std::flush;
            pos = url_end + 1;
        }
        std::cout << "\n";
    }

    // Add missing endpoints to config
    std::string new_rpc_entries;
    for (const auto& endpoint : new_endpoints) {
        if (std::find(existing_urls.begin(), existing_urls.end(), endpoint.url) == existing_urls.end()) {
            if (!new_rpc_entries.empty()) new_rpc_entries += ",\n      ";
            new_rpc_entries += "{\"url\": \"" + endpoint.url + "\", \"limit\": 30, \"active\": 0}";
        }
    }

    // Update neozork-config if there are new endpoints
    if (!new_rpc_entries.empty()) {
        std::string updated_rpc = rpc_section.empty() ? new_rpc_entries : rpc_section + ",\n      " + new_rpc_entries;
        config_content.replace(rpc_pos + 8, rpc_end - rpc_pos - 8, updated_rpc);
        file.seekp(0);
        file << config_content;
    } else std::cerr << BLUE << "Blockchain " << RED << blockchain << YELLOW << " There are NO New Endpoints" << RESET << "\n";

    file.close();

    auto end_parse = std::chrono::high_resolution_clock::now();
    auto parse_time = std::chrono::duration_cast<std::chrono::milliseconds>(end_parse - start_parse).count();

    // Display timing results
    std::cout << GREEN << "Download time: " << YELLOW << download_time << " ms" << RESET << "\n";
    std::cout << GREEN << "Parse time: " << YELLOW << parse_time << " ms" << RESET << "\n";
}

// Callback function to handle data received from CURL
// Parameters:
// - data: Pointer to the received data
// - size: Size of each data element
// - nmemb: Number of data elements
// - buffer: Reference to the string buffer to store the data
// Returns: Number of bytes processed
size_t write_callback(char* data, size_t size, size_t nmemb, std::string& buffer) {
    
    // Calculate the total size of the received data
    size_t real_size = size * nmemb;
    
    // Append the data to the buffer
    buffer.append(data, real_size);
    
    // Return the number of bytes processed
    return real_size;
}

// Function to extract the "result" field from a JSON RPC response
// Parameters:
// - json: The JSON string to parse
// Returns: The value of the "result" field or an empty string if not found
std::string parse_json_result(const std::string& json) {
    
    // Define the key to search for in the JSON
    std::string result_key = "\"result\":\"";
    
    // Find the position of the result key
    size_t start = json.find(result_key);
    
    // Return empty string if the key is not found
    if (start == std::string::npos) return "";
    
    // Move past the key to the value
    start += result_key.length();
    
    // Find the end of the result value
    size_t end = json.find('"', start);
    
    // Return empty string if the end is not found
    if (end == std::string::npos) return "";
    
    // Extract and return the result value
    return json.substr(start, end - start);
}

// Function to display a progress bar in the console
// Parameters:
// - current: Current progress value
// - total: Total value to reach
// - label: Descriptive label for the progress bar
void print_progress_bar(uint64_t current, uint64_t total, const std::string& label) {
    
    // Calculate the progress as a fraction
    float progress = static_cast<float>(current) / total;
    
    // Define the width of the progress bar
    int bar_width = 50;
    
    // Calculate the position of the progress indicator
    int pos = static_cast<int>(bar_width * progress);
    
    // Print the label and start of the bar
    std::cout << CYAN << label << ": [" << GREEN;
    
    // Draw the progress bar
    for (int i = 0; i < bar_width; ++i) {
        
        // Fill completed portion with "="
        if (i < pos) std::cout << "=";
        
        // Mark the current position with ">"
        else if (i == pos) std::cout << ">";
        
        // Fill remaining portion with spaces
        else std::cout << " ";
    }
    // Close the bar and print the percentage
    std::cout << RESET << "] " << YELLOW << int(progress * 100.0) << "%" << RESET << "\r";
    
    // Flush the output to update the display
    std::cout.flush();
    
    // Delay to make the progress bar visible
    std::this_thread::sleep_for(std::chrono::milliseconds(100));
}

// Function to fetch the latest block number from a blockchain
// Parameters:
// - rpc_url: URL of the RPC endpoint
// - request_limit: Maximum requests per second allowed by the endpoint
// - stats: Reference to FunctionStats for performance tracking
// Returns: Latest block number in hex format or empty string if failed
std::string get_latest_block_number(const std::string& rpc_url, int request_limit, struct_function_stats& stats) {
    
    // Define the RPC payload to fetch the block number
    std::string payload = "{\"jsonrpc\":\"2.0\",\"method\":\"eth_blockNumber\",\"params\":[],\"id\":1}";
    
    // Perform the RPC call and return the result
    return make_rpc_call(rpc_url, payload, request_limit, stats);
}

// Function to perform a generic RPC call to a blockchain
// Parameters:
// - rpc_url: URL of the RPC endpoint
// - payload: JSON payload for the RPC request
// - request_limit: Maximum requests per second allowed by the endpoint
// - stats: Reference to FunctionStats for performance tracking
// Returns: Parsed result from the RPC response
std::string make_rpc_call(const std::string& rpc_url, const std::string& payload, int request_limit, struct_function_stats& stats) {
    
    // Record the start time for performance measurement
    auto start = std::chrono::high_resolution_clock::now();
    
    // Initialize CURL handle for the HTTP request
    CURL* curl = curl_easy_init();
    
    // Buffer to store the RPC response
    std::string read_buffer;
    
    // Record the size of the outbound payload
    size_t outbound_size = payload.size();

    // Check if CURL initialization succeeded
    if (curl) {
        
        // Set the RPC URL for the request
        curl_easy_setopt(curl, CURLOPT_URL, rpc_url.c_str());
        
        // Set the payload for the POST request
        curl_easy_setopt(curl, CURLOPT_POSTFIELDS, payload.c_str());
        
        // Set the callback function to handle response data
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, write_callback);
        
        // Set the buffer to write response data into
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, &read_buffer);
        
        // Set HTTP headers for JSON content type
        curl_slist* headers = curl_slist_append(nullptr, "Content-Type: application/json");
        
        // Apply the headers to the CURL request
        curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headers);

        // Perform the RPC request
        CURLcode res = curl_easy_perform(curl);
        
        // Check if the request failed
        if (res != CURLE_OK) {
            std::cerr << RED << "RPC call failed: " << curl_easy_strerror(res) << RESET << '\n';
        }

        // Free the CURL headers
        curl_slist_free_all(headers);
        
        // Clean up the CURL handle
        curl_easy_cleanup(curl);
        
        // Throttle requests to respect the RPC limit
        std::this_thread::sleep_for(std::chrono::milliseconds(1000 / request_limit));
    }

    // Record the end time for performance measurement
    auto end = std::chrono::high_resolution_clock::now();
    
    // Update performance stats with timing and traffic data
    update_stats(stats, start, end, outbound_size, read_buffer.size());
    
    // Calculate and set the network latency
    stats.latency_ms = stats.execution_time_ms - (1000.0 / request_limit);
    
    // Parse and return the result from the response
    return parse_json_result(read_buffer);
}

