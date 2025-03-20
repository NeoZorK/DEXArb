//
//  check_rpc_endpoints.cpp
//  NeoZorKDEXArb
//
//  Created by Rostyslav S. on 19.03.2025.
//

#include "check_rpc_endpoints.h"
#include "main.h"
#include <iostream>
#include <string>
#include <vector>
#include <curl/curl.h>
#include <functional>
#include <fstream>

// Function to set the "active" field for a specific URL in neozork-config
void set_active_endpoint(const std::string& blockchain, const std::string& url, bool activate) {
    
    // Open file for both reading and writing
    std::fstream file("neozork-config", std::ios::in | std::ios::out);
    if (!file.is_open()) {
        throw std::runtime_error("Failed to open neozork-config for reading and writing");
    }

    // Read file content into a string
    std::string content((std::istreambuf_iterator<char>(file)), std::istreambuf_iterator<char>());

    // Find blockchain section
    size_t chain_pos = content.find("\"" + blockchain + "\": {");
    if (chain_pos == std::string::npos) {
        file.close();
        throw std::runtime_error("Blockchain " + blockchain + " not found in config");
    }

    // Find RPC array
    size_t rpc_pos = content.find("\"rpc\": [", chain_pos);
    if (rpc_pos == std::string::npos) {
        file.close();
        throw std::runtime_error("RPC array not found in " + blockchain + " section");
    }

    // Find end of RPC array
    size_t rpc_end = content.find("]", rpc_pos);
    if (rpc_end == std::string::npos) {
        file.close();
        throw std::runtime_error("Invalid RPC array end");
    }

    // Find the specific URL within the RPC array
    size_t url_pos = content.find("\"url\": \"" + url + "\"", rpc_pos);
    if (url_pos == std::string::npos || url_pos >= rpc_end) {
        file.close();
        throw std::runtime_error("URL " + url + " not found in " + blockchain + " RPC array");
    }

    // Find "active" field after the URL
    size_t active_pos = content.find("\"active\": ", url_pos);
    if (active_pos == std::string::npos || active_pos >= rpc_end) {
        file.close();
        throw std::runtime_error("Field 'active' not found for URL " + url);
    }

    // Move to the value position after "active":
    size_t value_pos = active_pos + 9;
    size_t value_end = content.find_first_of(",}", value_pos);
    std::string current_value = content.substr(value_pos, value_end - value_pos);
    current_value.erase(0, current_value.find_first_not_of(" \t"));
    current_value.erase(current_value.find_last_not_of(" \t") + 1);

    // Replace the value with "1" or "0" based on activate flag
    std::string new_value = activate ? " 1" : " 0";
    if (current_value != new_value) {
        content.replace(value_pos, value_end - value_pos, new_value);

        // Move file pointer to the beginning and write updated content
        file.seekp(0);
        file << content;
    }

    // Close the file
    file.close();
}


// Using std::string to avoid manual memory management
struct struct_response {
    std::string data;
};

// Function to process the response data
size_t process_response_data(const std::string& contents, struct_response& response) {
    
    // Append the received data to the response
    response.data.append(contents);
    
    // Return the size of the received data
    return contents.size();
}

// Callback function to handle data received from CURL
size_t write_callback(void* contents, size_t size, size_t nmemb, void* userp) {
    
    // Cast userp to struct_response
    struct_response& response = *static_cast<struct_response*>(userp);
    
    // Convert contents to std::string
    std::string data(static_cast<char*>(contents), size * nmemb);
    
    // Process the response data
    return process_response_data(data, response);
}

// Function to check the RPC endpoint
void check_connect_rpc(const std::string& url, std::string& blockchain_str) {
    
    // Initialize CURL
    CURL* curl_handle = curl_easy_init();
    
    // Check if initialization was successful
    if (!curl_handle) {
        std::cerr << RED <<  " Failed to initialize CURL for " << BLUE << url << RESET << std::endl;
        return;
    }
    
    // Cleanup CURL on scope exit
    auto curl_cleanup = [curl_handle]() { curl_easy_cleanup(curl_handle); };
    
    // Create a guard object to automatically clean up CURL
    struct curl_guard {
        
        // Store the cleanup function
        std::function<void()> cleanup;
        
        // Construct the guard
        ~curl_guard() { cleanup(); }
        
        // Move constructor
    } guard{curl_cleanup};

    
    // Create a struct to store the response
    struct_response response;

    // Set up the CURL options
    curl_easy_setopt(curl_handle, CURLOPT_URL, url.c_str());
    curl_easy_setopt(curl_handle, CURLOPT_POST, 1L);
    curl_easy_setopt(curl_handle, CURLOPT_POSTFIELDS, "{\"jsonrpc\":\"2.0\",\"method\":\"eth_blockNumber\",\"params\":[],\"id\":1}");
    curl_easy_setopt(curl_handle, CURLOPT_WRITEFUNCTION, write_callback);
    curl_easy_setopt(curl_handle, CURLOPT_WRITEDATA, &response);
    curl_easy_setopt(curl_handle, CURLOPT_FOLLOWLOCATION, 1L);

    // Set custom headers
    std::vector<std::string> header_list = {"Content-Type: application/json"};
    
    // Set headers
    struct curl_slist* headers = nullptr;
    
    // Add headers
    for (const auto& header : header_list) {
        headers = curl_slist_append(headers, header.c_str());
    }
    
    // Set headers
    curl_easy_setopt(curl_handle, CURLOPT_HTTPHEADER, headers);

    // Create a guard object to automatically free headers
    struct struct_header_guard {
        
        // Store the headers
        curl_slist* ptr;
        
        // Construct the guard
        ~struct_header_guard() { if (ptr) curl_slist_free_all(ptr); }
        
        // Move constructor
    } header_guard{headers};

    // Perform the request
    CURLcode res = curl_easy_perform(curl_handle);
    
    // Check for errors
    if (res != CURLE_OK) {
        std::cerr << RED << "RPC "<< BLUE << url << RED << ": Error - " << curl_easy_strerror(res) << RESET << std::endl;
        return;
    }

    // Check the response
    long http_code = 0;
    
    // Get the response code
    curl_easy_getinfo(curl_handle, CURLINFO_RESPONSE_CODE, &http_code);
    
    // Check if the response code is 200
    if (http_code == 200 && response.data.find("result") != std::string::npos) {
        
        // mark Active RPC
        set_active_endpoint(blockchain_str, url, true);
        
        // Print the response
        std::cout << GREEN << "RPC " << CYAN << url << GREEN << ": Available Answer " << WHITE << response.data << RESET << std::endl;
    } else {
        
        // mark NOT Active RPC
        set_active_endpoint(blockchain_str, url, false);
        
        // Print the response
        std::cerr << RED << "RPC " << BLUE << url << RED <<": Not Available or wrong answer - HTTP " << http_code
                  << ", Answer: " << WHITE << response.data << RESET << std::endl;
    }
}

// Function to check all RPC endpoints
void preliminary_check_rpc_endpoints(std::vector<RpcEndpoint>& rpc_endpoints, std::string& blockchain_str) {

    // Initialize CURL
    curl_global_init(CURL_GLOBAL_ALL);
    
    // Create a guard object to automatically clean up CURL
    struct struct_global_curl_guard {
        
        // Construct the guard
        ~struct_global_curl_guard() { curl_global_cleanup(); }
        
        // Move constructor
    } globalGuard;

    // Print the number of RPC endpoints
    std::cout  << CYAN << " Total RPC endpoints: "  << std::to_string(rpc_endpoints.size())  << RESET << std::endl;
    
    // Initialize url index
    int url_index = 0;
    
    // Check each RPC endpoint
    for (const auto& endpoint : rpc_endpoints) {
        
        // Increment url index
        url_index++;
        
        // Print the RPC endpoint
        std::cout << std::to_string(url_index) << YELLOW << " >> Checking " << BLUE << endpoint.url << GREEN << "..." << RESET << std::endl;
        
        // Check RPC
        check_connect_rpc(endpoint.url, blockchain_str);
    }

    // Cleanup CURL
    return;
}
