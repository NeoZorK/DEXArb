// test_new_features.cpp
// Unit tests for new CLI features

#include <gtest/gtest.h>
#include <gmock/gmock.h>
#include <iostream>
#include <sstream>
#include "../include/cli/command_parser.h"
#include "../include/cli/help_display.h"

class NewFeaturesTest : public ::testing::Test {
protected:
    void SetUp() override {
        // Setup for tests
    }
    
    void TearDown() override {
        // Cleanup after tests
    }
};

// Test network ID to blockchain conversion
TEST_F(NewFeaturesTest, NetworkIdToBlockchain) {
    EXPECT_EQ(cli::CommandParser::network_id_to_blockchain("1"), "ethereum");
    EXPECT_EQ(cli::CommandParser::network_id_to_blockchain("56"), "bsc");
    EXPECT_EQ(cli::CommandParser::network_id_to_blockchain("137"), "polygon");
    EXPECT_EQ(cli::CommandParser::network_id_to_blockchain("250"), "fantom");
    EXPECT_EQ(cli::CommandParser::network_id_to_blockchain("43114"), "avalanche");
    EXPECT_EQ(cli::CommandParser::network_id_to_blockchain("101"), "solana");
}

// Test blockchain to network ID conversion
TEST_F(NewFeaturesTest, BlockchainToNetworkId) {
    EXPECT_EQ(cli::CommandParser::blockchain_to_network_id("ethereum"), "1");
    EXPECT_EQ(cli::CommandParser::blockchain_to_network_id("bsc"), "56");
    EXPECT_EQ(cli::CommandParser::blockchain_to_network_id("polygon"), "137");
    EXPECT_EQ(cli::CommandParser::blockchain_to_network_id("fantom"), "250");
    EXPECT_EQ(cli::CommandParser::blockchain_to_network_id("avalanche"), "43114");
    EXPECT_EQ(cli::CommandParser::blockchain_to_network_id("solana"), "101");
}

// Test network ID validation
TEST_F(NewFeaturesTest, IsNetworkId) {
    EXPECT_TRUE(cli::CommandParser::is_network_id("1"));
    EXPECT_TRUE(cli::CommandParser::is_network_id("56"));
    EXPECT_TRUE(cli::CommandParser::is_network_id("137"));
    EXPECT_TRUE(cli::CommandParser::is_network_id("250"));
    EXPECT_TRUE(cli::CommandParser::is_network_id("43114"));
    EXPECT_TRUE(cli::CommandParser::is_network_id("101"));
    
    EXPECT_FALSE(cli::CommandParser::is_network_id("ethereum"));
    EXPECT_FALSE(cli::CommandParser::is_network_id("fantom"));
    EXPECT_FALSE(cli::CommandParser::is_network_id("invalid"));
    EXPECT_FALSE(cli::CommandParser::is_network_id("0"));
    EXPECT_FALSE(cli::CommandParser::is_network_id("999"));
}

// Test default values
TEST_F(NewFeaturesTest, DefaultValues) {
    EXPECT_EQ(cli::CommandParser::get_default_blockchain(), "fantom");
    EXPECT_EQ(cli::CommandParser::get_default_block_count(), "1000");
}

// Test command parsing with network IDs
TEST_F(NewFeaturesTest, CommandParsingWithNetworkIds) {
    // Test scan command with network ID
    const char* scan_argv[] = {"program", "-scan", "250", "5000"};
    auto scan_cmd = cli::CommandParser::parse(4, scan_argv);
    EXPECT_TRUE(scan_cmd.is_valid);
    EXPECT_EQ(scan_cmd.type, cli::CommandType::SCAN);
    EXPECT_EQ(scan_cmd.blockchain, "fantom");
    EXPECT_EQ(scan_cmd.value, "5000");
    
    // Test showDEXES command with network ID
    const char* dexes_argv[] = {"program", "-showDEXES", "1"};
    auto dexes_cmd = cli::CommandParser::parse(3, dexes_argv);
    EXPECT_TRUE(dexes_cmd.is_valid);
    EXPECT_EQ(dexes_cmd.type, cli::CommandType::SHOW_DEXES);
    EXPECT_EQ(dexes_cmd.blockchain, "ethereum");
}

// Test command parsing with default values
TEST_F(NewFeaturesTest, CommandParsingWithDefaults) {
    // Test scan command with default blockchain and block count
    const char* scan_argv[] = {"program", "-scan"};
    auto scan_cmd = cli::CommandParser::parse(2, scan_argv);
    EXPECT_TRUE(scan_cmd.is_valid);
    EXPECT_EQ(scan_cmd.type, cli::CommandType::SCAN);
    EXPECT_EQ(scan_cmd.blockchain, "fantom");
    EXPECT_EQ(scan_cmd.value, "1000");
    
    // Test scan command with default block count
    const char* scan2_argv[] = {"program", "-scan", "fantom"};
    auto scan2_cmd = cli::CommandParser::parse(3, scan2_argv);
    EXPECT_TRUE(scan2_cmd.is_valid);
    EXPECT_EQ(scan2_cmd.type, cli::CommandType::SCAN);
    EXPECT_EQ(scan2_cmd.blockchain, "fantom");
    EXPECT_EQ(scan2_cmd.value, "1000");
}

// Test examples command
TEST_F(NewFeaturesTest, ExamplesCommand) {
    const char* examples_argv[] = {"program", "-examples"};
    auto examples_cmd = cli::CommandParser::parse(2, examples_argv);
    EXPECT_TRUE(examples_cmd.is_valid);
    EXPECT_EQ(examples_cmd.type, cli::CommandType::EXAMPLES);
}

// Test help display formatting
TEST_F(NewFeaturesTest, HelpDisplayFormatting) {
    std::stringstream output;
    cli::HelpDisplay::show_examples(output);
    std::string result = output.str();
    
    // Check that output contains expected sections
    EXPECT_TRUE(result.find("🚀 DETAILED EXAMPLES") != std::string::npos);
    EXPECT_TRUE(result.find("📋 Help & Information") != std::string::npos);
    EXPECT_TRUE(result.find("🔍 Scanning & Discovery") != std::string::npos);
    EXPECT_TRUE(result.find("🏦 DEX Analysis") != std::string::npos);
    EXPECT_TRUE(result.find("🔎 Token Search") != std::string::npos);
    EXPECT_TRUE(result.find("📊 Configuration & Statistics") != std::string::npos);
    EXPECT_TRUE(result.find("🌐 Supported Network IDs") != std::string::npos);
    
    // Check that output contains network ID examples
    EXPECT_TRUE(result.find("neozork -scan 250 5000") != std::string::npos);
    EXPECT_TRUE(result.find("neozork -scan 1 50000") != std::string::npos);
    EXPECT_TRUE(result.find("neozork -showDEXES 250") != std::string::npos);
}

// Test error handling
TEST_F(NewFeaturesTest, ErrorHandling) {
    // Test invalid network ID
    const char* invalid_argv[] = {"program", "-scan", "999", "1000"};
    auto invalid_cmd = cli::CommandParser::parse(4, invalid_argv);
    EXPECT_FALSE(invalid_cmd.is_valid);
    
    // Test invalid block range
    const char* invalid_range_argv[] = {"program", "-scan", "fantom", "500"};
    auto invalid_range_cmd = cli::CommandParser::parse(4, invalid_range_argv);
    EXPECT_FALSE(invalid_range_cmd.is_valid);
}

// Test command type conversion
TEST_F(NewFeaturesTest, CommandTypeConversion) {
    EXPECT_EQ(cli::CommandParser::string_to_command_type("-examples"), cli::CommandType::EXAMPLES);
    EXPECT_EQ(cli::CommandParser::string_to_command_type("--examples"), cli::CommandType::EXAMPLES);
    EXPECT_EQ(cli::CommandParser::string_to_command_type("-scan"), cli::CommandType::SCAN);
    EXPECT_EQ(cli::CommandParser::string_to_command_type("--scan"), cli::CommandType::SCAN);
}

// Test command descriptions
TEST_F(NewFeaturesTest, CommandDescriptions) {
    EXPECT_EQ(cli::CommandParser::get_command_description(cli::CommandType::EXAMPLES), 
              "Display detailed examples");
    EXPECT_EQ(cli::CommandParser::get_command_description(cli::CommandType::SCAN), 
              "Scan blockchain for arbitrage opportunities");
}

// Test command requirements
TEST_F(NewFeaturesTest, CommandRequirements) {
    EXPECT_FALSE(cli::CommandParser::requires_blockchain(cli::CommandType::EXAMPLES));
    EXPECT_TRUE(cli::CommandParser::requires_blockchain(cli::CommandType::SCAN));
    EXPECT_TRUE(cli::CommandParser::requires_value(cli::CommandType::SCAN));
    EXPECT_FALSE(cli::CommandParser::requires_value(cli::CommandType::EXAMPLES));
}

int main(int argc, char** argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
