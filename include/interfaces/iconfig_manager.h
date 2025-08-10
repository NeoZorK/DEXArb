//
//  iconfig_manager.h
//  NeoZorKDEXArb
//
//  Created by Rostyslav S. on 10.08.2025.
//
#ifndef ICONFIG_MANAGER_H
#define ICONFIG_MANAGER_H

#include <string>
#include <string_view>
#include <vector>

// Forward declarations
struct RpcEndpoint;

namespace interfaces {

// Interface for configuration management
class IConfigManager {
public:
    virtual ~IConfigManager() = default;
    
    // Load configuration from file
    virtual bool load_config(std::string_view config_path) = 0;
    
    // Save configuration to file
    virtual bool save_config(std::string_view config_path) = 0;
    
    // Create default configuration
    virtual bool create_default_config(std::string_view config_path) = 0;
    
    // Validate configuration
    virtual bool validate_config() const = 0;
    
    // Get RPC endpoints for blockchain
    virtual std::vector<RpcEndpoint> get_rpc_endpoints(std::string_view blockchain) const = 0;
    
    // Get thread count
    virtual int get_thread_count() const = 0;
    
    // Set thread count
    virtual void set_thread_count(int count) = 0;
    
    // Get configuration value
    virtual std::string get_value(std::string_view key) const = 0;
    
    // Set configuration value
    virtual void set_value(std::string_view key, std::string_view value) = 0;
    
    // Get all configuration keys
    virtual std::vector<std::string> get_all_keys() const = 0;
    
    // Check if configuration is loaded
    virtual bool is_loaded() const = 0;
};

} // namespace interfaces

#endif // ICONFIG_MANAGER_H
