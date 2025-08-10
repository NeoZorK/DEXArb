// test_config_manager.cpp
// Unit tests for config_manager module

#include <gtest/gtest.h>
#include <gmock/gmock.h>
#include <fstream>
#include <sstream>
#include <filesystem>
#include "config/config_manager.h"
#include "main.h"

class ConfigManagerTest : public ::testing::Test {
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
        "factory_address": "0x5C69bEe701ef814a2B6a3EDD4B1652CB9cc5aA6f"
      },
      {
        "factory_address": "0x7a250d5630B4cF539739dF2C5dAcb4c659F2488D"
      }
    ]
  },
  "fantom": {
    "dex": [
      {
        "factory_address": "0x152eE697f2E276fA89E96742e9bB9aB51FcFcA15"
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
    
    void createInvalidConfigFile() {
        std::ofstream config("neozork-config");
        config << "invalid json content";
        config.close();
    }
};

// Test load_dexes_from_config function
TEST_F(ConfigManagerTest, LoadDexesFromConfig_Success) {
    auto dexes = load_dexes_from_config();
    
    EXPECT_EQ(dexes.size(), 3);
    
    // Check first DEX
    EXPECT_EQ(dexes[0].name, "Unknown_5C69bE");
    EXPECT_EQ(dexes[0].factory_address, "0x5C69bEe701ef814a2B6a3EDD4B1652CB9cc5aA6f");
    
    // Check second DEX
    EXPECT_EQ(dexes[1].name, "Unknown_7a250d");
    EXPECT_EQ(dexes[1].factory_address, "0x7a250d5630B4cF539739dF2C5dAcb4c659F2488D");
    
    // Check third DEX
    EXPECT_EQ(dexes[2].name, "Unknown_152eE6");
    EXPECT_EQ(dexes[2].factory_address, "0x152eE697f2E276fA89E96742e9bB9aB51FcFcA15");
}

TEST_F(ConfigManagerTest, LoadDexesFromConfig_EmptyFile) {
    createEmptyConfigFile();
    auto dexes = load_dexes_from_config();
    EXPECT_EQ(dexes.size(), 0);
}

TEST_F(ConfigManagerTest, LoadDexesFromConfig_InvalidFile) {
    createInvalidConfigFile();
    auto dexes = load_dexes_from_config();
    EXPECT_EQ(dexes.size(), 0);
}

TEST_F(ConfigManagerTest, LoadDexesFromConfig_NoDexSection) {
    std::ofstream config("neozork-config");
    config << R"({
  "ethereum": {
    "rpc": "https://mainnet.infura.io/v3/123"
  }
})";
    config.close();
    
    auto dexes = load_dexes_from_config();
    EXPECT_EQ(dexes.size(), 0);
}

TEST_F(ConfigManagerTest, LoadDexesFromConfig_MalformedDexSection) {
    std::ofstream config("neozork-config");
    config << R"({
  "ethereum": {
    "dex": [
      {
        "factory_address": "0x5C69bEe701ef814a2B6a3EDD4B1652CB9cc5aA6f"
      },
      {
        "invalid_field": "value"
      }
    ]
  }
})";
    config.close();
    
    auto dexes = load_dexes_from_config();
    EXPECT_EQ(dexes.size(), 1); // Only first DEX should be loaded
    EXPECT_EQ(dexes[0].factory_address, "0x5C69bEe701ef814a2B6a3EDD4B1652CB9cc5aA6f");
}

TEST_F(ConfigManagerTest, LoadDexesFromConfig_ShortAddress) {
    std::ofstream config("neozork-config");
    config << R"({
  "ethereum": {
    "dex": [
      {
        "factory_address": "0x123"
      }
    ]
  }
})";
    config.close();
    
    auto dexes = load_dexes_from_config();
    EXPECT_EQ(dexes.size(), 1);
    EXPECT_EQ(dexes[0].name, "Unknown_0x123");
}

// Test update_config_with_dex function
TEST_F(ConfigManagerTest, UpdateConfigWithDex_EmptyDexList) {
    std::vector<RpcEndpoint> rpc_endpoints = {
        {"https://test.com", 1000}
    };
    std::vector<DexInfo> dex_list;
    FunctionStats stats;
    
    // This should not crash and should handle empty list gracefully
    EXPECT_NO_THROW(update_config_with_dex(rpc_endpoints, dex_list, stats));
}

TEST_F(ConfigManagerTest, UpdateConfigWithDex_InvalidRpcEndpoint) {
    std::vector<RpcEndpoint> rpc_endpoints = {
        {"", 0} // Invalid endpoint
    };
    std::vector<DexInfo> dex_list = {
        {"TestDEX", "0x1234567890123456789012345678901234567890", 0, 0, 0, 0, {}}
    };
    FunctionStats stats;
    
    // Should handle invalid RPC endpoint gracefully
    EXPECT_NO_THROW(update_config_with_dex(rpc_endpoints, dex_list, stats));
}

TEST_F(ConfigManagerTest, UpdateConfigWithDex_ConfigFileNotFound) {
    std::filesystem::remove("neozork-config");
    
    std::vector<RpcEndpoint> rpc_endpoints = {
        {"https://test.com", 1000}
    };
    std::vector<DexInfo> dex_list = {
        {"TestDEX", "0x1234567890123456789012345678901234567890", 0, 0, 0, 0, {}}
    };
    FunctionStats stats;
    
    // Should handle missing config file gracefully
    EXPECT_NO_THROW(update_config_with_dex(rpc_endpoints, dex_list, stats));
}

// Test edge cases
TEST_F(ConfigManagerTest, LoadDexesFromConfig_UnicodeContent) {
    std::ofstream config("neozork-config");
    config << R"({
  "ethereum": {
    "dex": [
      {
        "factory_address": "0x5C69bEe701ef814a2B6a3EDD4B1652CB9cc5aA6f"
      }
    ]
  },
  "comment": "Тест с кириллицей"
})";
    config.close();
    
    auto dexes = load_dexes_from_config();
    EXPECT_EQ(dexes.size(), 1);
    EXPECT_EQ(dexes[0].factory_address, "0x5C69bEe701ef814a2B6a3EDD4B1652CB9cc5aA6f");
}

TEST_F(ConfigManagerTest, LoadDexesFromConfig_VeryLongContent) {
    std::ofstream config("neozork-config");
    std::string long_content = "{\"ethereum\":{\"dex\":[";
    
    // Add many DEX entries
    for (int i = 0; i < 1000; ++i) {
        if (i > 0) long_content += ",";
        long_content += "{\"factory_address\":\"0x" + std::to_string(i) + "0000000000000000000000000000000000000000\"}";
    }
    long_content += "]}}";
    
    config << long_content;
    config.close();
    
    auto dexes = load_dexes_from_config();
    EXPECT_EQ(dexes.size(), 1000);
}

TEST_F(ConfigManagerTest, LoadDexesFromConfig_EmptyDexArray) {
    std::ofstream config("neozork-config");
    config << R"({
  "ethereum": {
    "dex": []
  }
})";
    config.close();
    
    auto dexes = load_dexes_from_config();
    EXPECT_EQ(dexes.size(), 0);
}

TEST_F(ConfigManagerTest, LoadDexesFromConfig_MissingFactoryAddress) {
    std::ofstream config("neozork-config");
    config << R"({
  "ethereum": {
    "dex": [
      {
        "name": "TestDEX"
      }
    ]
  }
})";
    config.close();
    
    auto dexes = load_dexes_from_config();
    EXPECT_EQ(dexes.size(), 0);
}

// Test timing and stats
TEST_F(ConfigManagerTest, UpdateConfigWithDex_ExecutionTime) {
    std::vector<RpcEndpoint> rpc_endpoints = {
        {"https://test.com", 1000}
    };
    std::vector<DexInfo> dex_list = {
        {"TestDEX", "0x1234567890123456789012345678901234567890", 0, 0, 0, 0, {}}
    };
    FunctionStats stats;
    
    update_config_with_dex(rpc_endpoints, dex_list, stats);
    
    // Should have recorded execution time
    EXPECT_GE(stats.execution_time_ms, 0.0);
}

// Test file size tracking
TEST_F(ConfigManagerTest, UpdateConfigWithDex_FileSizeTracking) {
    std::vector<RpcEndpoint> rpc_endpoints = {
        {"https://test.com", 1000}
    };
    std::vector<DexInfo> dex_list = {
        {"TestDEX", "0x1234567890123456789012345678901234567890", 0, 0, 0, 0, {}}
    };
    FunctionStats stats;
    
    update_config_with_dex(rpc_endpoints, dex_list, stats);
    
    // Should have recorded disk usage if file was written
    if (std::filesystem::exists("neozork-config")) {
        EXPECT_GT(stats.disk_usage_bytes, 0);
    }
}
