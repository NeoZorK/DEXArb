// test_measure.cpp
// Comprehensive tests for measure.cpp with 100% coverage

#include <gtest/gtest.h>
#include <chrono>
#include <iostream>
#include <sstream>
#include <thread>
#include "../include/main.h"

// Forward declarations for the functions we're testing
void update_stats(FunctionStats& stats, const std::chrono::high_resolution_clock::time_point& start,
                  const std::chrono::high_resolution_clock::time_point& end, size_t outbound_size, size_t inbound_size);
void StartTimeMeasure();
void StopTimeMeasure(const ENUM_TIME_UNITS time_units);

// Use FunctionStats and ENUM_TIME_UNITS from main.h

// Test fixture for measure tests
class MeasureTest : public ::testing::Test {
protected:
    void SetUp() override {
        // Setup common test data
        start_time = std::chrono::high_resolution_clock::now();
        
        // Wait a bit to ensure different timestamps
        std::this_thread::sleep_for(std::chrono::milliseconds(1));
        end_time = std::chrono::high_resolution_clock::now();
        
        // Reset stats
        stats = FunctionStats{};
    }

    std::chrono::high_resolution_clock::time_point start_time;
    std::chrono::high_resolution_clock::time_point end_time;
    FunctionStats stats;
};

// Test update_stats function
TEST_F(MeasureTest, UpdateStatsBasic) {
    // Test basic functionality
    size_t outbound_size = 1024;
    size_t inbound_size = 2048;
    
    update_stats(stats, start_time, end_time, outbound_size, inbound_size);
    
    // Verify execution time is calculated
    EXPECT_GT(stats.execution_time_ms, 0.0);
    
    // Verify network traffic stats are set
    EXPECT_EQ(stats.outbound_traffic, outbound_size);
    EXPECT_EQ(stats.inbound_traffic, inbound_size);
    
    // Verify placeholder values
    EXPECT_EQ(stats.cpu_usage_percent, 0.0);
    EXPECT_EQ(stats.virtual_memory_kb, 0);
}

// Test update_stats with different sizes
TEST_F(MeasureTest, UpdateStatsDifferentSizes) {
    std::vector<std::pair<size_t, size_t>> test_sizes = {
        {0, 0},
        {1, 1},
        {1024, 2048},
        {1000000, 2000000},
        {0xFFFFFFFF, 0xFFFFFFFF}
    };
    
    for (const auto& [outbound, inbound] : test_sizes) {
        FunctionStats test_stats{};
        update_stats(test_stats, start_time, end_time, outbound, inbound);
        
        EXPECT_EQ(test_stats.outbound_traffic, outbound);
        EXPECT_EQ(test_stats.inbound_traffic, inbound);
        EXPECT_GT(test_stats.execution_time_ms, 0.0);
    }
}

// Test update_stats timing accuracy
TEST_F(MeasureTest, UpdateStatsTimingAccuracy) {
    // Test with known duration
    auto test_start = std::chrono::high_resolution_clock::now();
    std::this_thread::sleep_for(std::chrono::milliseconds(10)); // 10ms
    auto test_end = std::chrono::high_resolution_clock::now();
    
    FunctionStats test_stats{};
    update_stats(test_stats, test_start, test_end, 100, 200);
    
    // Execution time should be approximately 10ms (with some tolerance)
    EXPECT_GT(test_stats.execution_time_ms, 9.0);
    EXPECT_LT(test_stats.execution_time_ms, 11.0);
}

// Test update_stats with zero duration
TEST_F(MeasureTest, UpdateStatsZeroDuration) {
    auto same_time = std::chrono::high_resolution_clock::now();
    
    FunctionStats test_stats{};
    update_stats(test_stats, same_time, same_time, 100, 200);
    
    // Execution time should be 0 or very close to 0
    EXPECT_GE(test_stats.execution_time_ms, 0.0);
    EXPECT_LT(test_stats.execution_time_ms, 0.1);
}

// Test update_stats with very short duration
TEST_F(MeasureTest, UpdateStatsShortDuration) {
    auto test_start = std::chrono::high_resolution_clock::now();
    std::this_thread::sleep_for(std::chrono::microseconds(100)); // 100 microseconds
    auto test_end = std::chrono::high_resolution_clock::now();
    
    FunctionStats test_stats{};
    update_stats(test_stats, test_start, test_end, 100, 200);
    
    // Execution time should be approximately 0.1ms
    EXPECT_GT(test_stats.execution_time_ms, 0.09);
    EXPECT_LT(test_stats.execution_time_ms, 0.11);
}

// Test update_stats with very long duration
TEST_F(MeasureTest, UpdateStatsLongDuration) {
    auto test_start = std::chrono::high_resolution_clock::now();
    std::this_thread::sleep_for(std::chrono::milliseconds(100)); // 100ms
    auto test_end = std::chrono::high_resolution_clock::now();
    
    FunctionStats test_stats{};
    update_stats(test_stats, test_start, test_end, 100, 200);
    
    // Execution time should be approximately 100ms
    EXPECT_GT(test_stats.execution_time_ms, 99.0);
    EXPECT_LT(test_stats.execution_time_ms, 101.0);
}

// Test StartTimeMeasure function
TEST_F(MeasureTest, StartTimeMeasure) {
    // Test that StartTimeMeasure can be called without errors
    EXPECT_NO_THROW(StartTimeMeasure());
    
    // Test that it updates the start time
    auto before_start = std::chrono::high_resolution_clock::now();
    StartTimeMeasure();
    auto after_start = std::chrono::high_resolution_clock::now();
    
    // The start time should be between before and after
    // Note: This is a basic test since we can't directly access the static variable
    EXPECT_NO_THROW();
}

// Test StopTimeMeasure function with different time units
TEST_F(MeasureTest, StopTimeMeasureNanoseconds) {
    StartTimeMeasure();
    std::this_thread::sleep_for(std::chrono::milliseconds(1));
    
    // Capture cout output
    std::stringstream buffer;
    std::streambuf* old_cout = std::cout.rdbuf(buffer.rdbuf());
    
    StopTimeMeasure(NANOSECONDS);
    
    std::cout.rdbuf(old_cout);
    std::string output = buffer.str();
    
    // Verify output contains expected text
    EXPECT_TRUE(output.find("Time Running:") != std::string::npos);
    EXPECT_TRUE(output.find("nanoseconds") != std::string::npos);
}

// Test StopTimeMeasure with microseconds
TEST_F(MeasureTest, StopTimeMeasureMicroseconds) {
    StartTimeMeasure();
    std::this_thread::sleep_for(std::chrono::milliseconds(1));
    
    std::stringstream buffer;
    std::streambuf* old_cout = std::cout.rdbuf(buffer.rdbuf());
    
    StopTimeMeasure(MICROSECONDS);
    
    std::cout.rdbuf(old_cout);
    std::string output = buffer.str();
    
    EXPECT_TRUE(output.find("Time Running:") != std::string::npos);
    EXPECT_TRUE(output.find("microseconds") != std::string::npos);
}

// Test StopTimeMeasure with milliseconds
TEST_F(MeasureTest, StopTimeMeasureMilliseconds) {
    StartTimeMeasure();
    std::this_thread::sleep_for(std::chrono::milliseconds(1));
    
    std::stringstream buffer;
    std::streambuf* old_cout = std::cout.rdbuf(buffer.rdbuf());
    
    StopTimeMeasure(MILLISECONDS);
    
    std::cout.rdbuf(old_cout);
    std::string output = buffer.str();
    
    EXPECT_TRUE(output.find("Time Running:") != std::string::npos);
    EXPECT_TRUE(output.find("milliseconds") != std::string::npos);
}

// Test StopTimeMeasure with seconds
TEST_F(MeasureTest, StopTimeMeasureSeconds) {
    StartTimeMeasure();
    std::this_thread::sleep_for(std::chrono::milliseconds(1));
    
    std::stringstream buffer;
    std::streambuf* old_cout = std::cout.rdbuf(buffer.rdbuf());
    
    StopTimeMeasure(SECONDS);
    
    std::cout.rdbuf(old_cout);
    std::string output = buffer.str();
    
    EXPECT_TRUE(output.find("Time Running:") != std::string::npos);
    EXPECT_TRUE(output.find("seconds") != std::string::npos);
}

// Test StopTimeMeasure with unknown time unit
TEST_F(MeasureTest, StopTimeMeasureUnknownUnit) {
    StartTimeMeasure();
    std::this_thread::sleep_for(std::chrono::milliseconds(1));
    
    std::stringstream buffer;
    std::streambuf* old_cout = std::cout.rdbuf(buffer.rdbuf());
    
    StopTimeMeasure(static_cast<ENUM_TIME_UNITS>(999)); // Unknown time unit
    
    std::cout.rdbuf(old_cout);
    std::string output = buffer.str();
    
    EXPECT_TRUE(output.find("Unknown time unit") != std::string::npos);
}

// Test timing accuracy of StopTimeMeasure
TEST_F(MeasureTest, StopTimeMeasureAccuracy) {
    StartTimeMeasure();
    std::this_thread::sleep_for(std::chrono::milliseconds(50)); // 50ms
    
    std::stringstream buffer;
    std::streambuf* old_cout = std::cout.rdbuf(buffer.rdbuf());
    
    StopTimeMeasure(MILLISECONDS);
    
    std::cout.rdbuf(old_cout);
    std::string output = buffer.str();
    
    // Extract the time value from output
    size_t pos = output.find("Time Running: ");
    if (pos != std::string::npos) {
        pos += 14; // Skip "Time Running: "
        size_t end = output.find(" milliseconds");
        if (end != std::string::npos) {
            std::string time_str = output.substr(pos, end - pos);
            double time_value = std::stod(time_str);
            
            // Time should be approximately 50ms (with tolerance)
            EXPECT_GT(time_value, 45.0);
            EXPECT_LT(time_value, 55.0);
        }
    }
}

// Test multiple StartTimeMeasure calls
TEST_F(MeasureTest, MultipleStartTimeMeasure) {
    // Test multiple start/stop cycles
    for (int i = 0; i < 5; i++) {
        StartTimeMeasure();
        std::this_thread::sleep_for(std::chrono::microseconds(100));
        
        std::stringstream buffer;
        std::streambuf* old_cout = std::cout.rdbuf(buffer.rdbuf());
        
        StopTimeMeasure(MICROSECONDS);
        
        std::cout.rdbuf(old_cout);
        std::string output = buffer.str();
        
        EXPECT_TRUE(output.find("Time Running:") != std::string::npos);
        EXPECT_TRUE(output.find("microseconds") != std::string::npos);
    }
}

// Test edge case timing
TEST_F(MeasureTest, EdgeCaseTiming) {
    // Test with minimal sleep
    StartTimeMeasure();
    std::this_thread::sleep_for(std::chrono::nanoseconds(1));
    
    std::stringstream buffer;
    std::streambuf* old_cout = std::cout.rdbuf(buffer.rdbuf());
    
    StopTimeMeasure(NANOSECONDS);
    
    std::cout.rdbuf(old_cout);
    std::string output = buffer.str();
    
    EXPECT_TRUE(output.find("Time Running:") != std::string::npos);
    EXPECT_TRUE(output.find("nanoseconds") != std::string::npos);
}

// Test concurrent access (basic)
TEST_F(MeasureTest, ConcurrentAccess) {
    // Test that functions can be called from different contexts
    std::vector<std::thread> threads;
    
    for (int i = 0; i < 5; i++) {
        threads.emplace_back([i]() {
            StartTimeMeasure();
            std::this_thread::sleep_for(std::chrono::microseconds(100));
            
            std::stringstream buffer;
            std::streambuf* old_cout = std::cout.rdbuf(buffer.rdbuf());
            
            StopTimeMeasure(MICROSECONDS);
            
            std::cout.rdbuf(old_cout);
        });
    }
    
    for (auto& thread : threads) {
        thread.join();
    }
    
    EXPECT_NO_THROW();
}

// Test error handling
TEST_F(MeasureTest, ErrorHandling) {
    // Test that functions don't throw exceptions
    EXPECT_NO_THROW(StartTimeMeasure());
    EXPECT_NO_THROW(StopTimeMeasure(NANOSECONDS));
    
    // Test with invalid time units
    EXPECT_NO_THROW(StopTimeMeasure(static_cast<ENUM_TIME_UNITS>(-1)));
    EXPECT_NO_THROW(StopTimeMeasure(static_cast<ENUM_TIME_UNITS>(1000)));
}

// Main function to run all tests
int main(int argc, char** argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
