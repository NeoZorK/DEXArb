//
//  rpc_core.cpp
//  NeoZorKDEXArb
//
//  Created by Rostyslav S. on 18.03.2025.
//

#include "main.h"

size_t write_callback(char* data, size_t size, size_t nmemb, std::string& buffer) {
    // Calculate the real size of the data received
    size_t real_size = size * nmemb;
    // Append the received data to the buffer
    buffer.append(data, real_size);
    return real_size;   // Return the size processed
}

std::string parse_json_result(const std::string& json) {
    // Define the key to search for in the JSON
    std::string result_key = "\"result\":\"";
    // Find the position of the result field
    size_t start = json.find(result_key);
    if (start == std::string::npos) return ""; // Return empty if not found
    start += result_key.length(); // Move to the start of the result value
    // Find the end of the result value
    size_t end = json.find('"', start);
    if (end == std::string::npos) return ""; // Return empty if no closing quote
    return json.substr(start, end - start); // Extract and return the result
}

void print_progress_bar(int current, int total, const std::string& label) {
    // Calculate progress percentage
    float progress = static_cast<float>(current) / total;
    int bar_width = 50; // Set the width of the progress bar
    int pos = static_cast<int>(bar_width * progress); // Calculate filled portion
    std::cout << CYAN << label << ": [" << GREEN; // Start progress bar with label
    for (int i = 0; i < bar_width; ++i) { // Loop through bar width
        if (i < pos) std::cout << "=";    // Filled part
        else if (i == pos) std::cout << ">"; // Current position
        else std::cout << " ";            // Empty part
    }
    std::cout << RESET << "] " << YELLOW << int(progress * 100.0) << "%" << RESET << "\r"; // Show percentage
    std::cout.flush(); // Flush output to ensure it displays immediately
    std::this_thread::sleep_for(std::chrono::milliseconds(100)); // Small delay for visibility
}

std::string get_latest_block_number(const std::string& rpc_url, int request_limit, FunctionStats& stats) {
    // Prepare JSON RPC payload to get the latest block number
    std::string payload = "{\"jsonrpc\":\"2.0\",\"method\":\"eth_blockNumber\",\"params\":[],\"id\":1}";
    return make_rpc_call(rpc_url, payload, request_limit, stats); // Make the RPC call
}

std::string make_rpc_call(const std::string& rpc_url, const std::string& payload, int request_limit, FunctionStats& stats) {
    // Start timing the RPC call
    auto start = std::chrono::high_resolution_clock::now();
    CURL* curl = curl_easy_init(); // Initialize CURL handle
    std::string read_buffer;       // Buffer to store response
    size_t outbound_size = payload.size(); // Size of the outgoing payload

    if (curl) { // Check if CURL initialized successfully
        // Set the RPC URL
        curl_easy_setopt(curl, CURLOPT_URL, rpc_url.c_str());
        // Set the payload for the POST request
        curl_easy_setopt(curl, CURLOPT_POSTFIELDS, payload.c_str());
        // Set the write callback to handle response
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, write_callback);
        // Set the buffer to write the response into
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, &read_buffer);
        // Set content type header
        curl_slist* headers = curl_slist_append(nullptr, "Content-Type: application/json");
        curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headers);

        // Perform the RPC call
        CURLcode res = curl_easy_perform(curl);
        if (res != CURLE_OK) { // Check for errors
            std::cerr << RED << "RPC call failed: " << curl_easy_strerror(res) << RESET << '\n'; // Report error
        }

        // Clean up CURL resources
        curl_slist_free_all(headers);
        curl_easy_cleanup(curl);
        // Respect the request limit by pausing
        std::this_thread::sleep_for(std::chrono::milliseconds(1000 / request_limit));
    }

    // Update performance stats
    auto end = std::chrono::high_resolution_clock::now();
    update_stats(stats, start, end, outbound_size, read_buffer.size()); // Record timing and traffic
    stats.latency_ms = stats.execution_time_ms - (1000.0 / request_limit); // Adjust latency
    return parse_json_result(read_buffer); // Return parsed result
}
