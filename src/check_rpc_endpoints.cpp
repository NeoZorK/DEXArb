//
//  check_rpc_endpoints.cpp
//  NeoZorKDEXArb
//
//  Created by Rostyslav S. on 19.03.2025.
//

#include "check_rpc_endpoints.h"
#include <iostream>
#include <string>
#include <vector>
#include <curl/curl.h>
#include <functional>

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
size_t writeCallback(void* contents, size_t size, size_t nmemb, void* userp) {
    
    // Cast userp to struct_response
    struct_response& response = *static_cast<struct_response*>(userp);
    
    // Convert contents to std::string
    std::string data(static_cast<char*>(contents), size * nmemb);
    
    // Process the response data
    return process_response_data(data, response);
}

// Function to check the RPC endpoint
void check_connect_rpc(const std::string& url) {
    
    // Initialize CURL
    CURL* curl_handle = curl_easy_init();
    
    // Check if initialization was successful
    if (!curl_handle) {
        std::cerr << "Failed to initialize CURL for " << url << std::endl;
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
    curl_easy_setopt(curl_handle, CURLOPT_WRITEFUNCTION, writeCallback);
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
        std::cerr << "RPC " << url << ": Error - " << curl_easy_strerror(res) << std::endl;
        return;
    }

    // Check the response
    long http_code = 0;
    
    // Get the response code
    curl_easy_getinfo(curl_handle, CURLINFO_RESPONSE_CODE, &http_code);
    
    // Check if the response code is 200
    if (http_code == 200 && response.data.find("result") != std::string::npos) {
        std::cout << "RPC " << url << ": Available Answer " << response.data << std::endl;
    } else {
        std::cerr << "RPC " << url << ": Not Available or wrong answer - HTTP " << http_code
                  << ", Answer: " << response.data << std::endl;
    }
}

// Function to check all RPC endpoints
void check_rpc_endpoints() {

    // List of RPC endpoints
    std::vector<std::string> rpc_endpoints{
        "https://rpc.ftm.tools/",
        "https://rpc.ankr.com/fantom",
        "https://fantom.publicnode.com",
        "https://fantom-mainnet-rpc.coinsdo.com",
        "https://fantom-mainnet.public.blastapi.io",
        "https://1rpc.io/ftm"
    };

    // Initialize CURL
    curl_global_init(CURL_GLOBAL_ALL);
    
    // Create a guard object to automatically clean up CURL
    struct struct_global_curl_guard {
        
        // Construct the guard
        ~struct_global_curl_guard() { curl_global_cleanup(); }
        
        // Move constructor
    } globalGuard;

    // Check each RPC endpoint
    for (const auto& endpoint : rpc_endpoints) {
        std::cout << "Checking " << endpoint << "..." << std::endl;
        check_connect_rpc(endpoint);
    }

    // Cleanup CURL
    return;
}
