//
//  irpc_client.h
//  NeoZorKDEXArb
//
//  Created by Rostyslav S. on 10.08.2025.
//
#ifndef IRPC_CLIENT_H
#define IRPC_CLIENT_H

#include <string>
#include <string_view>
#include <vector>

namespace interfaces {

// Interface for RPC communication
class IRpcClient {
public:
    virtual ~IRpcClient() = default;
    
    // Make JSON-RPC request
    virtual std::string make_request(
        std::string_view endpoint, 
        std::string_view method, 
        std::string_view params) = 0;
    
    // Make request with custom headers
    virtual std::string make_request_with_headers(
        std::string_view endpoint,
        std::string_view method,
        std::string_view params,
        const std::vector<std::pair<std::string, std::string>>& headers) = 0;
    
    // Check if endpoint is available
    virtual bool is_endpoint_available(std::string_view endpoint) = 0;
    
    // Request statistics structure
    struct RequestStats {
        uint64_t total_requests = 0;
        uint64_t successful_requests = 0;
        uint64_t failed_requests = 0;
        double average_response_time_ms = 0.0;
    };
    
    // Get request statistics
    virtual RequestStats get_stats() const = 0;
    
    // Reset statistics
    virtual void reset_stats() = 0;
};

} // namespace interfaces

#endif // IRPC_CLIENT_H
