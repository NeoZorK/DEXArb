//
//  test_profit_analyzer.cpp
//  NeoZorKDEXArb
//
//  Created by AI Assistant on 18.03.2025.
//
#include <gtest/gtest.h>
#include "core/profit_analyzer.h"
#include "main.h"
#include <vector>
#include <iostream>
#include <sstream>

// Test fixture for ProfitAnalyzer tests
class ProfitAnalyzerTest : public ::testing::Test {
protected:
    void SetUp() override {
        // Initialize test data
        test_dex_list = {
            {"TestDEX1", "0x1234567890123456789012345678901234567890"},
            {"TestDEX2", "0x2345678901234567890123456789012345678901"},
            {"TestDEX3", "0x3456789012345678901234567890123456789012"}
        };
        
        // Set some test data for pools
        test_dex_list[0].liquidity = 1000;
        test_dex_list[0].volume_24h = 500;
        test_dex_list[1].liquidity = 800;
        test_dex_list[1].volume_24h = 300;
        test_dex_list[2].liquidity = 1200;
        test_dex_list[2].volume_24h = 700;
        
        test_stats = FunctionStats{};
        test_stats.execution_time_ms = 0.0;
        test_stats.virtual_memory_kb = 0;
        test_stats.disk_usage_bytes = 0;
        test_stats.latency_ms = 0.0;
        test_stats.cpu_usage_percent = 0.0;
        test_stats.outbound_traffic = 0;
        test_stats.inbound_traffic = 0;
    }

    void TearDown() override {
        // Clean up test data
        test_dex_list.clear();
    }

    std::vector<DexInfo> test_dex_list;
    FunctionStats test_stats;
};

// Test for measure_tx_speed_and_profit function
TEST_F(ProfitAnalyzerTest, MeasureTxSpeedAndProfit) {
    // Capture stdout to verify output
    std::stringstream buffer;
    std::streambuf* old = std::cout.rdbuf(buffer.rdbuf());
    
    // Call the function
    measure_tx_speed_and_profit(test_dex_list, test_stats);
    
    // Restore stdout
    std::cout.rdbuf(old);
    
    // Verify the output contains expected message
    std::string output = buffer.str();
    EXPECT_TRUE(output.find("Measuring transaction speed and profit (not implemented yet)") != std::string::npos);
    EXPECT_TRUE(output.find("YELLOW") != std::string::npos || output.find("\033[33m") != std::string::npos);
    EXPECT_TRUE(output.find("RESET") != std::string::npos || output.find("\033[0m") != std::string::npos);
}

// Test for maximize_profit_zero_loss function
TEST_F(ProfitAnalyzerTest, MaximizeProfitZeroLoss) {
    // Capture stdout to verify output
    std::stringstream buffer;
    std::streambuf* old = std::cout.rdbuf(buffer.rdbuf());
    
    uint64_t max_profit = 999; // Set initial value to verify it gets reset
    
    // Call the function
    maximize_profit_zero_loss(test_dex_list, max_profit, test_stats);
    
    // Restore stdout
    std::cout.rdbuf(old);
    
    // Verify the output contains expected message
    std::string output = buffer.str();
    EXPECT_TRUE(output.find("Maximizing profit with zero loss guarantee (not implemented yet)") != std::string::npos);
    EXPECT_TRUE(output.find("YELLOW") != std::string::npos || output.find("\033[33m") != std::string::npos);
    EXPECT_TRUE(output.find("RESET") != std::string::npos || output.find("\033[0m") != std::string::npos);
    
    // Verify max_profit is set to 0 (zero loss guarantee)
    EXPECT_EQ(max_profit, 0);
}

// Test with empty dex list
TEST_F(ProfitAnalyzerTest, EmptyDexList) {
    std::vector<DexInfo> empty_dex_list;
    
    // Capture stdout
    std::stringstream buffer;
    std::streambuf* old = std::cout.rdbuf(buffer.rdbuf());
    
    // Test measure_tx_speed_and_profit with empty list
    measure_tx_speed_and_profit(empty_dex_list, test_stats);
    
    // Restore stdout
    std::cout.rdbuf(old);
    
    // Should still work without crashing
    std::string output = buffer.str();
    EXPECT_TRUE(output.find("Measuring transaction speed and profit (not implemented yet)") != std::string::npos);
}

// Test with large dex list
TEST_F(ProfitAnalyzerTest, LargeDexList) {
    // Create a larger list for stress testing
    std::vector<DexInfo> large_dex_list;
    for (int i = 0; i < 1000; ++i) {
        DexInfo dex("LargeDEX" + std::to_string(i), 
                   "0x" + std::string(38, '0') + std::to_string(i));
        dex.liquidity = static_cast<uint64_t>(i * 100);
        dex.volume_24h = static_cast<uint64_t>(i * 50);
        large_dex_list.push_back(dex);
    }
    
    // Capture stdout
    std::stringstream buffer;
    std::streambuf* old = std::cout.rdbuf(buffer.rdbuf());
    
    uint64_t max_profit = 999;
    
    // Test both functions with large list
    measure_tx_speed_and_profit(large_dex_list, test_stats);
    maximize_profit_zero_loss(large_dex_list, max_profit, test_stats);
    
    // Restore stdout
    std::cout.rdbuf(old);
    
    // Should handle large lists without issues
    EXPECT_EQ(max_profit, 0);
    std::string output = buffer.str();
    EXPECT_TRUE(output.find("Measuring transaction speed and profit (not implemented yet)") != std::string::npos);
    EXPECT_TRUE(output.find("Maximizing profit with zero loss guarantee (not implemented yet)") != std::string::npos);
}

// Test function parameter handling
TEST_F(ProfitAnalyzerTest, ParameterHandling) {
    // Test with const parameters (should not modify input)
    std::vector<DexInfo> original_dex_list = test_dex_list;
    FunctionStats original_stats = test_stats;
    
    uint64_t max_profit = 999;
    
    // Capture stdout
    std::stringstream buffer;
    std::streambuf* old = std::cout.rdbuf(buffer.rdbuf());
    
    // Call functions
    measure_tx_speed_and_profit(test_dex_list, test_stats);
    maximize_profit_zero_loss(test_dex_list, max_profit, test_stats);
    
    // Restore stdout
    std::cout.rdbuf(old);
    
    // Verify input parameters are not modified (except max_profit which is intentionally modified)
    EXPECT_EQ(test_dex_list.size(), original_dex_list.size());
    for (size_t i = 0; i < test_dex_list.size(); ++i) {
        EXPECT_EQ(test_dex_list[i].name, original_dex_list[i].name);
        EXPECT_EQ(test_dex_list[i].factory_address, original_dex_list[i].factory_address);
        EXPECT_EQ(test_dex_list[i].liquidity, original_dex_list[i].liquidity);
        EXPECT_EQ(test_dex_list[i].volume_24h, original_dex_list[i].volume_24h);
    }
    
    // Stats should not be modified by these functions
    EXPECT_EQ(test_stats.execution_time_ms, original_stats.execution_time_ms);
    EXPECT_EQ(test_stats.virtual_memory_kb, original_stats.virtual_memory_kb);
    EXPECT_EQ(test_stats.disk_usage_bytes, original_stats.disk_usage_bytes);
    EXPECT_EQ(test_stats.latency_ms, original_stats.latency_ms);
    EXPECT_EQ(test_stats.cpu_usage_percent, original_stats.cpu_usage_percent);
    EXPECT_EQ(test_stats.outbound_traffic, original_stats.outbound_traffic);
    EXPECT_EQ(test_stats.inbound_traffic, original_stats.inbound_traffic);
}

// Main function for running tests
int main(int argc, char** argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
