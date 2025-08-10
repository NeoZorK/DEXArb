//
//  rpc_core.cpp
//  NeoZorKDEXArb
//
//  Created by Rostyslav S. on 18.03.2025.
//
#include "rpc_core.h"       // Include own header for declarations
#include "modern_utils.h"    // For modern JSON parser and logger
#include "measure.h"        // For update_stats function
#include <curl/curl.h>      // For CURL HTTP requests
#include <iostream>         // For console output
#include <thread>           // For std::this_thread::sleep_for

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
    std::cout << "DEBUG: parse_json_result called with: '" << json << "'" << std::endl;
    
    // Simple string-based JSON parser for RPC responses
    // Look for "result": "value" pattern
    size_t result_pos = json.find("\"result\":");
    if (result_pos == std::string::npos) {
        std::cout << "DEBUG: No 'result' field found in JSON" << std::endl;
        return ""; // No result field found
    }
    
    std::cout << "DEBUG: Found 'result' at position: " << result_pos << std::endl;
    
    // Move past "result":
    result_pos += 9;
    
    // Skip whitespace
    while (result_pos < json.length() && (json[result_pos] == ' ' || json[result_pos] == '\t' || json[result_pos] == '\n')) {
        result_pos++;
    }
    
    if (result_pos >= json.length()) {
        std::cout << "DEBUG: Reached end of string after 'result:'" << std::endl;
        return "";
    }
    
    std::cout << "DEBUG: After whitespace, position: " << result_pos << ", char: '" << json[result_pos] << "'" << std::endl;
    
    // Check if result is a string (starts with quote)
    if (json[result_pos] == '"') {
        result_pos++; // Skip opening quote
        size_t end_pos = json.find('"', result_pos);
        if (end_pos == std::string::npos) {
            std::cout << "DEBUG: No closing quote found" << std::endl;
            return "";
        }
        std::string result = json.substr(result_pos, end_pos - result_pos);
        std::cout << "DEBUG: Extracted string result: '" << result << "'" << std::endl;
        return result;
    }
    
    // Check if result is a number (no quotes)
    size_t end_pos = result_pos;
    while (end_pos < json.length() && 
           ((json[end_pos] >= '0' && json[end_pos] <= '9') || 
            (json[end_pos] >= 'a' && json[end_pos] <= 'f') || 
            (json[end_pos] >= 'A' && json[end_pos] <= 'F') || 
            json[end_pos] == 'x')) {
        end_pos++;
    }
    
    std::string result = json.substr(result_pos, end_pos - result_pos);
    std::cout << "DEBUG: Extracted number result: '" << result << "'" << std::endl;
    return result;
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
std::string get_latest_block_number(const std::string& rpc_url, int request_limit, FunctionStats& stats) {
    std::cout << "DEBUG: get_latest_block_number called with URL: " << rpc_url << std::endl;
    
    // Define the RPC payload to fetch the block number
    std::string payload = "{\"jsonrpc\":\"2.0\",\"method\":\"eth_blockNumber\",\"params\":[],\"id\":1}";
    
    std::cout << "DEBUG: Calling RPC endpoint: " << rpc_url << std::endl;
    std::cout << "DEBUG: Payload: " << payload << std::endl;
    
    // Perform the RPC call and return the result
    std::string result = make_rpc_call(rpc_url, payload, request_limit, stats);
    
    std::cout << "DEBUG: RPC result: '" << result << "'" << std::endl;
    
    return result;
}

// Function to perform a generic RPC call to a blockchain
// Parameters:
// - rpc_url: URL of the RPC endpoint
// - payload: JSON payload for the RPC request
// - request_limit: Maximum requests per second allowed by the endpoint
// - stats: Reference to FunctionStats for performance tracking
// Returns: Parsed result from the RPC response
std::string make_rpc_call(const std::string& rpc_url, const std::string& payload, int request_limit, FunctionStats& stats) {
    // Record the start time for performance measurement
    auto start = std::chrono::high_resolution_clock::now();
    // Initialize CURL handle for the HTTP request
    CURL* curl = curl_easy_init();
    // Buffer to store the RPC response
    std::string read_buffer;
    // Record the size of the outbound payload
    size_t outbound_size = payload.size();

    // Check if CURL initialization succeeded
    if (!curl) {
        std::cerr << RED << "Failed to initialize CURL" << RESET << '\n';
        return ""; // Return empty string if CURL init failed
    }

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
        // Print error message with CURL error details
        std::cerr << RED << "RPC call failed: " << curl_easy_strerror(res) << RESET << '\n';
        // Free the CURL headers
        curl_slist_free_all(headers);
        // Clean up the CURL handle
        curl_easy_cleanup(curl);
        return ""; // Return empty string if RPC call failed
    }

    // Free the CURL headers
    curl_slist_free_all(headers);
    // Clean up the CURL handle
    curl_easy_cleanup(curl);
    // Throttle requests to respect the RPC limit
    std::this_thread::sleep_for(std::chrono::milliseconds(1000 / request_limit));

    // Record the end time for performance measurement
    auto end = std::chrono::high_resolution_clock::now();
    // Update performance stats with timing and traffic data
    update_stats(stats, start, end, outbound_size, read_buffer.size());
    // Calculate and set the network latency
    stats.latency_ms = stats.execution_time_ms - (1000.0 / request_limit);
    
    // Add debug output to see what we're getting
    std::cout << "DEBUG: RPC response: " << read_buffer << std::endl;
    
    // Parse and return the result from the response
    return parse_json_result(read_buffer);
}
