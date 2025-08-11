// test_application.cpp
// Unit tests for application module

#include <gtest/gtest.h>
#include <gmock/gmock.h>
#include <string>
#include <vector>
#include "core/application.h"
#include "cli/command_parser.h"

class ApplicationTest : public ::testing::Test {
protected:
    void SetUp() override {
        app = std::make_unique<core::Application>();
    }
    
    void TearDown() override {
        app.reset();
    }
    
    std::unique_ptr<core::Application> app;
};

// Test Application constructor and destructor
TEST_F(ApplicationTest, ConstructorDestructor) {
    EXPECT_FALSE(app->is_initialized());
}

// Test initialize method
TEST_F(ApplicationTest, Initialize_Success) {
    EXPECT_TRUE(app->initialize());
    EXPECT_TRUE(app->is_initialized());
}

TEST_F(ApplicationTest, Initialize_AlreadyInitialized) {
    EXPECT_TRUE(app->initialize());
    EXPECT_TRUE(app->initialize()); // Should not fail
    EXPECT_TRUE(app->is_initialized());
}

// Test shutdown method
TEST_F(ApplicationTest, Shutdown_WhenInitialized) {
    app->initialize();
    app->shutdown();
    EXPECT_FALSE(app->is_initialized());
}

TEST_F(ApplicationTest, Shutdown_WhenNotInitialized) {
    app->shutdown(); // Should not crash
    EXPECT_FALSE(app->is_initialized());
}

// Test get_version method
TEST_F(ApplicationTest, GetVersion) {
    std::string version = app->get_version();
    EXPECT_EQ(version, "1.0.7");
}

// Test get_build_info method
TEST_F(ApplicationTest, GetBuildInfo) {
    std::string build_info = app->get_build_info();
    EXPECT_EQ(build_info, "C++23, CMake 3.28, Cross-platform");
}

// Test handle_help_command method
TEST_F(ApplicationTest, HandleHelpCommand) {
    int result = app->handle_help_command();
    EXPECT_EQ(result, 0);
}

// Test handle_version_command method
TEST_F(ApplicationTest, HandleVersionCommand) {
    int result = app->handle_version_command();
    EXPECT_EQ(result, 0);
}

// Test handle_scan_command method
TEST_F(ApplicationTest, HandleScanCommand) {
    cli::ParsedCommand cmd;
    cmd.type = cli::CommandType::SCAN;
    cmd.blockchain = "ethereum";
    cmd.value = "1000";
    
    int result = app->handle_scan_command(cmd);
    EXPECT_EQ(result, 0);
}

// Test handle_show_command method for different command types
TEST_F(ApplicationTest, HandleShowCommand_ShowDexes) {
    cli::ParsedCommand cmd;
    cmd.type = cli::CommandType::SHOW_DEXES;
    cmd.blockchain = "ethereum";
    
    int result = app->handle_show_command(cmd);
    EXPECT_EQ(result, 0);
}

TEST_F(ApplicationTest, HandleShowCommand_ShowPools) {
    cli::ParsedCommand cmd;
    cmd.type = cli::CommandType::SHOW_POOLS;
    cmd.blockchain = "ethereum";
    cmd.dex_name = "UniswapV2";
    
    int result = app->handle_show_command(cmd);
    EXPECT_EQ(result, 0);
}

TEST_F(ApplicationTest, HandleShowCommand_ShowTokens) {
    cli::ParsedCommand cmd;
    cmd.type = cli::CommandType::SHOW_TOKENS;
    cmd.blockchain = "ethereum";
    cmd.dex_name = "UniswapV2";
    
    int result = app->handle_show_command(cmd);
    EXPECT_EQ(result, 0);
}

TEST_F(ApplicationTest, HandleShowCommand_ShowScanConfig) {
    cli::ParsedCommand cmd;
    cmd.type = cli::CommandType::SHOW_SCAN_CONFIG;
    cmd.blockchain = "ethereum";
    
    int result = app->handle_show_command(cmd);
    EXPECT_EQ(result, 0);
}

TEST_F(ApplicationTest, HandleShowCommand_ShowScan) {
    cli::ParsedCommand cmd;
    cmd.type = cli::CommandType::SHOW_SCAN;
    cmd.blockchain = "ethereum";
    
    int result = app->handle_show_command(cmd);
    EXPECT_EQ(result, 0);
}

TEST_F(ApplicationTest, HandleShowCommand_ShowScanStat) {
    cli::ParsedCommand cmd;
    cmd.type = cli::CommandType::SHOW_SCAN_STAT;
    cmd.blockchain = "ethereum";
    
    int result = app->handle_show_command(cmd);
    EXPECT_EQ(result, 0);
}

TEST_F(ApplicationTest, HandleShowCommand_UnknownType) {
    cli::ParsedCommand cmd;
    cmd.type = static_cast<cli::CommandType>(999); // Invalid type
    
    int result = app->handle_show_command(cmd);
    EXPECT_EQ(result, 1);
}

// Test handle_find_command method
TEST_F(ApplicationTest, HandleFindCommand_FindToken) {
    cli::ParsedCommand cmd;
    cmd.type = cli::CommandType::FIND_TOKEN;
    cmd.blockchain = "ethereum";
    cmd.dex_name = "UniswapV2";
    cmd.token_address = "0x1234567890123456789012345678901234567890";
    
    int result = app->handle_find_command(cmd);
    EXPECT_EQ(result, 0);
}

TEST_F(ApplicationTest, HandleFindCommand_FindTokens) {
    cli::ParsedCommand cmd;
    cmd.type = cli::CommandType::FIND_TOKENS;
    cmd.blockchain = "ethereum";
    cmd.token_address = "0x1234567890123456789012345678901234567890";
    
    int result = app->handle_find_command(cmd);
    EXPECT_EQ(result, 0);
}

TEST_F(ApplicationTest, HandleFindCommand_UnknownType) {
    cli::ParsedCommand cmd;
    cmd.type = static_cast<cli::CommandType>(999); // Invalid type
    
    int result = app->handle_find_command(cmd);
    EXPECT_EQ(result, 1);
}

// Test handle_unknown_command method
TEST_F(ApplicationTest, HandleUnknownCommand) {
    cli::ParsedCommand cmd;
    cmd.flag = "unknown_flag";
    
    int result = app->handle_unknown_command(cmd);
    EXPECT_EQ(result, 1);
}

// Test execute_scan method
TEST_F(ApplicationTest, ExecuteScan_ValidParameters) {
    int result = app->execute_scan("ethereum", "1000");
    EXPECT_EQ(result, 0);
}

TEST_F(ApplicationTest, ExecuteScan_EmptyBlockchain) {
    int result = app->execute_scan("", "1000");
    EXPECT_EQ(result, 0);
}

TEST_F(ApplicationTest, ExecuteScan_EmptyBlocks) {
    int result = app->execute_scan("ethereum", "");
    EXPECT_EQ(result, 0);
}

// Test execute_show_dexes method
TEST_F(ApplicationTest, ExecuteShowDexes_ValidBlockchain) {
    int result = app->execute_show_dexes("ethereum");
    EXPECT_EQ(result, 0);
}

TEST_F(ApplicationTest, ExecuteShowDexes_EmptyBlockchain) {
    int result = app->execute_show_dexes("");
    EXPECT_EQ(result, 0);
}

// Test execute_show_pools method
TEST_F(ApplicationTest, ExecuteShowPools_ValidParameters) {
    int result = app->execute_show_pools("ethereum", "UniswapV2");
    EXPECT_EQ(result, 0);
}

TEST_F(ApplicationTest, ExecuteShowPools_EmptyBlockchain) {
    int result = app->execute_show_pools("", "UniswapV2");
    EXPECT_EQ(result, 0);
}

TEST_F(ApplicationTest, ExecuteShowPools_EmptyDexName) {
    int result = app->execute_show_pools("ethereum", "");
    EXPECT_EQ(result, 0);
}

// Test execute_show_tokens method
TEST_F(ApplicationTest, ExecuteShowTokens_ValidParameters) {
    int result = app->execute_show_tokens("ethereum", "UniswapV2");
    EXPECT_EQ(result, 0);
}

TEST_F(ApplicationTest, ExecuteShowTokens_EmptyBlockchain) {
    int result = app->execute_show_tokens("", "UniswapV2");
    EXPECT_EQ(result, 0);
}

TEST_F(ApplicationTest, ExecuteShowTokens_EmptyDexName) {
    int result = app->execute_show_tokens("ethereum", "");
    EXPECT_EQ(result, 0);
}

// Test execute_show_scan_config method
TEST_F(ApplicationTest, ExecuteShowScanConfig_ValidBlockchain) {
    int result = app->execute_show_scan_config("ethereum");
    EXPECT_EQ(result, 0);
}

TEST_F(ApplicationTest, ExecuteShowScanConfig_EmptyBlockchain) {
    int result = app->execute_show_scan_config("");
    EXPECT_EQ(result, 0);
}

// Test execute_show_scan method
TEST_F(ApplicationTest, ExecuteShowScan_ValidBlockchain) {
    int result = app->execute_show_scan("ethereum");
    EXPECT_EQ(result, 0);
}

TEST_F(ApplicationTest, ExecuteShowScan_EmptyBlockchain) {
    int result = app->execute_show_scan("");
    EXPECT_EQ(result, 0);
}

// Test execute_show_scan_stat method
TEST_F(ApplicationTest, ExecuteShowScanStat_ValidBlockchain) {
    int result = app->execute_show_scan_stat("ethereum");
    EXPECT_EQ(result, 0);
}

TEST_F(ApplicationTest, ExecuteShowScanStat_EmptyBlockchain) {
    int result = app->execute_show_scan_stat("");
    EXPECT_EQ(result, 0);
}

// Test execute_find_token method
TEST_F(ApplicationTest, ExecuteFindToken_ValidParameters) {
    int result = app->execute_find_token("ethereum", "UniswapV2", "0x1234567890123456789012345678901234567890");
    EXPECT_EQ(result, 0);
}

TEST_F(ApplicationTest, ExecuteFindToken_EmptyBlockchain) {
    int result = app->execute_find_token("", "UniswapV2", "0x1234567890123456789012345678901234567890");
    EXPECT_EQ(result, 0);
}

TEST_F(ApplicationTest, ExecuteFindToken_EmptyDexName) {
    int result = app->execute_find_token("ethereum", "", "0x1234567890123456789012345678901234567890");
    EXPECT_EQ(result, 0);
}

TEST_F(ApplicationTest, ExecuteFindToken_EmptyTokenAddress) {
    int result = app->execute_find_token("ethereum", "UniswapV2", "");
    EXPECT_EQ(result, 0);
}

// Test execute_find_tokens method
TEST_F(ApplicationTest, ExecuteFindTokens_ValidParameters) {
    int result = app->execute_find_tokens("ethereum", "0x1234567890123456789012345678901234567890");
    EXPECT_EQ(result, 0);
}

TEST_F(ApplicationTest, ExecuteFindTokens_EmptyBlockchain) {
    int result = app->execute_find_tokens("", "0x1234567890123456789012345678901234567890");
    EXPECT_EQ(result, 0);
}

TEST_F(ApplicationTest, ExecuteFindTokens_EmptyTokenAddress) {
    int result = app->execute_find_tokens("ethereum", "");
    EXPECT_EQ(result, 0);
}

// Test validate_scan_parameters method
TEST_F(ApplicationTest, ValidateScanParameters_ValidParameters) {
    bool result = app->validate_scan_parameters("ethereum", "1000");
    EXPECT_TRUE(result);
}

TEST_F(ApplicationTest, ValidateScanParameters_EmptyBlockchain) {
    bool result = app->validate_scan_parameters("", "1000");
    EXPECT_TRUE(result);
}

TEST_F(ApplicationTest, ValidateScanParameters_EmptyBlocks) {
    bool result = app->validate_scan_parameters("ethereum", "");
    EXPECT_TRUE(result);
}

// Test logging methods
TEST_F(ApplicationTest, LogError) {
    EXPECT_NO_THROW(app->log_error("Test error message"));
}

TEST_F(ApplicationTest, LogWarning) {
    EXPECT_NO_THROW(app->log_warning("Test warning message"));
}

TEST_F(ApplicationTest, LogInfo) {
    EXPECT_NO_THROW(app->log_info("Test info message"));
}

// Test edge cases
TEST_F(ApplicationTest, ExecuteScan_UnicodeParameters) {
    int result = app->execute_scan("эфириум", "1000");
    EXPECT_EQ(result, 0);
}

TEST_F(ApplicationTest, ExecuteShowDexes_UnicodeBlockchain) {
    int result = app->execute_show_dexes("эфириум");
    EXPECT_EQ(result, 0);
}

TEST_F(ApplicationTest, ExecuteShowPools_UnicodeParameters) {
    int result = app->execute_show_pools("эфириум", "ЮнисвапV2");
    EXPECT_EQ(result, 0);
}

// Test with very long parameters
TEST_F(ApplicationTest, ExecuteScan_VeryLongParameters) {
    std::string long_blockchain(1000, 'A');
    std::string long_blocks(1000, 'B');
    
    int result = app->execute_scan(long_blockchain, long_blocks);
    EXPECT_EQ(result, 0);
}

TEST_F(ApplicationTest, ExecuteShowPools_VeryLongParameters) {
    std::string long_blockchain(1000, 'A');
    std::string long_dex_name(1000, 'B');
    
    int result = app->execute_show_pools(long_blockchain, long_dex_name);
    EXPECT_EQ(result, 0);
}

// Test application lifecycle
TEST_F(ApplicationTest, ApplicationLifecycle) {
    // Create new application
    auto new_app = std::make_unique<core::Application>();
    
    // Should not be initialized initially
    EXPECT_FALSE(new_app->is_initialized());
    
    // Initialize
    EXPECT_TRUE(new_app->initialize());
    EXPECT_TRUE(new_app->is_initialized());
    
    // Shutdown
    new_app->shutdown();
    EXPECT_FALSE(new_app->is_initialized());
    
    // Re-initialize
    EXPECT_TRUE(new_app->initialize());
    EXPECT_TRUE(new_app->is_initialized());
}
