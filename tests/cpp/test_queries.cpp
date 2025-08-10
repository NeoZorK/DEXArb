// test_queries.cpp
// Unit tests for queries module

#include <gtest/gtest.h>
#include <gmock/gmock.h>
#include <fstream>
#include <sstream>
#include <filesystem>
#include "network/queries.h"
#include "main.h"

class QueriesTest : public ::testing::Test {
protected:
    void SetUp() override {
        // Create test config file
        createTestConfigFile();
    }
    
    void TearDown() override {
        // Clean up test files
        if (std::filesystem::exists("neozork-config")) {
            std::filesystem::remove("neozork-config");
        }
    }
    
    void createTestConfigFile() {
        std::ofstream config("neozork-config");
        config << R"({
  "ethereum": {
    "dex": [
      {
        "factory_address": "0x5C69bEe701ef814a2B6a3EDD4B1652CB9cc5aA6f",
        "name": "UniswapV2"
      },
      {
        "factory_address": "0x7a250d5630B4cF539739dF2C5dAcb4c659F2488D",
        "name": "UniswapV2Router"
      }
    ]
  },
  "fantom": {
    "dex": [
      {
        "factory_address": "0x152eE697f2E276fA89E96742e9bB9aB51FcFcA15",
        "name": "SpookySwap"
      }
    ]
  }
})";
        config.close();
    }
    
    void createEmptyConfigFile() {
        std::ofstream config("neozork-config");
        config << "{}";
        config.close();
    }
};

// Test show_pools function
TEST_F(QueriesTest, ShowPools_ValidDexName) {
    std::vector<RpcEndpoint> rpc_endpoints = {{"https://test.com", 1000}};
    std::string dex_identifier = "UniswapV2";
    
    // Should not crash when called with valid DEX name
    EXPECT_NO_THROW(show_pools(rpc_endpoints, dex_identifier));
}

TEST_F(QueriesTest, ShowPools_ValidDexAddress) {
    std::vector<RpcEndpoint> rpc_endpoints = {{"https://test.com", 1000}};
    std::string dex_identifier = "0x5C69bEe701ef814a2B6a3EDD4B1652CB9cc5aA6f";
    
    // Should not crash when called with valid DEX address
    EXPECT_NO_THROW(show_pools(rpc_endpoints, dex_identifier));
}

TEST_F(QueriesTest, ShowPools_InvalidDex) {
    std::vector<RpcEndpoint> rpc_endpoints = {{"https://test.com", 1000}};
    std::string dex_identifier = "InvalidDEX";
    
    // Should not crash when called with invalid DEX
    EXPECT_NO_THROW(show_pools(rpc_endpoints, dex_identifier));
}

TEST_F(QueriesTest, ShowPools_EmptyDexIdentifier) {
    std::vector<RpcEndpoint> rpc_endpoints = {{"https://test.com", 1000}};
    std::string dex_identifier = "";
    
    // Should not crash when called with empty identifier
    EXPECT_NO_THROW(show_pools(rpc_endpoints, dex_identifier));
}

TEST_F(QueriesTest, ShowPools_EmptyRpcEndpoints) {
    std::vector<RpcEndpoint> rpc_endpoints;
    std::string dex_identifier = "UniswapV2";
    
    // Should not crash when called with empty RPC endpoints
    EXPECT_NO_THROW(show_pools(rpc_endpoints, dex_identifier));
}

// Test show_tokens function
TEST_F(QueriesTest, ShowTokens_ValidDexName) {
    std::vector<RpcEndpoint> rpc_endpoints = {{"https://test.com", 1000}};
    std::string dex_identifier = "UniswapV2";
    
    // Should not crash when called with valid DEX name
    EXPECT_NO_THROW(show_tokens(rpc_endpoints, dex_identifier));
}

TEST_F(QueriesTest, ShowTokens_ValidDexAddress) {
    std::vector<RpcEndpoint> rpc_endpoints = {{"https://test.com", 1000}};
    std::string dex_identifier = "0x5C69bEe701ef814a2B6a3EDD4B1652CB9cc5aA6f";
    
    // Should not crash when called with valid DEX address
    EXPECT_NO_THROW(show_tokens(rpc_endpoints, dex_identifier));
}

TEST_F(QueriesTest, ShowTokens_InvalidDex) {
    std::vector<RpcEndpoint> rpc_endpoints = {{"https://test.com", 1000}};
    std::string dex_identifier = "InvalidDEX";
    
    // Should not crash when called with invalid DEX
    EXPECT_NO_THROW(show_tokens(rpc_endpoints, dex_identifier));
}

TEST_F(QueriesTest, ShowTokens_EmptyDexIdentifier) {
    std::vector<RpcEndpoint> rpc_endpoints = {{"https://test.com", 1000}};
    std::string dex_identifier = "";
    
    // Should not crash when called with empty identifier
    EXPECT_NO_THROW(show_tokens(rpc_endpoints, dex_identifier));
}

// Test find_token_in_dex function
TEST_F(QueriesTest, FindTokenInDex_ValidDexAndToken) {
    std::vector<RpcEndpoint> rpc_endpoints = {{"https://test.com", 1000}};
    std::string dex_identifier = "UniswapV2";
    std::string token_address = "0x1234567890123456789012345678901234567890";
    
    // Should not crash when called with valid parameters
    EXPECT_NO_THROW(find_token_in_dex(rpc_endpoints, dex_identifier, token_address));
}

TEST_F(QueriesTest, FindTokenInDex_InvalidDex) {
    std::vector<RpcEndpoint> rpc_endpoints = {{"https://test.com", 1000}};
    std::string dex_identifier = "InvalidDEX";
    std::string token_address = "0x1234567890123456789012345678901234567890";
    
    // Should not crash when called with invalid DEX
    EXPECT_NO_THROW(find_token_in_dex(rpc_endpoints, dex_identifier, token_address));
}

TEST_F(QueriesTest, FindTokenInDex_EmptyTokenAddress) {
    std::vector<RpcEndpoint> rpc_endpoints = {{"https://test.com", 1000}};
    std::string dex_identifier = "UniswapV2";
    std::string token_address = "";
    
    // Should not crash when called with empty token address
    EXPECT_NO_THROW(find_token_in_dex(rpc_endpoints, dex_identifier, token_address));
}

TEST_F(QueriesTest, FindTokenInDex_EmptyDexIdentifier) {
    std::vector<RpcEndpoint> rpc_endpoints = {{"https://test.com", 1000}};
    std::string dex_identifier = "";
    std::string token_address = "0x1234567890123456789012345678901234567890";
    
    // Should not crash when called with empty DEX identifier
    EXPECT_NO_THROW(find_token_in_dex(rpc_endpoints, dex_identifier, token_address));
}

// Test find_tokens_across_dexes function
TEST_F(QueriesTest, FindTokensAcrossDexes_ValidToken) {
    std::vector<RpcEndpoint> rpc_endpoints = {{"https://test.com", 1000}};
    std::string token_address = "0x1234567890123456789012345678901234567890";
    
    // Should not crash when called with valid token address
    EXPECT_NO_THROW(find_tokens_across_dexes(rpc_endpoints, token_address));
}

TEST_F(QueriesTest, FindTokensAcrossDexes_EmptyTokenAddress) {
    std::vector<RpcEndpoint> rpc_endpoints = {{"https://test.com", 1000}};
    std::string token_address = "";
    
    // Should not crash when called with empty token address
    EXPECT_NO_THROW(find_tokens_across_dexes(rpc_endpoints, token_address));
}

TEST_F(QueriesTest, FindTokensAcrossDexes_EmptyRpcEndpoints) {
    std::vector<RpcEndpoint> rpc_endpoints;
    std::string token_address = "0x1234567890123456789012345678901234567890";
    
    // Should not crash when called with empty RPC endpoints
    EXPECT_NO_THROW(find_tokens_across_dexes(rpc_endpoints, token_address));
}

// Test show_dexes function
TEST_F(QueriesTest, ShowDexes_ValidConfig) {
    std::vector<RpcEndpoint> rpc_endpoints = {{"https://test.com", 1000}};
    
    // Should not crash when called with valid config
    EXPECT_NO_THROW(show_dexes(rpc_endpoints));
}

TEST_F(QueriesTest, ShowDexes_EmptyRpcEndpoints) {
    std::vector<RpcEndpoint> rpc_endpoints;
    
    // Should not crash when called with empty RPC endpoints
    EXPECT_NO_THROW(show_dexes(rpc_endpoints));
}

TEST_F(QueriesTest, ShowDexes_EmptyConfig) {
    createEmptyConfigFile();
    std::vector<RpcEndpoint> rpc_endpoints = {{"https://test.com", 1000}};
    
    // Should not crash when called with empty config
    EXPECT_NO_THROW(show_dexes(rpc_endpoints));
}

// Test show_all_tokens function
TEST_F(QueriesTest, ShowAllTokens_ValidConfig) {
    std::vector<RpcEndpoint> rpc_endpoints = {{"https://test.com", 1000}};
    
    // Should not crash when called with valid config
    EXPECT_NO_THROW(show_all_tokens(rpc_endpoints));
}

TEST_F(QueriesTest, ShowAllTokens_EmptyRpcEndpoints) {
    std::vector<RpcEndpoint> rpc_endpoints;
    
    // Should not crash when called with empty RPC endpoints
    EXPECT_NO_THROW(show_all_tokens(rpc_endpoints));
}

TEST_F(QueriesTest, ShowAllTokens_EmptyConfig) {
    createEmptyConfigFile();
    std::vector<RpcEndpoint> rpc_endpoints = {{"https://test.com", 1000}};
    
    // Should not crash when called with empty config
    EXPECT_NO_THROW(show_all_tokens(rpc_endpoints));
}

// Test edge cases
TEST_F(QueriesTest, ShowPools_UnicodeDexName) {
    std::vector<RpcEndpoint> rpc_endpoints = {{"https://test.com", 1000}};
    std::string dex_identifier = "ТестDEX";
    
    // Should not crash when called with Unicode DEX name
    EXPECT_NO_THROW(show_pools(rpc_endpoints, dex_identifier));
}

TEST_F(QueriesTest, FindTokenInDex_UnicodeTokenAddress) {
    std::vector<RpcEndpoint> rpc_endpoints = {{"https://test.com", 1000}};
    std::string dex_identifier = "UniswapV2";
    std::string token_address = "0xТестТокен";
    
    // Should not crash when called with Unicode token address
    EXPECT_NO_THROW(find_token_in_dex(rpc_endpoints, dex_identifier, token_address));
}

TEST_F(QueriesTest, ShowPools_VeryLongDexName) {
    std::vector<RpcEndpoint> rpc_endpoints = {{"https://test.com", 1000}};
    std::string dex_identifier = std::string(1000, 'A');
    
    // Should not crash when called with very long DEX name
    EXPECT_NO_THROW(show_pools(rpc_endpoints, dex_identifier));
}

TEST_F(QueriesTest, FindTokenInDex_VeryLongTokenAddress) {
    std::vector<RpcEndpoint> rpc_endpoints = {{"https://test.com", 1000}};
    std::string dex_identifier = "UniswapV2";
    std::string token_address = std::string(1000, 'B');
    
    // Should not crash when called with very long token address
    EXPECT_NO_THROW(find_token_in_dex(rpc_endpoints, dex_identifier, token_address));
}

// Test with missing config file
TEST_F(QueriesTest, ShowPools_MissingConfigFile) {
    std::filesystem::remove("neozork-config");
    std::vector<RpcEndpoint> rpc_endpoints = {{"https://test.com", 1000}};
    std::string dex_identifier = "UniswapV2";
    
    // Should not crash when config file is missing
    EXPECT_NO_THROW(show_pools(rpc_endpoints, dex_identifier));
}

TEST_F(QueriesTest, ShowTokens_MissingConfigFile) {
    std::filesystem::remove("neozork-config");
    std::vector<RpcEndpoint> rpc_endpoints = {{"https://test.com", 1000}};
    std::string dex_identifier = "UniswapV2";
    
    // Should not crash when config file is missing
    EXPECT_NO_THROW(show_tokens(rpc_endpoints, dex_identifier));
}

TEST_F(QueriesTest, FindTokenInDex_MissingConfigFile) {
    std::filesystem::remove("neozork-config");
    std::vector<RpcEndpoint> rpc_endpoints = {{"https://test.com", 1000}};
    std::string dex_identifier = "UniswapV2";
    std::string token_address = "0x1234567890123456789012345678901234567890";
    
    // Should not crash when config file is missing
    EXPECT_NO_THROW(find_token_in_dex(rpc_endpoints, dex_identifier, token_address));
}

TEST_F(QueriesTest, FindTokensAcrossDexes_MissingConfigFile) {
    std::filesystem::remove("neozork-config");
    std::vector<RpcEndpoint> rpc_endpoints = {{"https://test.com", 1000}};
    std::string token_address = "0x1234567890123456789012345678901234567890";
    
    // Should not crash when config file is missing
    EXPECT_NO_THROW(find_tokens_across_dexes(rpc_endpoints, token_address));
}

TEST_F(QueriesTest, ShowDexes_MissingConfigFile) {
    std::filesystem::remove("neozork-config");
    std::vector<RpcEndpoint> rpc_endpoints = {{"https://test.com", 1000}};
    
    // Should not crash when config file is missing
    EXPECT_NO_THROW(show_dexes(rpc_endpoints));
}

TEST_F(QueriesTest, ShowAllTokens_MissingConfigFile) {
    std::filesystem::remove("neozork-config");
    std::vector<RpcEndpoint> rpc_endpoints = {{"https://test.com", 1000}};
    
    // Should not crash when config file is missing
    EXPECT_NO_THROW(show_all_tokens(rpc_endpoints));
}
