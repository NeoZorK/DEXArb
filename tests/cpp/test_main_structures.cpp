#include <gtest/gtest.h>
#include <iostream>
#include <string>
#include <vector>

// Include the main header
#include "main.h"

class MainStructuresTest : public ::testing::Test {
protected:
    void SetUp() override {
        // Setup test data
    }
    
    void TearDown() override {
        // Cleanup if needed
    }
};

// Test BlockchainType enum
TEST_F(MainStructuresTest, BlockchainTypeEnum) {
    EXPECT_EQ(static_cast<int>(BlockchainType::Ethereum), 0);
    EXPECT_EQ(static_cast<int>(BlockchainType::Fantom), 1);
    EXPECT_EQ(static_cast<int>(BlockchainType::BSC), 2);
    EXPECT_EQ(static_cast<int>(BlockchainType::Polygon), 3);
    EXPECT_EQ(static_cast<int>(BlockchainType::Avalanche), 4);
    EXPECT_EQ(static_cast<int>(BlockchainType::Solana), 5);
}

// Test color constants
TEST_F(MainStructuresTest, ColorConstants) {
    EXPECT_FALSE(GREEN.empty());
    EXPECT_FALSE(YELLOW.empty());
    EXPECT_FALSE(RED.empty());
    EXPECT_FALSE(BLUE.empty());
    EXPECT_FALSE(RESET.empty());
    EXPECT_FALSE(CYAN.empty());
    
    // Check that colors contain ANSI escape sequences
    EXPECT_TRUE(GREEN.find("\033[32m") != std::string::npos);
    EXPECT_TRUE(YELLOW.find("\033[33m") != std::string::npos);
    EXPECT_TRUE(RED.find("\033[31m") != std::string::npos);
    EXPECT_TRUE(BLUE.find("\033[34m") != std::string::npos);
    EXPECT_TRUE(RESET.find("\033[0m") != std::string::npos);
    EXPECT_TRUE(CYAN.find("\033[36m") != std::string::npos);
}

// Test RpcEndpoint structure
TEST_F(MainStructuresTest, RpcEndpointConstruction) {
    RpcEndpoint endpoint("https://rpc.example.com", 100);
    
    EXPECT_EQ(endpoint.url, "https://rpc.example.com");
    EXPECT_EQ(endpoint.request_limit, 100);
}

TEST_F(MainStructuresTest, RpcEndpointValidation) {
    RpcEndpoint valid_endpoint("https://rpc.example.com", 100);
    EXPECT_TRUE(valid_endpoint.is_valid());
    
    RpcEndpoint invalid_url("", 100);
    EXPECT_FALSE(invalid_url.is_valid());
    
    // Constructor automatically sets default limit for 0 or negative values
    RpcEndpoint zero_limit("https://rpc.example.com", 0);
    EXPECT_TRUE(zero_limit.is_valid());
    EXPECT_EQ(zero_limit.request_limit, 10);
    
    RpcEndpoint negative_limit("https://rpc.example.com", -10);
    EXPECT_TRUE(negative_limit.is_valid());
    EXPECT_EQ(negative_limit.request_limit, 10);
}

TEST_F(MainStructuresTest, RpcEndpointDefaultLimit) {
    RpcEndpoint endpoint("https://rpc.example.com", 0);
    // Should set default limit to 10
    EXPECT_EQ(endpoint.request_limit, 10);
    
    RpcEndpoint endpoint2("https://rpc.example.com", -5);
    // Should set default limit to 10
    EXPECT_EQ(endpoint2.request_limit, 10);
}

// Test PoolInfo structure
TEST_F(MainStructuresTest, PoolInfoConstruction) {
    PoolInfo pool;
    EXPECT_TRUE(pool.address.empty());
    EXPECT_TRUE(pool.token0.empty());
    EXPECT_TRUE(pool.token1.empty());
    EXPECT_EQ(pool.liquidity, 0);
    
    PoolInfo pool2("0xpool", "0xtoken0", "0xtoken1", 1000000);
    EXPECT_EQ(pool2.address, "0xpool");
    EXPECT_EQ(pool2.token0, "0xtoken0");
    EXPECT_EQ(pool2.token1, "0xtoken1");
    EXPECT_EQ(pool2.liquidity, 1000000);
}

TEST_F(MainStructuresTest, PoolInfoValidation) {
    PoolInfo valid_pool("0xpool", "0xtoken0", "0xtoken1", 1000000);
    EXPECT_TRUE(valid_pool.is_valid());
    
    PoolInfo invalid_address("", "0xtoken0", "0xtoken1", 1000000);
    EXPECT_FALSE(invalid_address.is_valid());
    
    PoolInfo invalid_token0("0xpool", "", "0xtoken1", 1000000);
    EXPECT_FALSE(invalid_token0.is_valid());
    
    PoolInfo invalid_token1("0xpool", "0xtoken0", "", 1000000);
    EXPECT_FALSE(invalid_token1.is_valid());
    
    PoolInfo all_empty("", "", "", 0);
    EXPECT_FALSE(all_empty.is_valid());
}

// Test DexInfo structure
TEST_F(MainStructuresTest, DexInfoConstruction) {
    DexInfo dex;
    EXPECT_TRUE(dex.name.empty());
    EXPECT_TRUE(dex.factory_address.empty());
    EXPECT_EQ(dex.pool_count, 0);
    EXPECT_EQ(dex.pools.size(), 0);
    EXPECT_EQ(dex.liquidity, 0);
    EXPECT_EQ(dex.tvl, 0);
    EXPECT_EQ(dex.volume_24h, 0);
    EXPECT_EQ(dex.tx_count_24h, 0);
    
    DexInfo dex2("Uniswap", "0xfactory");
    EXPECT_EQ(dex2.name, "Uniswap");
    EXPECT_EQ(dex2.factory_address, "0xfactory");
    EXPECT_EQ(dex2.pool_count, 0);
    EXPECT_EQ(dex2.pools.size(), 0);
}

TEST_F(MainStructuresTest, DexInfoValidation) {
    DexInfo valid_dex("Uniswap", "0xfactory");
    EXPECT_TRUE(valid_dex.is_valid());
    
    DexInfo invalid_name("", "0xfactory");
    EXPECT_FALSE(invalid_name.is_valid());
    
    DexInfo invalid_factory("Uniswap", "");
    EXPECT_FALSE(invalid_factory.is_valid());
    
    DexInfo all_empty("", "");
    EXPECT_FALSE(all_empty.is_valid());
}

TEST_F(MainStructuresTest, DexInfoAddPool) {
    DexInfo dex("Uniswap", "0xfactory");
    
    PoolInfo valid_pool("0xpool", "0xtoken0", "0xtoken1", 1000000);
    dex.add_pool(valid_pool);
    
    EXPECT_EQ(dex.pools.size(), 1);
    EXPECT_EQ(dex.pool_count, 1);
    EXPECT_EQ(dex.pools[0].address, "0xpool");
    EXPECT_EQ(dex.pools[0].token0, "0xtoken0");
    EXPECT_EQ(dex.pools[0].token1, "0xtoken1");
    EXPECT_EQ(dex.pools[0].liquidity, 1000000);
}

TEST_F(MainStructuresTest, DexInfoAddInvalidPool) {
    DexInfo dex("Uniswap", "0xfactory");
    
    PoolInfo invalid_pool("", "0xtoken0", "0xtoken1", 1000000);
    dex.add_pool(invalid_pool);
    
    // Should not add invalid pool
    EXPECT_EQ(dex.pools.size(), 0);
    EXPECT_EQ(dex.pool_count, 0);
}

TEST_F(MainStructuresTest, DexInfoMultiplePools) {
    DexInfo dex("Uniswap", "0xfactory");
    
    PoolInfo pool1("0xpool1", "0xtoken0", "0xtoken1", 1000000);
    PoolInfo pool2("0xpool2", "0xtoken2", "0xtoken3", 2000000);
    PoolInfo pool3("0xpool3", "0xtoken4", "0xtoken5", 3000000);
    
    dex.add_pool(pool1);
    dex.add_pool(pool2);
    dex.add_pool(pool3);
    
    EXPECT_EQ(dex.pools.size(), 3);
    EXPECT_EQ(dex.pool_count, 3);
    
    EXPECT_EQ(dex.pools[0].address, "0xpool1");
    EXPECT_EQ(dex.pools[1].address, "0xpool2");
    EXPECT_EQ(dex.pools[2].address, "0xpool3");
}

// Test FunctionStats structure
TEST_F(MainStructuresTest, FunctionStatsConstruction) {
    FunctionStats stats;
    EXPECT_EQ(stats.execution_time_ms, 0.0);
    EXPECT_EQ(stats.virtual_memory_kb, 0);
    EXPECT_EQ(stats.disk_usage_bytes, 0);
    EXPECT_EQ(stats.latency_ms, 0.0);
    EXPECT_EQ(stats.cpu_usage_percent, 0.0);
    EXPECT_EQ(stats.outbound_traffic, 0);
    EXPECT_EQ(stats.inbound_traffic, 0);
}

TEST_F(MainStructuresTest, FunctionStatsReset) {
    FunctionStats stats;
    stats.execution_time_ms = 100.0;
    stats.virtual_memory_kb = 1024;
    stats.disk_usage_bytes = 2048;
    stats.latency_ms = 50.0;
    stats.cpu_usage_percent = 25.0;
    stats.outbound_traffic = 1000;
    stats.inbound_traffic = 2000;
    
    stats.reset();
    
    EXPECT_EQ(stats.execution_time_ms, 0.0);
    EXPECT_EQ(stats.virtual_memory_kb, 0);
    EXPECT_EQ(stats.disk_usage_bytes, 0);
    EXPECT_EQ(stats.latency_ms, 0.0);
    EXPECT_EQ(stats.cpu_usage_percent, 0.0);
    EXPECT_EQ(stats.outbound_traffic, 0);
    EXPECT_EQ(stats.inbound_traffic, 0);
}

TEST_F(MainStructuresTest, FunctionStatsAdd) {
    FunctionStats stats1;
    stats1.execution_time_ms = 100.0;
    stats1.virtual_memory_kb = 1024;
    stats1.disk_usage_bytes = 2048;
    stats1.latency_ms = 50.0;
    stats1.cpu_usage_percent = 25.0;
    stats1.outbound_traffic = 1000;
    stats1.inbound_traffic = 2000;
    
    FunctionStats stats2;
    stats2.execution_time_ms = 200.0;
    stats2.virtual_memory_kb = 2048;
    stats2.disk_usage_bytes = 4096;
    stats2.latency_ms = 100.0;
    stats2.cpu_usage_percent = 50.0;
    stats2.outbound_traffic = 2000;
    stats2.inbound_traffic = 4000;
    
    stats1.add(stats2);
    
    EXPECT_EQ(stats1.execution_time_ms, 300.0);
    EXPECT_EQ(stats1.virtual_memory_kb, 3072);
    EXPECT_EQ(stats1.disk_usage_bytes, 6144);
    EXPECT_EQ(stats1.latency_ms, 150.0);
    EXPECT_EQ(stats1.cpu_usage_percent, 75.0);
    EXPECT_EQ(stats1.outbound_traffic, 3000);
    EXPECT_EQ(stats1.inbound_traffic, 6000);
}

// Test time units enum
TEST_F(MainStructuresTest, TimeUnitsEnum) {
    EXPECT_EQ(static_cast<int>(NANOSECONDS), 0);
    EXPECT_EQ(static_cast<int>(MICROSECONDS), 1);
    EXPECT_EQ(static_cast<int>(MILLISECONDS), 2);
    EXPECT_EQ(static_cast<int>(SECONDS), 3);
}

// Test edge cases
TEST_F(MainStructuresTest, EdgeCases) {
    // Test with very long strings
    std::string long_string(1000, 'a');
    RpcEndpoint long_url_endpoint(long_string, 100);
    EXPECT_TRUE(long_url_endpoint.is_valid());
    
    // Test with very large numbers
    PoolInfo large_pool("0xpool", "0xtoken0", "0xtoken1", UINT64_MAX);
    EXPECT_TRUE(large_pool.is_valid());
    
    // Test with special characters in strings
    std::string special_chars = "!@#$%^&*()_+-=[]{}|;':\",./<>?";
    DexInfo special_dex(special_chars, special_chars);
    EXPECT_TRUE(special_dex.is_valid());
}

// Test performance
TEST_F(MainStructuresTest, Performance) {
    const int iterations = 10000;
    
    auto start = std::chrono::high_resolution_clock::now();
    
    for (int i = 0; i < iterations; ++i) {
        RpcEndpoint endpoint("https://rpc.example.com", 100);
        endpoint.is_valid();
        
        PoolInfo pool("0xpool", "0xtoken0", "0xtoken1", 1000000);
        pool.is_valid();
        
        DexInfo dex("Uniswap", "0xfactory");
        dex.is_valid();
        
        FunctionStats stats;
        stats.reset();
    }
    
    auto end = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(end - start);
    
    // Should complete in reasonable time (less than 1 second)
    EXPECT_LT(duration.count(), 1000);
}

// Test memory usage
TEST_F(MainStructuresTest, MemoryUsage) {
    std::vector<RpcEndpoint> endpoints;
    std::vector<PoolInfo> pools;
    std::vector<DexInfo> dexes;
    std::vector<FunctionStats> stats;
    
    const int count = 1000;
    
    for (int i = 0; i < count; ++i) {
        endpoints.emplace_back("https://rpc" + std::to_string(i) + ".example.com", 100);
        pools.emplace_back("0xpool" + std::to_string(i), "0xtoken0", "0xtoken1", 1000000);
        dexes.emplace_back("DEX" + std::to_string(i), "0xfactory" + std::to_string(i));
        stats.emplace_back();
    }
    
    // Verify all objects were created
    EXPECT_EQ(endpoints.size(), count);
    EXPECT_EQ(pools.size(), count);
    EXPECT_EQ(dexes.size(), count);
    EXPECT_EQ(stats.size(), count);
    
    // Verify all objects are valid
    for (const auto& endpoint : endpoints) {
        EXPECT_TRUE(endpoint.is_valid());
    }
    
    for (const auto& pool : pools) {
        EXPECT_TRUE(pool.is_valid());
    }
    
    for (const auto& dex : dexes) {
        EXPECT_TRUE(dex.is_valid());
    }
}

// Test error handling
TEST_F(MainStructuresTest, ErrorHandling) {
    // Test with null pointers (if applicable)
    // This is mainly to ensure the functions don't crash
    
    // Test with invalid data types
    FunctionStats stats;
    stats.execution_time_ms = std::numeric_limits<double>::max();
    stats.virtual_memory_kb = SIZE_MAX;
    stats.disk_usage_bytes = SIZE_MAX;
    
    // Should not crash
    stats.reset();
    stats.add(stats);
}

int main(int argc, char **argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
