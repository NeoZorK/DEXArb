//
//  config_get.cpp
//  NeoZorKDEXArb
//
//  Created by Rostyslav S. on 20.03.2025.
//

#include "config_get.h"
#include "main.h"
#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <chrono>
#include <curl/curl.h>

// Structure for RPC endpoint details
struct RpcEndpoint {
    std::string url;
    int request_limit = 30; // Default request limit
    bool active = false;    // Default inactive state
};

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
            std::cout << (i < progress / 2 ? GREEN "#" : " ");
        }
        std::cout << "] " << progress << "%" << RESET << std::flush;
    }
    return realsize;
}

// Function to fetch RPC/WSS from chainlist.org and update neozork-config
void update_rpc_from_chainlist(const std::string& blockchain) {
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

    // Start parsing
    auto start_parse = std::chrono::high_resolution_clock::now();

    // Open neozork-config for reading and writing
    std::fstream file("neozork-config", std::ios::in | std::ios::out);
    if (!file.is_open()) {
        std::cerr << RED << "Error: neozork-config not found" << RESET << "\n";
        return;
    }

    // Read neozork-config content
    std::string config_content((std::istreambuf_iterator<char>(file)), std::istreambuf_iterator<char>());

    // Find blockchain section in config
    size_t chain_pos = config_content.find("\"" + blockchain + "\": {");
    if (chain_pos == std::string::npos) {
        std::cerr << RED << "Blockchain " << blockchain << " not found in neozork-config" << RESET << "\n";
        file.close();
        return;
    }

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

    // Extract RPC and WSS from downloaded response with progress bar
    std::vector<RpcEndpoint> new_endpoints;
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
                new_endpoints.push_back({url});
            }
            processed = url_end;
            size_t progress = (processed * 100) / total_length;
            std::cout << "\r" << BLUE << "Parsing RPCs: [";
            for (size_t i = 0; i < 50; ++i) {
                std::cout << (i < progress / 2 ? YELLOW "#" : " ");
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
    }

    file.close();

    auto end_parse = std::chrono::high_resolution_clock::now();
    auto parse_time = std::chrono::duration_cast<std::chrono::milliseconds>(end_parse - start_parse).count();

    // Display timing results
    std::cout << GREEN << "Download time: " << YELLOW << download_time << " ms" << RESET << "\n";
    std::cout << GREEN << "Parse time: " << YELLOW << parse_time << " ms" << RESET << "\n";
}

// Example usage
int main() {
    update_rpc_from_chainlist("bsc");
    return 0;
}
