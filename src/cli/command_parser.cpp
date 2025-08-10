//
//  command_parser.cpp
//  NeoZorKDEXArb
//
//  Created by Rostyslav S. on 10.08.2025.
//
#include "../../include/cli/command_parser.h"
#include "../../include/blockchain.h"
#include <algorithm>
#include <cctype>

namespace cli {

ParsedCommand CommandParser::parse(int argc, const char* argv[]) {
    ParsedCommand cmd;
    
    if (argc < 2) {
        cmd.type = CommandType::HELP;
        cmd.is_valid = true;
        return cmd;
    }
    
    std::string flag = argv[1];
    cmd.flag = flag;
    cmd.type = string_to_command_type(flag);
    
    // Parse additional arguments based on command type
    switch (cmd.type) {
        case CommandType::HELP:
        case CommandType::VERSION_CMD:
            cmd.is_valid = true;
            break;
            
        case CommandType::SCAN:
            if (argc >= 4) {
                cmd.blockchain = argv[2];
                cmd.value = argv[3];
                cmd.is_valid = true;
                if (!validate_command(cmd)) {
                    cmd.is_valid = false;
                }
            } else {
                cmd.error_message = "Scan command requires blockchain and block count";
            }
            break;
            
        case CommandType::SHOW_DEXES:
            if (argc >= 3) {
                cmd.blockchain = argv[2];
                cmd.is_valid = true;
                if (!validate_command(cmd)) {
                    cmd.is_valid = false;
                }
            } else {
                cmd.error_message = "Show DEXes command requires blockchain";
            }
            break;
            
        case CommandType::SHOW_POOLS:
        case CommandType::SHOW_TOKENS:
            if (argc >= 4) {
                cmd.blockchain = argv[2];
                cmd.dex_name = argv[3];
                cmd.is_valid = true;
                if (!validate_command(cmd)) {
                    cmd.is_valid = false;
                }
            } else {
                cmd.error_message = "Show pools/tokens command requires blockchain and DEX name";
            }
            break;
            
        case CommandType::SHOW_SCAN_CONFIG:
        case CommandType::SHOW_SCAN:
        case CommandType::SHOW_SCAN_STAT:
            if (argc >= 3) {
                cmd.blockchain = argv[2];
                cmd.is_valid = true;
                if (!validate_command(cmd)) {
                    cmd.is_valid = false;
                }
            } else {
                cmd.error_message = "Show scan command requires blockchain";
            }
            break;
            
        case CommandType::FIND_TOKEN:
            if (argc >= 5) {
                cmd.blockchain = argv[2];
                cmd.dex_name = argv[3];
                cmd.token_address = argv[4];
                cmd.is_valid = true;
                if (!validate_command(cmd)) {
                    cmd.is_valid = false;
                }
            } else {
                cmd.error_message = "Find token command requires blockchain, DEX name, and token address";
            }
            break;
            
        case CommandType::FIND_TOKENS:
            if (argc >= 4) {
                cmd.blockchain = argv[2];
                cmd.token_address = argv[3];
                cmd.is_valid = true;
                if (!validate_command(cmd)) {
                    cmd.is_valid = false;
                }
            } else {
                cmd.error_message = "Find tokens command requires blockchain and token address";
            }
            break;
            
        case CommandType::UNKNOWN:
            cmd.error_message = "Unknown command: " + flag;
            break;
    }
    
    return cmd;
}

bool CommandParser::validate_command(const ParsedCommand& cmd) {
    if (!cmd.is_valid) {
        return false;
    }
    
    // Validate blockchain if required
    if (requires_blockchain(cmd.type) && !is_valid_blockchain(cmd.blockchain)) {
        return false;
    }
    
    // Validate block range if required
    if (requires_value(cmd.type) && cmd.type == CommandType::SCAN) {
        if (!is_valid_block_range(cmd.value)) {
            return false;
        }
    }
    
    // Validate token address if required
    if (requires_token(cmd.type) && !is_valid_address(cmd.token_address)) {
        return false;
    }
    
    return true;
}

CommandType CommandParser::string_to_command_type(std::string_view flag) {
    if (flag == "-h" || flag == "-help" || flag == "--help") {
        return CommandType::HELP;
    } else if (flag == "-v" || flag == "-version" || flag == "--version") {
        return CommandType::VERSION_CMD;
    } else if (flag == "-scan") {
        return CommandType::SCAN;
    } else if (flag == "-showDEXES") {
        return CommandType::SHOW_DEXES;
    } else if (flag == "-showPOOLS") {
        return CommandType::SHOW_POOLS;
    } else if (flag == "-showTOKENS") {
        return CommandType::SHOW_TOKENS;
    } else if (flag == "-showSCAN-CONFIG") {
        return CommandType::SHOW_SCAN_CONFIG;
    } else if (flag == "-showSCAN") {
        return CommandType::SHOW_SCAN;
    } else if (flag == "-showSCAN-STAT") {
        return CommandType::SHOW_SCAN_STAT;
    } else if (flag == "-findTOKEN") {
        return CommandType::FIND_TOKEN;
    } else if (flag == "-findTOKENS") {
        return CommandType::FIND_TOKENS;
    }
    
    return CommandType::UNKNOWN;
}

std::string CommandParser::get_command_description(CommandType type) {
    switch (type) {
        case CommandType::HELP: return "Show help information";
        case CommandType::VERSION_CMD: return "Show version information";
        case CommandType::SCAN: return "Scan blockchain for DEXes";
        case CommandType::SHOW_DEXES: return "Show all DEXes on blockchain";
        case CommandType::SHOW_POOLS: return "Show pools for specific DEX";
        case CommandType::SHOW_TOKENS: return "Show tokens for specific DEX";
        case CommandType::SHOW_SCAN_CONFIG: return "Show scan configuration";
        case CommandType::SHOW_SCAN: return "Show scan results";
        case CommandType::SHOW_SCAN_STAT: return "Show scan statistics";
        case CommandType::FIND_TOKEN: return "Find token in specific DEX";
        case CommandType::FIND_TOKENS: return "Find token across all DEXes";
        case CommandType::UNKNOWN: return "Unknown command";
    }
}

bool CommandParser::requires_blockchain(CommandType type) {
    return type != CommandType::HELP && type != CommandType::VERSION_CMD;
}

bool CommandParser::requires_value(CommandType type) {
    return type == CommandType::SCAN || type == CommandType::FIND_TOKEN || type == CommandType::FIND_TOKENS;
}

bool CommandParser::requires_dex(CommandType type) {
    return type == CommandType::SHOW_POOLS || type == CommandType::SHOW_TOKENS || 
           type == CommandType::FIND_TOKEN;
}

bool CommandParser::requires_token(CommandType type) {
    return type == CommandType::FIND_TOKEN;
}

bool CommandParser::is_valid_blockchain(std::string_view blockchain) {
    std::string chain = std::string(blockchain);
    std::transform(chain.begin(), chain.end(), chain.begin(), ::tolower);
    
    return chain == "ethereum" || chain == "bsc" || chain == "polygon" || 
           chain == "fantom" || chain == "avalanche" || chain == "solana";
}

bool CommandParser::is_valid_block_range(std::string_view value) {
    try {
        int blocks = std::stoi(std::string(value));
        return blocks >= 1000 && blocks <= 1000000;
    } catch (...) {
        return false;
    }
}

bool CommandParser::is_valid_address(std::string_view address) {
    // Basic Ethereum address validation (0x + 40 hex chars)
    if (address.length() != 42 || address.substr(0, 2) != "0x") {
        return false;
    }
    
    for (size_t i = 2; i < address.length(); ++i) {
        if (!std::isxdigit(address[i])) {
            return false;
        }
    }
    
    return true;
}

} // namespace cli
