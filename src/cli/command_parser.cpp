//
//  command_parser.cpp
//  NeoZorKDEXArb
//
//  Created by Rostyslav S. on 10.08.2025.
//
#include "../../include/cli/command_parser.h"
#include "../../include/core/blockchain.h"
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
        case CommandType::EXAMPLES:
            cmd.is_valid = true;
            break;
            
        case CommandType::SCAN:
            if (argc >= 4) {
                // Handle network ID or blockchain name
                std::string blockchain_or_id = argv[2];
                if (is_network_id(blockchain_or_id)) {
                    cmd.blockchain = network_id_to_blockchain(blockchain_or_id);
                } else {
                    cmd.blockchain = blockchain_or_id;
                }
                cmd.value = argv[3];
                cmd.is_valid = true;
                if (!validate_command(cmd)) {
                    cmd.is_valid = false;
                }
            } else if (argc == 3) {
                // Handle scan with default block count
                std::string blockchain_or_id = argv[2];
                if (is_network_id(blockchain_or_id)) {
                    cmd.blockchain = network_id_to_blockchain(blockchain_or_id);
                } else {
                    cmd.blockchain = blockchain_or_id;
                }
                cmd.value = get_default_block_count();
                cmd.is_valid = true;
                if (!validate_command(cmd)) {
                    cmd.is_valid = false;
                }
            } else if (argc == 2) {
                // Handle scan with default blockchain and block count
                cmd.blockchain = get_default_blockchain();
                cmd.value = get_default_block_count();
                cmd.is_valid = true;
                if (!validate_command(cmd)) {
                    cmd.is_valid = false;
                }
            } else {
                cmd.error_message = "Scan command requires blockchain and block count";
            }
            break;
            
        case CommandType::SHOW_DEXES:
        case CommandType::SHOW_ALL_DEXES:
        case CommandType::SHOW_SCAN_CONFIG:
        case CommandType::SHOW_SCAN:
        case CommandType::SHOW_SCAN_STAT:
            if (argc == 3) {
                std::string blockchain_or_id = argv[2];
                if (is_network_id(blockchain_or_id)) {
                    cmd.blockchain = network_id_to_blockchain(blockchain_or_id);
                } else {
                    cmd.blockchain = blockchain_or_id;
                }
                cmd.is_valid = true;
                if (!validate_command(cmd)) {
                    cmd.is_valid = false;
                }
            } else if (argc > 3) {
                cmd.error_message = "Command requires only blockchain parameter";
            } else {
                cmd.error_message = "Command requires blockchain parameter";
            }
            break;
            
        case CommandType::SHOW_POOLS:
        case CommandType::SHOW_TOKENS:
            if (argc >= 4) {
                std::string blockchain_or_id = argv[2];
                if (is_network_id(blockchain_or_id)) {
                    cmd.blockchain = network_id_to_blockchain(blockchain_or_id);
                } else {
                    cmd.blockchain = blockchain_or_id;
                }
                cmd.dex_name = argv[3];
                cmd.is_valid = true;
                if (!validate_command(cmd)) {
                    cmd.is_valid = false;
                }
            } else if (argc == 3) {
                // Allow blockchain-only for showPOOLS and showTOKENS (shows all pools/tokens)
                std::string blockchain_or_id = argv[2];
                if (is_network_id(blockchain_or_id)) {
                    cmd.blockchain = network_id_to_blockchain(blockchain_or_id);
                } else {
                    cmd.blockchain = blockchain_or_id;
                }
                cmd.is_valid = true;
                if (!validate_command(cmd)) {
                    cmd.is_valid = false;
                }
            } else {
                cmd.error_message = "Command requires blockchain parameter";
            }
            break;
            
        case CommandType::FIND_TOKEN:
            if (argc >= 5) {
                std::string blockchain_or_id = argv[2];
                if (is_network_id(blockchain_or_id)) {
                    cmd.blockchain = network_id_to_blockchain(blockchain_or_id);
                } else {
                    cmd.blockchain = blockchain_or_id;
                }
                cmd.dex_name = argv[3];
                cmd.token_address = argv[4];
                cmd.is_valid = true;
                if (!validate_command(cmd)) {
                    cmd.is_valid = false;
                }
            } else if (argc >= 4) {
                // Set blockchain and DEX but mark as invalid due to missing token
                std::string blockchain_or_id = argv[2];
                if (is_network_id(blockchain_or_id)) {
                    cmd.blockchain = network_id_to_blockchain(blockchain_or_id);
                } else {
                    cmd.blockchain = blockchain_or_id;
                }
                cmd.dex_name = argv[3];
                cmd.is_valid = false;
                cmd.error_message = "Find token command requires blockchain, DEX, and token parameters";
            } else if (argc >= 3) {
                // Set blockchain but mark as invalid due to missing DEX and token
                std::string blockchain_or_id = argv[2];
                if (is_network_id(blockchain_or_id)) {
                    cmd.blockchain = network_id_to_blockchain(blockchain_or_id);
                } else {
                    cmd.blockchain = blockchain_or_id;
                }
                cmd.is_valid = false;
                cmd.error_message = "Find token command requires blockchain, DEX, and token parameters";
            } else {
                cmd.error_message = "Find token command requires blockchain, DEX, and token parameters";
            }
            break;
            
        case CommandType::FIND_TOKENS:
            if (argc >= 4) {
                std::string blockchain_or_id = argv[2];
                if (is_network_id(blockchain_or_id)) {
                    cmd.blockchain = network_id_to_blockchain(blockchain_or_id);
                } else {
                    cmd.blockchain = blockchain_or_id;
                }
                cmd.token_address = argv[3];
                cmd.is_valid = true;
                if (!validate_command(cmd)) {
                    cmd.is_valid = false;
                }
            } else {
                cmd.error_message = "Find tokens command requires blockchain and token parameters";
            }
            break;
            
        default:
            cmd.error_message = "Unknown command: " + flag;
            break;
    }
    
    return cmd;
}

bool CommandParser::validate_command(const ParsedCommand& cmd) {
    // Validate blockchain if required
    if (requires_blockchain(cmd.type)) {
        if (cmd.blockchain.empty() || !is_valid_blockchain(cmd.blockchain)) {
            return false;
        }
    }
    
    // Validate value if required
    if (requires_value(cmd.type)) {
        if (cmd.value.empty() || !is_valid_block_range(cmd.value)) {
            return false;
        }
    }
    
    // Validate token address if required
    if (requires_token(cmd.type)) {
        if (cmd.token_address.empty() || !is_valid_address(cmd.token_address)) {
            return false;
        }
    }
    
    return true;
}

CommandType CommandParser::string_to_command_type(std::string_view flag) {
    if (flag == "-h" || flag == "-help" || flag == "--help") {
        return CommandType::HELP;
    } else if (flag == "-v" || flag == "-version" || flag == "--version") {
        return CommandType::VERSION_CMD;
    } else if (flag == "-examples" || flag == "--examples") {
        return CommandType::EXAMPLES;
    } else if (flag == "-scan" || flag == "--scan") {
        return CommandType::SCAN;
    } else if (flag == "-showDEXES" || flag == "--show-dexes") {
        return CommandType::SHOW_DEXES;
    } else if (flag == "-dexes" || flag == "--dexes") {
        return CommandType::SHOW_ALL_DEXES;
    } else if (flag == "-showPOOLS" || flag == "--show-pools") {
        return CommandType::SHOW_POOLS;
    } else if (flag == "-showTOKENS" || flag == "--show-tokens") {
        return CommandType::SHOW_TOKENS;
    } else if (flag == "-showSCAN-CONFIG" || flag == "--show-scan-config") {
        return CommandType::SHOW_SCAN_CONFIG;
    } else if (flag == "-showSCAN" || flag == "--show-scan") {
        return CommandType::SHOW_SCAN;
    } else if (flag == "-showSCAN-STAT" || flag == "--show-scan-stat") {
        return CommandType::SHOW_SCAN_STAT;
    } else if (flag == "-findTOKEN" || flag == "--find-token") {
        return CommandType::FIND_TOKEN;
    } else if (flag == "-findTOKENS" || flag == "--find-tokens") {
        return CommandType::FIND_TOKENS;
    }
    
    return CommandType::UNKNOWN;
}

std::string CommandParser::get_command_description(CommandType type) {
    switch (type) {
        case CommandType::HELP: return "Display help information";
        case CommandType::VERSION_CMD: return "Display version information";
        case CommandType::EXAMPLES: return "Display detailed examples";
        case CommandType::SCAN: return "Scan blockchain for arbitrage opportunities";
        case CommandType::SHOW_DEXES: return "Show available DEXes for blockchain";
        case CommandType::SHOW_ALL_DEXES: return "Show all known DEXes by blockchain";
        case CommandType::SHOW_POOLS: return "Show pools for specific DEX";
        case CommandType::SHOW_TOKENS: return "Show tokens for specific DEX";
        case CommandType::SHOW_SCAN_CONFIG: return "Show scan configuration";
        case CommandType::SHOW_SCAN: return "Show scan results";
        case CommandType::SHOW_SCAN_STAT: return "Show scan statistics";
        case CommandType::FIND_TOKEN: return "Find specific token information";
        case CommandType::FIND_TOKENS: return "Find token across all DEXes";
        case CommandType::UNKNOWN: return "Unknown command";
        default: return "Unknown command";
    }
}

bool CommandParser::requires_blockchain(CommandType type) {
    return type != CommandType::HELP && type != CommandType::VERSION_CMD && 
           type != CommandType::EXAMPLES && type != CommandType::UNKNOWN;
}

bool CommandParser::requires_value(CommandType type) {
    return type == CommandType::SCAN;
}

bool CommandParser::requires_dex(CommandType type) {
    return type == CommandType::FIND_TOKEN;
}

bool CommandParser::requires_token(CommandType type) {
    return type == CommandType::FIND_TOKEN;
}

std::string CommandParser::network_id_to_blockchain(std::string_view network_id) {
    if (network_id == "1") return "ethereum";
    if (network_id == "56") return "bsc";
    if (network_id == "137") return "polygon";
    if (network_id == "250") return "fantom";
    if (network_id == "43114") return "avalanche";
    if (network_id == "101") return "solana";
    return std::string(network_id); // Return as-is if not recognized
}

std::string CommandParser::blockchain_to_network_id(std::string_view blockchain) {
    std::string chain = std::string(blockchain);
    std::transform(chain.begin(), chain.end(), chain.begin(), ::tolower);
    
    if (chain == "ethereum") return "1";
    if (chain == "bsc") return "56";
    if (chain == "polygon") return "137";
    if (chain == "fantom") return "250";
    if (chain == "avalanche") return "43114";
    if (chain == "solana") return "101";
    return std::string(blockchain); // Return as-is if not recognized
}

bool CommandParser::is_network_id(std::string_view input) {
    return is_valid_network_id(input);
}

std::string CommandParser::get_default_blockchain() {
    return "fantom"; // Default blockchain is Fantom
}

std::string CommandParser::get_default_block_count() {
    return "1000"; // Default block count is 1000
}

bool CommandParser::is_valid_blockchain(std::string_view blockchain) {
    std::string chain = std::string(blockchain);
    std::transform(chain.begin(), chain.end(), chain.begin(), ::tolower);
    
    return chain == "ethereum" || chain == "bsc" || chain == "polygon" || 
           chain == "fantom" || chain == "avalanche" || chain == "solana";
}

bool CommandParser::is_valid_block_range(std::string_view value) {
    try {
        int block_count = std::stoi(std::string(value));
        return block_count >= 1000 && block_count <= 1000000;
    } catch (const std::exception&) {
        return false;
    }
}

bool CommandParser::is_valid_address(std::string_view address) {
    // Basic Ethereum address validation (0x + 40 hex chars)
    if (address.length() != 42 || address.substr(0, 2) != "0x") {
        return false;
    }
    
    for (size_t i = 2; i < address.length(); i++) {
        if (!isxdigit(address[i])) {
            return false;
        }
    }
    
    return true;
}

bool CommandParser::is_valid_network_id(std::string_view network_id) {
    try {
        int id = std::stoi(std::string(network_id));
        return id == 1 || id == 56 || id == 137 || id == 250 || id == 43114 || id == 101;
    } catch (const std::exception&) {
        return false;
    }
}

} // namespace cli
