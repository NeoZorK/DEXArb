//
//  command_parser.h
//  NeoZorKDEXArb
//
//  Created by Rostyslav S. on 10.08.2025.
//
#ifndef COMMAND_PARSER_H
#define COMMAND_PARSER_H

#include <string>
#include <string_view>
#include <vector>

namespace cli {

// Command types enumeration
enum class CommandType {
    HELP,
    VERSION_CMD,
    SCAN,
    SHOW_DEXES,
    SHOW_POOLS,
    SHOW_TOKENS,
    SHOW_SCAN_CONFIG,
    SHOW_SCAN,
    SHOW_SCAN_STAT,
    FIND_TOKEN,
    FIND_TOKENS,
    UNKNOWN
};

// Parsed command structure
struct ParsedCommand {
    CommandType type = CommandType::UNKNOWN;
    std::string flag;
    std::string blockchain;
    std::string value;
    std::string dex_name;
    std::string token_address;
    bool is_valid = false;
    std::string error_message;
    
    // Constructor
    ParsedCommand() = default;
    
    // Constructor with parameters
    ParsedCommand(CommandType cmd_type, std::string_view flag_str, 
                  std::string_view chain = "", std::string_view val = "")
        : type(cmd_type), flag(flag_str), blockchain(chain), value(val), is_valid(true) {}
};

// Command parser class
class CommandParser {
public:
    // Parse command line arguments
    static ParsedCommand parse(int argc, char* argv[]);
    
    // Validate parsed command
    static bool validate_command(const ParsedCommand& cmd);
    
    // Convert string to command type
    static CommandType string_to_command_type(std::string_view flag);
    
    // Get command type description
    static std::string get_command_description(CommandType type);
    
    // Check if command requires blockchain parameter
    static bool requires_blockchain(CommandType type);
    
    // Check if command requires value parameter
    static bool requires_value(CommandType type);
    
    // Check if command requires DEX parameter
    static bool requires_dex(CommandType type);
    
    // Check if command requires token parameter
    static bool requires_token(CommandType type);
    
private:
    // Validate blockchain name
    static bool is_valid_blockchain(std::string_view blockchain);
    
    // Validate block range
    static bool is_valid_block_range(std::string_view value);
    
    // Validate address format
    static bool is_valid_address(std::string_view address);
};

} // namespace cli

#endif // COMMAND_PARSER_H
