//
//  application.h
//  NeoZorKDEXArb
//
//  Created by Rostyslav S. on 10.08.2025.
//
#ifndef APPLICATION_H
#define APPLICATION_H

#include <memory>
#include <string>
#include <string_view>

// Include the actual implementations instead of forward declarations
#include "../utils/modern_utils.h"
#include "../interfaces/iconfig_manager.h"
#include "../interfaces/iblockchain_scanner.h"
#include "../interfaces/irpc_client.h"
#include "../cli/command_parser.h"
#include "../main.h"

namespace core {

// Main application class
class Application {
public:
    // Constructor
    Application();
    
    // Destructor
    ~Application();
    
    // Run application with command line arguments
    int run(int argc, const char* argv[]);
    
    // Initialize application
    bool initialize();
    
    // Shutdown application
    void shutdown();
    
    // Check if application is initialized
    bool is_initialized() const;
    
    // Get application version
    static std::string get_version();
    
    // Get build information
    static std::string get_build_info();

private:
    // Handle different command types
    int handle_help_command();
    int handle_version_command();
    int handle_scan_command(const cli::ParsedCommand& cmd);
    int handle_show_command(const cli::ParsedCommand& cmd);
    int handle_find_command(const cli::ParsedCommand& cmd);
    int handle_unknown_command(const cli::ParsedCommand& cmd);
    
    // Command execution helpers
    int execute_scan(std::string_view blockchain, std::string_view blocks);
    int execute_show_dexes(std::string_view blockchain);
    int execute_show_pools(std::string_view blockchain, std::string_view dex_name);
    int execute_show_tokens(std::string_view blockchain, std::string_view dex_name);
    int execute_show_scan_config(std::string_view blockchain);
    int execute_show_scan(std::string_view blockchain);
    int execute_show_scan_stat(std::string_view blockchain);
    int execute_find_token(std::string_view blockchain, std::string_view dex_name, 
                          std::string_view token_address);
    int execute_find_tokens(std::string_view blockchain, std::string_view token_address);
    
    // Validation helpers
    bool validate_scan_parameters(std::string_view blockchain, std::string_view blocks);
    bool validate_show_parameters(const cli::ParsedCommand& cmd);
    bool validate_find_parameters(const cli::ParsedCommand& cmd);
    
    // Error handling
    void log_error(std::string_view error_message);
    void log_warning(std::string_view warning_message);
    void log_info(std::string_view info_message);
    
    // Member variables
    std::unique_ptr<modern_utils::Logger> logger_;
    std::unique_ptr<interfaces::IConfigManager> config_manager_;
    std::unique_ptr<interfaces::IBlockchainScanner> blockchain_scanner_;
    std::unique_ptr<interfaces::IRpcClient> rpc_client_;
    bool initialized_ = false;
    std::unique_ptr<FunctionStats> app_stats_;
};

} // namespace core

#endif // APPLICATION_H
