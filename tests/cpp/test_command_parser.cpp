//
//  test_command_parser.cpp
//  NeoZorKDEXArb Tests
//
//  Created by Rostyslav S. on 10.08.2025.
//

#include <gtest/gtest.h>
#include "../../include/cli/command_parser.h"
#include <vector>
#include <string>

namespace cli {
namespace test {

class CommandParserTest : public ::testing::Test {
protected:
    void SetUp() override {
        // Setup code if needed
    }
    
    void TearDown() override {
        // Cleanup code if needed
    }
    
    // Helper method to create argv-like array
    std::vector<std::string> args_storage;
    std::vector<const char*> create_argv(const std::vector<std::string>& args) {
        args_storage = args; // Store strings to keep them alive
        std::vector<const char*> argv;
        argv.reserve(args.size());
        for (const auto& arg : args_storage) {
            argv.push_back(arg.c_str());
        }
        return argv;
    }
};

// Test basic command parsing
TEST_F(CommandParserTest, ParseHelpCommand) {
    auto argv = create_argv({"program", "--help"});
    auto cmd = CommandParser::parse(argv.size(), argv.data());
    
    EXPECT_EQ(cmd.type, CommandType::HELP);
    EXPECT_EQ(cmd.flag, "--help");
    EXPECT_TRUE(cmd.is_valid);
    EXPECT_TRUE(cmd.error_message.empty());
}

TEST_F(CommandParserTest, ParseVersionCommand) {
    auto argv = create_argv({"program", "--version"});
    auto cmd = CommandParser::parse(argv.size(), argv.data());
    
    EXPECT_EQ(cmd.type, CommandType::VERSION_CMD);
    EXPECT_EQ(cmd.flag, "--version");
    EXPECT_TRUE(cmd.is_valid);
    EXPECT_TRUE(cmd.error_message.empty());
}

TEST_F(CommandParserTest, ParseScanCommandValid) {
    auto argv = create_argv({"program", "--scan", "ethereum", "1000"});
    auto cmd = CommandParser::parse(argv.size(), argv.data());
    
    EXPECT_EQ(cmd.type, CommandType::SCAN);
    EXPECT_EQ(cmd.flag, "--scan");
    EXPECT_EQ(cmd.blockchain, "ethereum");
    EXPECT_EQ(cmd.value, "1000");
    EXPECT_TRUE(cmd.is_valid);
    EXPECT_TRUE(cmd.error_message.empty());
}

TEST_F(CommandParserTest, ParseScanCommandInvalidArgs) {
    auto argv = create_argv({"program", "--scan", "ethereum"});
    auto cmd = CommandParser::parse(argv.size(), argv.data());
    
    EXPECT_EQ(cmd.type, CommandType::SCAN);
    EXPECT_EQ(cmd.flag, "--scan");
    EXPECT_EQ(cmd.blockchain, "ethereum");
    EXPECT_FALSE(cmd.is_valid);
    EXPECT_FALSE(cmd.error_message.empty());
    EXPECT_EQ(cmd.error_message, "Scan command requires blockchain and block count");
}

TEST_F(CommandParserTest, ParseShowDexesCommandValid) {
    auto argv = create_argv({"program", "--show-dexes", "ethereum"});
    auto cmd = CommandParser::parse(argv.size(), argv.data());
    
    EXPECT_EQ(cmd.type, CommandType::SHOW_DEXES);
    EXPECT_EQ(cmd.flag, "--show-dexes");
    EXPECT_EQ(cmd.blockchain, "ethereum");
    EXPECT_TRUE(cmd.is_valid);
    EXPECT_TRUE(cmd.error_message.empty());
}

TEST_F(CommandParserTest, ParseShowDexesCommandInvalidArgs) {
    auto argv = create_argv({"program", "--show-dexes"});
    auto cmd = CommandParser::parse(argv.size(), argv.data());
    
    EXPECT_EQ(cmd.type, CommandType::SHOW_DEXES);
    EXPECT_EQ(cmd.flag, "--show-dexes");
    EXPECT_FALSE(cmd.is_valid);
    EXPECT_FALSE(cmd.error_message.empty());
    EXPECT_EQ(cmd.error_message, "Show DEXes command requires blockchain");
}

TEST_F(CommandParserTest, ParseShowPoolsCommandValid) {
    auto argv = create_argv({"program", "--show-pools", "ethereum", "uniswap"});
    auto cmd = CommandParser::parse(argv.size(), argv.data());
    
    EXPECT_EQ(cmd.type, CommandType::SHOW_POOLS);
    EXPECT_EQ(cmd.flag, "--show-pools");
    EXPECT_EQ(cmd.blockchain, "ethereum");
    EXPECT_EQ(cmd.dex_name, "uniswap");
    EXPECT_TRUE(cmd.is_valid);
    EXPECT_TRUE(cmd.error_message.empty());
}

TEST_F(CommandParserTest, ParseShowPoolsCommandInvalidArgs) {
    auto argv = create_argv({"program", "--show-pools", "ethereum"});
    auto cmd = CommandParser::parse(argv.size(), argv.data());
    
    EXPECT_EQ(cmd.type, CommandType::SHOW_POOLS);
    EXPECT_EQ(cmd.flag, "--show-pools");
    EXPECT_EQ(cmd.blockchain, "ethereum");
    EXPECT_FALSE(cmd.is_valid);
    EXPECT_FALSE(cmd.error_message.empty());
    EXPECT_EQ(cmd.error_message, "Show pools/tokens command requires blockchain and DEX name");
}

TEST_F(CommandParserTest, ParseShowTokensCommandValid) {
    auto argv = create_argv({"program", "--show-tokens", "ethereum", "uniswap"});
    auto cmd = CommandParser::parse(argv.size(), argv.data());
    
    EXPECT_EQ(cmd.type, CommandType::SHOW_TOKENS);
    EXPECT_EQ(cmd.flag, "--show-tokens");
    EXPECT_EQ(cmd.blockchain, "ethereum");
    EXPECT_EQ(cmd.dex_name, "uniswap");
    EXPECT_TRUE(cmd.is_valid);
    EXPECT_TRUE(cmd.error_message.empty());
}

TEST_F(CommandParserTest, ParseShowTokensCommandInvalidArgs) {
    auto argv = create_argv({"program", "--show-tokens", "ethereum"});
    auto cmd = CommandParser::parse(argv.size(), argv.data());
    
    EXPECT_EQ(cmd.type, CommandType::SHOW_TOKENS);
    EXPECT_EQ(cmd.flag, "--show-tokens");
    EXPECT_EQ(cmd.blockchain, "ethereum");
    EXPECT_FALSE(cmd.is_valid);
    EXPECT_FALSE(cmd.error_message.empty());
    EXPECT_EQ(cmd.error_message, "Show pools/tokens command requires blockchain and DEX name");
}

TEST_F(CommandParserTest, ParseShowScanConfigCommandValid) {
    auto argv = create_argv({"program", "--show-scan-config", "ethereum"});
    auto cmd = CommandParser::parse(argv.size(), argv.data());
    
    EXPECT_EQ(cmd.type, CommandType::SHOW_SCAN_CONFIG);
    EXPECT_EQ(cmd.flag, "--show-scan-config");
    EXPECT_EQ(cmd.blockchain, "ethereum");
    EXPECT_TRUE(cmd.is_valid);
    EXPECT_TRUE(cmd.error_message.empty());
}

TEST_F(CommandParserTest, ParseShowScanCommandValid) {
    auto argv = create_argv({"program", "--show-scan", "ethereum"});
    auto cmd = CommandParser::parse(argv.size(), argv.data());
    
    EXPECT_EQ(cmd.type, CommandType::SHOW_SCAN);
    EXPECT_EQ(cmd.flag, "--show-scan");
    EXPECT_EQ(cmd.blockchain, "ethereum");
    EXPECT_TRUE(cmd.is_valid);
    EXPECT_TRUE(cmd.error_message.empty());
}

TEST_F(CommandParserTest, ParseShowScanStatCommandValid) {
    auto argv = create_argv({"program", "--show-scan-stat", "ethereum"});
    auto cmd = CommandParser::parse(argv.size(), argv.data());
    
    EXPECT_EQ(cmd.type, CommandType::SHOW_SCAN_STAT);
    EXPECT_EQ(cmd.flag, "--show-scan-stat");
    EXPECT_EQ(cmd.blockchain, "ethereum");
    EXPECT_TRUE(cmd.is_valid);
    EXPECT_TRUE(cmd.error_message.empty());
}

TEST_F(CommandParserTest, ParseFindTokenCommandValid) {
    auto argv = create_argv({"program", "--find-token", "ethereum", "uniswap", "0x1234567890abcdef1234567890abcdef12345678"});
    auto cmd = CommandParser::parse(argv.size(), argv.data());
    
    EXPECT_EQ(cmd.type, CommandType::FIND_TOKEN);
    EXPECT_EQ(cmd.flag, "--find-token");
    EXPECT_EQ(cmd.blockchain, "ethereum");
    EXPECT_EQ(cmd.dex_name, "uniswap");
    EXPECT_EQ(cmd.token_address, "0x1234567890abcdef1234567890abcdef12345678");
    EXPECT_TRUE(cmd.is_valid);
    EXPECT_TRUE(cmd.error_message.empty());
}

TEST_F(CommandParserTest, ParseFindTokenCommandInvalidArgs) {
    auto argv = create_argv({"program", "--find-token", "ethereum", "uniswap"});
    auto cmd = CommandParser::parse(argv.size(), argv.data());
    
    EXPECT_EQ(cmd.type, CommandType::FIND_TOKEN);
    EXPECT_EQ(cmd.flag, "--find-token");
    EXPECT_EQ(cmd.blockchain, "ethereum");
    EXPECT_EQ(cmd.dex_name, "uniswap");
    EXPECT_FALSE(cmd.is_valid);
    EXPECT_FALSE(cmd.error_message.empty());
    EXPECT_EQ(cmd.error_message, "Find token command requires blockchain, DEX name, and token address");
}

TEST_F(CommandParserTest, ParseUnknownCommand) {
    auto argv = create_argv({"program", "--unknown-flag"});
    auto cmd = CommandParser::parse(argv.size(), argv.data());
    
    EXPECT_EQ(cmd.type, CommandType::UNKNOWN);
    EXPECT_EQ(cmd.flag, "--unknown-flag");
    EXPECT_FALSE(cmd.is_valid);
}

TEST_F(CommandParserTest, ParseNoArguments) {
    auto argv = create_argv({"program"});
    auto cmd = CommandParser::parse(argv.size(), argv.data());
    
    EXPECT_EQ(cmd.type, CommandType::HELP);
    EXPECT_TRUE(cmd.is_valid);
    EXPECT_TRUE(cmd.error_message.empty());
}

// Test string to command type conversion
TEST_F(CommandParserTest, StringToCommandType) {
    EXPECT_EQ(CommandParser::string_to_command_type("--help"), CommandType::HELP);
    EXPECT_EQ(CommandParser::string_to_command_type("--version"), CommandType::VERSION_CMD);
    EXPECT_EQ(CommandParser::string_to_command_type("--scan"), CommandType::SCAN);
    EXPECT_EQ(CommandParser::string_to_command_type("--show-dexes"), CommandType::SHOW_DEXES);
    EXPECT_EQ(CommandParser::string_to_command_type("--show-pools"), CommandType::SHOW_POOLS);
    EXPECT_EQ(CommandParser::string_to_command_type("--show-tokens"), CommandType::SHOW_TOKENS);
    EXPECT_EQ(CommandParser::string_to_command_type("--show-scan-config"), CommandType::SHOW_SCAN_CONFIG);
    EXPECT_EQ(CommandParser::string_to_command_type("--show-scan"), CommandType::SHOW_SCAN);
    EXPECT_EQ(CommandParser::string_to_command_type("--show-scan-stat"), CommandType::SHOW_SCAN_STAT);
    EXPECT_EQ(CommandParser::string_to_command_type("--find-token"), CommandType::FIND_TOKEN);
    EXPECT_EQ(CommandParser::string_to_command_type("--unknown"), CommandType::UNKNOWN);
    EXPECT_EQ(CommandParser::string_to_command_type(""), CommandType::UNKNOWN);
}

// Test command descriptions
TEST_F(CommandParserTest, GetCommandDescription) {
    EXPECT_EQ(CommandParser::get_command_description(CommandType::HELP), "Display help information");
    EXPECT_EQ(CommandParser::get_command_description(CommandType::VERSION_CMD), "Display version information");
    EXPECT_EQ(CommandParser::get_command_description(CommandType::SCAN), "Scan blockchain for arbitrage opportunities");
    EXPECT_EQ(CommandParser::get_command_description(CommandType::SHOW_DEXES), "Show available DEXes for blockchain");
    EXPECT_EQ(CommandParser::get_command_description(CommandType::SHOW_POOLS), "Show pools for specific DEX");
    EXPECT_EQ(CommandParser::get_command_description(CommandType::SHOW_TOKENS), "Show tokens for specific DEX");
    EXPECT_EQ(CommandParser::get_command_description(CommandType::SHOW_SCAN_CONFIG), "Show scan configuration");
    EXPECT_EQ(CommandParser::get_command_description(CommandType::SHOW_SCAN), "Show scan results");
    EXPECT_EQ(CommandParser::get_command_description(CommandType::SHOW_SCAN_STAT), "Show scan statistics");
    EXPECT_EQ(CommandParser::get_command_description(CommandType::FIND_TOKEN), "Find specific token information");
    EXPECT_EQ(CommandParser::get_command_description(CommandType::UNKNOWN), "Unknown command");
}

// Test command requirements
TEST_F(CommandParserTest, RequiresBlockchain) {
    EXPECT_FALSE(CommandParser::requires_blockchain(CommandType::HELP));
    EXPECT_FALSE(CommandParser::requires_blockchain(CommandType::VERSION_CMD));
    EXPECT_TRUE(CommandParser::requires_blockchain(CommandType::SCAN));
    EXPECT_TRUE(CommandParser::requires_blockchain(CommandType::SHOW_DEXES));
    EXPECT_TRUE(CommandParser::requires_blockchain(CommandType::SHOW_POOLS));
    EXPECT_TRUE(CommandParser::requires_blockchain(CommandType::SHOW_TOKENS));
    EXPECT_TRUE(CommandParser::requires_blockchain(CommandType::SHOW_SCAN_CONFIG));
    EXPECT_TRUE(CommandParser::requires_blockchain(CommandType::SHOW_SCAN));
    EXPECT_TRUE(CommandParser::requires_blockchain(CommandType::SHOW_SCAN_STAT));
    EXPECT_TRUE(CommandParser::requires_blockchain(CommandType::FIND_TOKEN));
    EXPECT_FALSE(CommandParser::requires_blockchain(CommandType::UNKNOWN));
}

TEST_F(CommandParserTest, RequiresValue) {
    EXPECT_FALSE(CommandParser::requires_value(CommandType::HELP));
    EXPECT_FALSE(CommandParser::requires_value(CommandType::VERSION_CMD));
    EXPECT_TRUE(CommandParser::requires_value(CommandType::SCAN));
    EXPECT_FALSE(CommandParser::requires_value(CommandType::SHOW_DEXES));
    EXPECT_FALSE(CommandParser::requires_value(CommandType::SHOW_POOLS));
    EXPECT_FALSE(CommandParser::requires_value(CommandType::SHOW_TOKENS));
    EXPECT_FALSE(CommandParser::requires_value(CommandType::SHOW_SCAN_CONFIG));
    EXPECT_FALSE(CommandParser::requires_value(CommandType::SHOW_SCAN));
    EXPECT_FALSE(CommandParser::requires_value(CommandType::SHOW_SCAN_STAT));
    EXPECT_FALSE(CommandParser::requires_value(CommandType::FIND_TOKEN));
    EXPECT_FALSE(CommandParser::requires_value(CommandType::UNKNOWN));
}

TEST_F(CommandParserTest, RequiresDex) {
    EXPECT_FALSE(CommandParser::requires_dex(CommandType::HELP));
    EXPECT_FALSE(CommandParser::requires_dex(CommandType::VERSION_CMD));
    EXPECT_FALSE(CommandParser::requires_dex(CommandType::SCAN));
    EXPECT_FALSE(CommandParser::requires_dex(CommandType::SHOW_DEXES));
    EXPECT_TRUE(CommandParser::requires_dex(CommandType::SHOW_POOLS));
    EXPECT_TRUE(CommandParser::requires_dex(CommandType::SHOW_TOKENS));
    EXPECT_FALSE(CommandParser::requires_dex(CommandType::SHOW_SCAN_CONFIG));
    EXPECT_FALSE(CommandParser::requires_dex(CommandType::SHOW_SCAN));
    EXPECT_FALSE(CommandParser::requires_dex(CommandType::SHOW_SCAN_STAT));
    EXPECT_TRUE(CommandParser::requires_dex(CommandType::FIND_TOKEN));
    EXPECT_FALSE(CommandParser::requires_dex(CommandType::UNKNOWN));
}

TEST_F(CommandParserTest, RequiresToken) {
    EXPECT_FALSE(CommandParser::requires_token(CommandType::HELP));
    EXPECT_FALSE(CommandParser::requires_token(CommandType::VERSION_CMD));
    EXPECT_FALSE(CommandParser::requires_token(CommandType::SCAN));
    EXPECT_FALSE(CommandParser::requires_token(CommandType::SHOW_DEXES));
    EXPECT_FALSE(CommandParser::requires_token(CommandType::SHOW_POOLS));
    EXPECT_FALSE(CommandParser::requires_token(CommandType::SHOW_TOKENS));
    EXPECT_FALSE(CommandParser::requires_token(CommandType::SHOW_SCAN_CONFIG));
    EXPECT_FALSE(CommandParser::requires_token(CommandType::SHOW_SCAN));
    EXPECT_FALSE(CommandParser::requires_token(CommandType::SHOW_SCAN_STAT));
    EXPECT_TRUE(CommandParser::requires_token(CommandType::FIND_TOKEN));
    EXPECT_FALSE(CommandParser::requires_token(CommandType::UNKNOWN));
}

// Note: Private validation methods are tested indirectly through public methods

// Test command validation
TEST_F(CommandParserTest, ValidateCommandValid) {
    ParsedCommand cmd(CommandType::SCAN, "--scan", "ethereum", "1000");
    EXPECT_TRUE(CommandParser::validate_command(cmd));
}

TEST_F(CommandParserTest, ValidateCommandInvalidBlockchain) {
    ParsedCommand cmd(CommandType::SCAN, "--scan", "invalid", "1000");
    EXPECT_FALSE(CommandParser::validate_command(cmd));
}

TEST_F(CommandParserTest, ValidateCommandInvalidBlockRange) {
    ParsedCommand cmd(CommandType::SCAN, "--scan", "ethereum", "0");
    EXPECT_FALSE(CommandParser::validate_command(cmd));
}

TEST_F(CommandParserTest, ValidateCommandInvalidAddress) {
    ParsedCommand cmd(CommandType::FIND_TOKEN, "--find-token", "ethereum", "uniswap");
    cmd.token_address = "invalid";
    EXPECT_FALSE(CommandParser::validate_command(cmd));
}

// Test ParsedCommand constructors
TEST_F(CommandParserTest, ParsedCommandDefaultConstructor) {
    ParsedCommand cmd;
    EXPECT_EQ(cmd.type, CommandType::UNKNOWN);
    EXPECT_TRUE(cmd.flag.empty());
    EXPECT_TRUE(cmd.blockchain.empty());
    EXPECT_TRUE(cmd.value.empty());
    EXPECT_TRUE(cmd.dex_name.empty());
    EXPECT_TRUE(cmd.token_address.empty());
    EXPECT_FALSE(cmd.is_valid);
    EXPECT_TRUE(cmd.error_message.empty());
}

TEST_F(CommandParserTest, ParsedCommandParameterConstructor) {
    ParsedCommand cmd(CommandType::SCAN, "--scan", "ethereum", "1000");
    EXPECT_EQ(cmd.type, CommandType::SCAN);
    EXPECT_EQ(cmd.flag, "--scan");
    EXPECT_EQ(cmd.blockchain, "ethereum");
    EXPECT_EQ(cmd.value, "1000");
    EXPECT_TRUE(cmd.is_valid);
}

// Test edge cases
TEST_F(CommandParserTest, ParseWithEmptyString) {
    auto argv = create_argv({"program", ""});
    auto cmd = CommandParser::parse(argv.size(), argv.data());
    
    EXPECT_EQ(cmd.type, CommandType::UNKNOWN);
    EXPECT_EQ(cmd.flag, "");
    EXPECT_FALSE(cmd.is_valid);
}

TEST_F(CommandParserTest, ParseWithVeryLongFlag) {
    std::string long_flag(1000, 'a');
    auto argv = create_argv({"program", long_flag});
    auto cmd = CommandParser::parse(argv.size(), argv.data());
    
    EXPECT_EQ(cmd.type, CommandType::UNKNOWN);
    EXPECT_EQ(cmd.flag, long_flag);
    EXPECT_FALSE(cmd.is_valid);
}

} // namespace test
} // namespace cli
