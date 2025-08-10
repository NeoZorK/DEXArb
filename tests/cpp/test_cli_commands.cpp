//
//  test_cli_commands.cpp
//  NeoZorKDEXArb
//
//  Created by Rostyslav S. on 10.08.2025.
//
#include <iostream>
#include <string>
#include <vector>
#include <cassert>

// Simple test framework
#define ASSERT_EQ(a, b) if ((a) != (b)) { std::cerr << "ASSERT_EQ failed: " << (a) << " != " << (b) << " at line " << __LINE__ << std::endl; return false; }
#define ASSERT_TRUE(a) if (!(a)) { std::cerr << "ASSERT_TRUE failed at line " << __LINE__ << std::endl; return false; }
#define ASSERT_FALSE(a) if (a) { std::cerr << "ASSERT_FALSE failed at line " << __LINE__ << std::endl; return false; }
#define ASSERT_STREQ(a, b) if ((a) != (b)) { std::cerr << "ASSERT_STREQ failed: " << (a) << " != " << (b) << " at line " << __LINE__ << std::endl; return false; }

// Include our CLI utilities
#include "../../include/cli/command_parser.h"

// Add stream operator for CommandType for testing
namespace cli {
    inline std::ostream& operator<<(std::ostream& os, CommandType type) {
        switch (type) {
            case CommandType::HELP: return os << "HELP";
            case CommandType::VERSION_CMD: return os << "VERSION_CMD";
            case CommandType::SCAN: return os << "SCAN";
            case CommandType::SHOW_DEXES: return os << "SHOW_DEXES";
            case CommandType::SHOW_POOLS: return os << "SHOW_POOLS";
            case CommandType::SHOW_TOKENS: return os << "SHOW_TOKENS";
            case CommandType::SHOW_SCAN_CONFIG: return os << "SHOW_SCAN_CONFIG";
            case CommandType::SHOW_SCAN: return os << "SHOW_SCAN";
            case CommandType::SHOW_SCAN_STAT: return os << "SHOW_SCAN_STAT";
            case CommandType::FIND_TOKEN: return os << "FIND_TOKEN";
            case CommandType::FIND_TOKENS: return os << "FIND_TOKENS";
            case CommandType::UNKNOWN: return os << "UNKNOWN";
        }
    }
}

using namespace cli;

static bool test_command_type_enum() {
    std::cout << "Testing command type enumeration..." << std::endl;
    
    // Test that all command types are distinct
    ASSERT_TRUE(CommandType::HELP != CommandType::VERSION_CMD);
    ASSERT_TRUE(CommandType::VERSION_CMD != CommandType::SCAN);
    ASSERT_TRUE(CommandType::SCAN != CommandType::SHOW_DEXES);
    ASSERT_TRUE(CommandType::SHOW_DEXES != CommandType::SHOW_POOLS);
    ASSERT_TRUE(CommandType::SHOW_POOLS != CommandType::SHOW_TOKENS);
    ASSERT_TRUE(CommandType::SHOW_TOKENS != CommandType::SHOW_SCAN_CONFIG);
    ASSERT_TRUE(CommandType::SHOW_SCAN_CONFIG != CommandType::SHOW_SCAN);
    ASSERT_TRUE(CommandType::SHOW_SCAN != CommandType::SHOW_SCAN_STAT);
    ASSERT_TRUE(CommandType::SHOW_SCAN_STAT != CommandType::FIND_TOKEN);
    ASSERT_TRUE(CommandType::FIND_TOKEN != CommandType::FIND_TOKENS);
    ASSERT_TRUE(CommandType::FIND_TOKENS != CommandType::UNKNOWN);
    
    return true;
}

static bool test_string_to_command_type() {
    std::cout << "Testing string to command type conversion..." << std::endl;
    
    // Test all flag mappings
    ASSERT_EQ(CommandParser::string_to_command_type("-h"), CommandType::HELP);
    ASSERT_EQ(CommandParser::string_to_command_type("-help"), CommandType::HELP);
    
    ASSERT_EQ(CommandParser::string_to_command_type("-v"), CommandType::VERSION_CMD);
    ASSERT_EQ(CommandParser::string_to_command_type("-version"), CommandType::VERSION_CMD);
    
    ASSERT_EQ(CommandParser::string_to_command_type("-scan"), CommandType::SCAN);
    
    ASSERT_EQ(CommandParser::string_to_command_type("-showDEXES"), CommandType::SHOW_DEXES);
    
    ASSERT_EQ(CommandParser::string_to_command_type("-showPOOLS"), CommandType::SHOW_POOLS);
    
    ASSERT_EQ(CommandParser::string_to_command_type("-showTOKENS"), CommandType::SHOW_TOKENS);
    
    ASSERT_EQ(CommandParser::string_to_command_type("-showSCAN-CONFIG"), CommandType::SHOW_SCAN_CONFIG);
    
    ASSERT_EQ(CommandParser::string_to_command_type("-showSCAN"), CommandType::SHOW_SCAN);
    
    ASSERT_EQ(CommandParser::string_to_command_type("-showSCAN-STAT"), CommandType::SHOW_SCAN_STAT);
    
    ASSERT_EQ(CommandParser::string_to_command_type("-findTOKEN"), CommandType::FIND_TOKEN);
    
    ASSERT_EQ(CommandParser::string_to_command_type("-findTOKENS"), CommandType::FIND_TOKENS);
    
    // Test unknown commands
    ASSERT_EQ(CommandParser::string_to_command_type("--unknown"), CommandType::UNKNOWN);
    ASSERT_EQ(CommandParser::string_to_command_type("invalid"), CommandType::UNKNOWN);
    ASSERT_EQ(CommandParser::string_to_command_type(""), CommandType::UNKNOWN);
    
    return true;
}

static bool test_command_requirements() {
    std::cout << "Testing command requirements..." << std::endl;
    
    // Test blockchain requirements
    ASSERT_TRUE(CommandParser::requires_blockchain(CommandType::SCAN));
    ASSERT_TRUE(CommandParser::requires_blockchain(CommandType::SHOW_DEXES));
    ASSERT_TRUE(CommandParser::requires_blockchain(CommandType::SHOW_POOLS));
    ASSERT_TRUE(CommandParser::requires_blockchain(CommandType::SHOW_TOKENS));
    ASSERT_TRUE(CommandParser::requires_blockchain(CommandType::FIND_TOKEN));
    ASSERT_TRUE(CommandParser::requires_blockchain(CommandType::FIND_TOKENS));
    
    ASSERT_FALSE(CommandParser::requires_blockchain(CommandType::HELP));
    ASSERT_FALSE(CommandParser::requires_blockchain(CommandType::VERSION_CMD));
    ASSERT_TRUE(CommandParser::requires_blockchain(CommandType::SCAN));
    ASSERT_TRUE(CommandParser::requires_blockchain(CommandType::SHOW_DEXES));
    ASSERT_TRUE(CommandParser::requires_blockchain(CommandType::SHOW_POOLS));
    ASSERT_TRUE(CommandParser::requires_blockchain(CommandType::SHOW_TOKENS));
    ASSERT_TRUE(CommandParser::requires_blockchain(CommandType::SHOW_SCAN_CONFIG));
    ASSERT_TRUE(CommandParser::requires_blockchain(CommandType::SHOW_SCAN));
    ASSERT_TRUE(CommandParser::requires_blockchain(CommandType::SHOW_SCAN_STAT));
    ASSERT_TRUE(CommandParser::requires_blockchain(CommandType::FIND_TOKEN));
    ASSERT_TRUE(CommandParser::requires_blockchain(CommandType::FIND_TOKENS));
    
    // Test value requirements
    ASSERT_TRUE(CommandParser::requires_value(CommandType::SCAN));
    ASSERT_TRUE(CommandParser::requires_value(CommandType::FIND_TOKEN));
    ASSERT_TRUE(CommandParser::requires_value(CommandType::FIND_TOKENS));
    
    ASSERT_FALSE(CommandParser::requires_value(CommandType::HELP));
    ASSERT_FALSE(CommandParser::requires_value(CommandType::VERSION_CMD));
    ASSERT_FALSE(CommandParser::requires_value(CommandType::SHOW_DEXES));
    ASSERT_FALSE(CommandParser::requires_value(CommandType::SHOW_POOLS));
    ASSERT_FALSE(CommandParser::requires_value(CommandType::SHOW_TOKENS));
    ASSERT_FALSE(CommandParser::requires_value(CommandType::SHOW_SCAN_CONFIG));
    ASSERT_FALSE(CommandParser::requires_value(CommandType::SHOW_SCAN));
    ASSERT_FALSE(CommandParser::requires_value(CommandType::SHOW_SCAN_STAT));
    
    // Test DEX requirements
    ASSERT_TRUE(CommandParser::requires_dex(CommandType::SHOW_POOLS));
    ASSERT_TRUE(CommandParser::requires_dex(CommandType::SHOW_TOKENS));
    ASSERT_TRUE(CommandParser::requires_dex(CommandType::FIND_TOKEN));
    
    ASSERT_FALSE(CommandParser::requires_dex(CommandType::HELP));
    ASSERT_FALSE(CommandParser::requires_dex(CommandType::VERSION_CMD));
    ASSERT_FALSE(CommandParser::requires_dex(CommandType::SCAN));
    ASSERT_FALSE(CommandParser::requires_dex(CommandType::SHOW_DEXES));
    ASSERT_FALSE(CommandParser::requires_dex(CommandType::SHOW_SCAN_CONFIG));
    ASSERT_FALSE(CommandParser::requires_dex(CommandType::SHOW_SCAN));
    ASSERT_FALSE(CommandParser::requires_dex(CommandType::SHOW_SCAN_STAT));
    ASSERT_FALSE(CommandParser::requires_dex(CommandType::FIND_TOKENS));
    
    // Test token requirements
    ASSERT_TRUE(CommandParser::requires_token(CommandType::FIND_TOKEN));
    
    ASSERT_FALSE(CommandParser::requires_token(CommandType::HELP));
    ASSERT_FALSE(CommandParser::requires_token(CommandType::VERSION_CMD));
    ASSERT_FALSE(CommandParser::requires_token(CommandType::SCAN));
    ASSERT_FALSE(CommandParser::requires_token(CommandType::SHOW_DEXES));
    ASSERT_FALSE(CommandParser::requires_token(CommandType::SHOW_POOLS));
    ASSERT_FALSE(CommandParser::requires_token(CommandType::SHOW_TOKENS));
    ASSERT_FALSE(CommandParser::requires_token(CommandType::SHOW_SCAN_CONFIG));
    ASSERT_FALSE(CommandParser::requires_token(CommandType::SHOW_SCAN));
    ASSERT_FALSE(CommandParser::requires_token(CommandType::SHOW_SCAN_STAT));
    ASSERT_FALSE(CommandParser::requires_token(CommandType::FIND_TOKENS));
    
    return true;
}

static bool test_command_descriptions() {
    std::cout << "Testing command descriptions..." << std::endl;
    
    // Test that all commands have descriptions
    ASSERT_FALSE(CommandParser::get_command_description(CommandType::HELP).empty());
    ASSERT_FALSE(CommandParser::get_command_description(CommandType::VERSION_CMD).empty());
    ASSERT_FALSE(CommandParser::get_command_description(CommandType::SCAN).empty());
    ASSERT_FALSE(CommandParser::get_command_description(CommandType::SHOW_DEXES).empty());
    ASSERT_FALSE(CommandParser::get_command_description(CommandType::SHOW_POOLS).empty());
    ASSERT_FALSE(CommandParser::get_command_description(CommandType::SHOW_TOKENS).empty());
    ASSERT_FALSE(CommandParser::get_command_description(CommandType::SHOW_SCAN_CONFIG).empty());
    ASSERT_FALSE(CommandParser::get_command_description(CommandType::SHOW_SCAN).empty());
    ASSERT_FALSE(CommandParser::get_command_description(CommandType::SHOW_SCAN_STAT).empty());
    ASSERT_FALSE(CommandParser::get_command_description(CommandType::FIND_TOKEN).empty());
    ASSERT_FALSE(CommandParser::get_command_description(CommandType::FIND_TOKENS).empty());
    ASSERT_FALSE(CommandParser::get_command_description(CommandType::UNKNOWN).empty());
    
    return true;
}

static bool test_parsed_command_construction() {
    std::cout << "Testing parsed command construction..." << std::endl;
    
    // Test default constructor
    ParsedCommand cmd1;
    ASSERT_FALSE(cmd1.is_valid);
    ASSERT_EQ(cmd1.type, CommandType::UNKNOWN);
    ASSERT_TRUE(cmd1.flag.empty());
    ASSERT_TRUE(cmd1.blockchain.empty());
    ASSERT_TRUE(cmd1.value.empty());
    ASSERT_TRUE(cmd1.dex_name.empty());
    ASSERT_TRUE(cmd1.token_address.empty());
    ASSERT_TRUE(cmd1.error_message.empty());
    
    // Test parameterized constructor
    ParsedCommand cmd2(CommandType::SCAN, "-scan", "ethereum", "1000");
    ASSERT_TRUE(cmd2.is_valid);
    ASSERT_EQ(cmd2.type, CommandType::SCAN);
    ASSERT_STREQ(cmd2.flag, "-scan");
    ASSERT_STREQ(cmd2.blockchain, "ethereum");
    ASSERT_STREQ(cmd2.value, "1000");
    
    return true;
}

static bool test_command_parsing() {
    std::cout << "Testing command parsing..." << std::endl;
    
    std::cout << "Testing help command..." << std::endl;
    // Test help command
    const char* help_argv[] = {"program", "-h"};
    auto help_cmd = CommandParser::parse(2, help_argv);
    ASSERT_TRUE(help_cmd.is_valid);
    ASSERT_EQ(help_cmd.type, CommandType::HELP);
    ASSERT_STREQ(help_cmd.flag, "-h");
    
    std::cout << "Testing version command..." << std::endl;
    // Test version command
    const char* version_argv[] = {"program", "-v"};
    auto version_cmd = CommandParser::parse(2, version_argv);
    ASSERT_TRUE(version_cmd.is_valid);
    ASSERT_EQ(version_cmd.type, CommandType::VERSION_CMD);
    ASSERT_STREQ(version_cmd.flag, "-v");
    
    std::cout << "Testing scan command..." << std::endl;
    // Test scan command
    const char* scan_argv[] = {"program", "-scan", "ethereum", "1000"};
    auto scan_cmd = CommandParser::parse(4, scan_argv);
    std::cout << "Scan command type: " << scan_cmd.type << ", valid: " << scan_cmd.is_valid << std::endl;
    std::cout << "Expected type: " << CommandType::SCAN << std::endl;
    std::cout << "Error message: " << scan_cmd.error_message << std::endl;
    ASSERT_TRUE(scan_cmd.is_valid);
    ASSERT_EQ(scan_cmd.type, CommandType::SCAN);
    ASSERT_STREQ(scan_cmd.blockchain, "ethereum");
    ASSERT_STREQ(scan_cmd.value, "1000");
    
    std::cout << "Testing show dexes command..." << std::endl;
    // Test show dexes command
    const char* dexes_argv[] = {"program", "-showDEXES", "polygon"};
    auto dexes_cmd = CommandParser::parse(3, dexes_argv);
    std::cout << "DEXES command type: " << dexes_cmd.type << ", valid: " << dexes_cmd.is_valid << std::endl;
    std::cout << "Expected type: " << CommandType::SHOW_DEXES << std::endl;
    ASSERT_TRUE(dexes_cmd.is_valid);
    ASSERT_EQ(dexes_cmd.type, CommandType::SHOW_DEXES);
    ASSERT_STREQ(dexes_cmd.blockchain, "polygon");
    
    std::cout << "Testing show pools command..." << std::endl;
    // Test show pools command
    const char* pools_argv[] = {"program", "-showPOOLS", "bsc", "pancakeswap"};
    auto pools_cmd = CommandParser::parse(4, pools_argv);
    ASSERT_TRUE(pools_cmd.is_valid);
    ASSERT_EQ(pools_cmd.type, CommandType::SHOW_POOLS);
    ASSERT_STREQ(pools_cmd.blockchain, "bsc");
    ASSERT_STREQ(pools_cmd.dex_name, "pancakeswap");
    
    return true;
}

static bool test_command_validation() {
    std::cout << "Testing command validation..." << std::endl;
    
    // Test valid commands
    ParsedCommand valid_cmd1(CommandType::HELP, "-h");
    ASSERT_TRUE(CommandParser::validate_command(valid_cmd1));
    
    ParsedCommand valid_cmd2(CommandType::VERSION_CMD, "-v");
    ASSERT_TRUE(CommandParser::validate_command(valid_cmd2));
    
    ParsedCommand valid_cmd3(CommandType::SCAN, "-scan", "ethereum", "1000");
    ASSERT_TRUE(CommandParser::validate_command(valid_cmd3));
    
    ParsedCommand valid_cmd4(CommandType::SHOW_DEXES, "-showDEXES", "polygon");
    ASSERT_TRUE(CommandParser::validate_command(valid_cmd4));
    
    ParsedCommand valid_cmd5(CommandType::SHOW_POOLS, "-showPOOLS", "bsc", "");
    valid_cmd5.dex_name = "pancakeswap";
    ASSERT_TRUE(CommandParser::validate_command(valid_cmd5));
    
    ParsedCommand valid_cmd6(CommandType::FIND_TOKEN, "-findTOKEN", "ethereum", "");
    valid_cmd6.dex_name = "uniswap";
    valid_cmd6.token_address = "0x1234567890abcdef1234567890abcdef12345678";
    ASSERT_TRUE(CommandParser::validate_command(valid_cmd6));
    
    // Test invalid commands
    ParsedCommand invalid_cmd1(CommandType::SCAN, "-scan", "", "");
    invalid_cmd1.blockchain = ""; // missing blockchain
    ASSERT_FALSE(CommandParser::validate_command(invalid_cmd1));
    
    ParsedCommand invalid_cmd2(CommandType::SCAN, "-scan", "invalid", "1000"); // invalid blockchain
    ASSERT_FALSE(CommandParser::validate_command(invalid_cmd2));
    
    ParsedCommand invalid_cmd3(CommandType::SCAN, "-scan", "ethereum", "invalid"); // invalid block range
    ASSERT_FALSE(CommandParser::validate_command(invalid_cmd3));
    
    ParsedCommand invalid_cmd4(CommandType::FIND_TOKEN, "-findTOKEN", "ethereum", "");
    invalid_cmd4.token_address = "invalid"; // invalid address
    ASSERT_FALSE(CommandParser::validate_command(invalid_cmd4));
    
    return true;
}

int main() {
    std::cout << "Running CLI Commands Tests" << std::endl;
    std::cout << "=========================" << std::endl;
    
    int passed = 0;
    int total = 0;
    
    auto run_test = [&](const std::string& name, bool (*test_func)()) {
        total++;
        std::cout << "\nRunning test: " << name << std::endl;
        if (test_func()) {
            std::cout << "✓ PASS: " << name << std::endl;
            passed++;
        } else {
            std::cout << "✗ FAIL: " << name << std::endl;
        }
    };
    
    run_test("Command Type Enum", test_command_type_enum);
    run_test("String to Command Type", test_string_to_command_type);
    run_test("Command Requirements", test_command_requirements);
    run_test("Command Descriptions", test_command_descriptions);
    run_test("Parsed Command Construction", test_parsed_command_construction);
    run_test("Command Parsing", test_command_parsing);
    run_test("Command Validation", test_command_validation);
    
    std::cout << "\n" << std::string(50, '=') << std::endl;
    std::cout << "Test Summary:" << std::endl;
    std::cout << "Passed: " << passed << "/" << total << std::endl;
    
    if (passed == total) {
        std::cout << "All tests passed! ✓" << std::endl;
        return 0;
    } else {
        std::cout << "Some tests failed! ✗" << std::endl;
        return 1;
    }
}
