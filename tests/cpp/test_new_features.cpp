// test_new_features.cpp
// Unit tests for new CLI features

#include <gtest/gtest.h>
#include <iostream>
#include <string>
#include <vector>

// Include the command parser
#include "cli/command_parser.h"

using namespace cli;

class NewFeaturesTest : public ::testing::Test {
protected:
    void SetUp() override {
        // Setup code if needed
    }
    
    void TearDown() override {
        // Cleanup code if needed
    }
};

// Test -dexes flag
TEST_F(NewFeaturesTest, TestDexesFlag) {
    std::cout << "Testing -dexes flag..." << std::endl;
    
    // Test -dexes flag
    auto argv = create_argv({"neozork", "-dexes"});
    auto cmd = CommandParser::parse(argv.size(), argv.data());
    
    EXPECT_EQ(cmd.type, CommandType::SHOW_ALL_DEXES);
    EXPECT_EQ(cmd.flag, "-dexes");
    EXPECT_TRUE(cmd.is_valid);
    EXPECT_TRUE(cmd.error_message.empty());
    
    // Test --dexes flag
    auto argv2 = create_argv({"neozork", "--dexes"});
    auto cmd2 = CommandParser::parse(argv2.size(), argv2.data());
    
    EXPECT_EQ(cmd2.type, CommandType::SHOW_ALL_DEXES);
    EXPECT_EQ(cmd2.flag, "--dexes");
    EXPECT_TRUE(cmd2.is_valid);
    EXPECT_TRUE(cmd2.error_message.empty());
}

// Test expanded -dexes functionality
TEST_F(NewFeaturesTest, TestExpandedDexesFunctionality) {
    std::cout << "Testing expanded -dexes functionality..." << std::endl;
    
    // Test that the command parser correctly identifies the flag
    auto argv = create_argv({"neozork", "-dexes"});
    auto cmd = CommandParser::parse(argv.size(), argv.data());
    
    EXPECT_EQ(cmd.type, CommandType::SHOW_ALL_DEXES);
    EXPECT_TRUE(cmd.is_valid);
    EXPECT_TRUE(cmd.error_message.empty());
    
    // Test that the flag doesn't require additional parameters
    EXPECT_TRUE(cmd.blockchain.empty());
    EXPECT_TRUE(cmd.dex_name.empty());
    
    // Test that the command description is correct
    auto desc = CommandParser::get_command_description(CommandType::SHOW_ALL_DEXES);
    EXPECT_FALSE(desc.empty());
    EXPECT_EQ(desc, "Show all known DEXes by blockchain");
}

// Test -showPOOLS without DEX parameter
TEST_F(NewFeaturesTest, TestShowPoolsWithoutDex) {
    std::cout << "Testing -showPOOLS without DEX parameter..." << std::endl;
    
    // Test -showPOOLS with blockchain only
    auto argv = create_argv({"neozork", "-showPOOLS", "ethereum"});
    auto cmd = CommandParser::parse(argv.size(), argv.data());
    
    EXPECT_EQ(cmd.type, CommandType::SHOW_POOLS);
    EXPECT_EQ(cmd.flag, "-showPOOLS");
    EXPECT_EQ(cmd.blockchain, "ethereum");
    EXPECT_TRUE(cmd.dex_name.empty());
    EXPECT_TRUE(cmd.is_valid);
    EXPECT_TRUE(cmd.error_message.empty());
    
    // Test -showPOOLS with network ID
    auto argv2 = create_argv({"neozork", "-showPOOLS", "250"});
    auto cmd2 = CommandParser::parse(argv2.size(), argv2.data());
    
    EXPECT_EQ(cmd2.type, CommandType::SHOW_POOLS);
    EXPECT_EQ(cmd2.flag, "-showPOOLS");
    EXPECT_EQ(cmd2.blockchain, "fantom");
    EXPECT_TRUE(cmd2.dex_name.empty());
    EXPECT_TRUE(cmd2.is_valid);
    EXPECT_TRUE(cmd2.error_message.empty());
}

// Test -showTOKENS without DEX parameter
TEST_F(NewFeaturesTest, TestShowTokensWithoutDex) {
    std::cout << "Testing -showTOKENS without DEX parameter..." << std::endl;
    
    // Test -showTOKENS with blockchain only
    auto argv = create_argv({"neozork", "-showTOKENS", "bsc"});
    auto cmd = CommandParser::parse(argv.size(), argv.data());
    
    EXPECT_EQ(cmd.type, CommandType::SHOW_TOKENS);
    EXPECT_EQ(cmd.flag, "-showTOKENS");
    EXPECT_EQ(cmd.blockchain, "bsc");
    EXPECT_TRUE(cmd.dex_name.empty());
    EXPECT_TRUE(cmd.is_valid);
    EXPECT_TRUE(cmd.error_message.empty());
    
    // Test -showTOKENS with network ID
    auto argv2 = create_argv({"neozork", "-showTOKENS", "137"});
    auto cmd2 = CommandParser::parse(argv2.size(), argv2.data());
    
    EXPECT_EQ(cmd2.type, CommandType::SHOW_TOKENS);
    EXPECT_EQ(cmd2.flag, "-showTOKENS");
    EXPECT_EQ(cmd2.blockchain, "polygon");
    EXPECT_TRUE(cmd2.dex_name.empty());
    EXPECT_TRUE(cmd2.is_valid);
    EXPECT_TRUE(cmd2.error_message.empty());
}

// Test -showPOOLS with DEX parameter (existing functionality)
TEST_F(NewFeaturesTest, TestShowPoolsWithDex) {
    std::cout << "Testing -showPOOLS with DEX parameter..." << std::endl;
    
    // Test -showPOOLS with blockchain and DEX
    auto argv = create_argv({"neozork", "-showPOOLS", "ethereum", "Uniswap"});
    auto cmd = CommandParser::parse(argv.size(), argv.data());
    
    EXPECT_EQ(cmd.type, CommandType::SHOW_POOLS);
    EXPECT_EQ(cmd.flag, "-showPOOLS");
    EXPECT_EQ(cmd.blockchain, "ethereum");
    EXPECT_EQ(cmd.dex_name, "Uniswap");
    EXPECT_TRUE(cmd.is_valid);
    EXPECT_TRUE(cmd.error_message.empty());
}

// Test -showTOKENS with DEX parameter (existing functionality)
TEST_F(NewFeaturesTest, TestShowTokensWithDex) {
    std::cout << "Testing -showTOKENS with DEX parameter..." << std::endl;
    
    // Test -showTOKENS with blockchain and DEX
    auto argv = create_argv({"neozork", "-showTOKENS", "fantom", "SpookySwap"});
    auto cmd = CommandParser::parse(argv.size(), argv.data());
    
    EXPECT_EQ(cmd.type, CommandType::SHOW_TOKENS);
    EXPECT_EQ(cmd.flag, "-showTOKENS");
    EXPECT_EQ(cmd.blockchain, "fantom");
    EXPECT_EQ(cmd.dex_name, "SpookySwap");
    EXPECT_TRUE(cmd.is_valid);
    EXPECT_TRUE(cmd.error_message.empty());
}

// Test command descriptions
TEST_F(NewFeaturesTest, TestCommandDescriptions) {
    std::cout << "Testing command descriptions..." << std::endl;
    
    auto desc1 = CommandParser::get_command_description(CommandType::SHOW_ALL_DEXES);
    EXPECT_FALSE(desc1.empty());
    EXPECT_EQ(desc1, "Show all known DEXes by blockchain");
    
    auto desc2 = CommandParser::get_command_description(CommandType::SHOW_POOLS);
    EXPECT_FALSE(desc2.empty());
    EXPECT_EQ(desc2, "Show pools for specific DEX");
    
    auto desc3 = CommandParser::get_command_description(CommandType::SHOW_TOKENS);
    EXPECT_FALSE(desc3.empty());
    EXPECT_EQ(desc3, "Show tokens for specific DEX");
}

// Test requires_dex function
TEST_F(NewFeaturesTest, TestRequiresDex) {
    std::cout << "Testing requires_dex function..." << std::endl;
    
    // SHOW_POOLS should not require DEX anymore
    EXPECT_FALSE(CommandParser::requires_dex(CommandType::SHOW_POOLS));
    
    // SHOW_TOKENS should not require DEX anymore
    EXPECT_FALSE(CommandParser::requires_dex(CommandType::SHOW_TOKENS));
    
    // FIND_TOKEN should still require DEX
    EXPECT_TRUE(CommandParser::requires_dex(CommandType::FIND_TOKEN));
    
    // Other commands should not require DEX
    EXPECT_FALSE(CommandParser::requires_dex(CommandType::SHOW_DEXES));
    EXPECT_FALSE(CommandParser::requires_dex(CommandType::SHOW_ALL_DEXES));
    EXPECT_FALSE(CommandParser::requires_dex(CommandType::SCAN));
}

// Helper function to create argv array
std::vector<const char*> create_argv(const std::vector<std::string>& args) {
    std::vector<const char*> argv;
    for (const auto& arg : args) {
        argv.push_back(arg.c_str());
    }
    return argv;
}

// Main test runner
int main(int argc, char** argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
