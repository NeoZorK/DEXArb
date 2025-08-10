//
//  test_modern_format.cpp
//  NeoZorKDEXArb
//
//  Created by Rostyslav S. on 10.08.2025.
//
#include <gtest/gtest.h>
#include <iostream>
#include <string>
#include <vector>
#include <chrono>

// Include our format utilities
#include "utils/modern_format.h"

using namespace modern;

class ModernFormatTest : public ::testing::Test {
protected:
    void SetUp() override {
        // Setup test data if needed
    }
    
    void TearDown() override {
        // Cleanup if needed
    }
};

// Test basic formatting
TEST_F(ModernFormatTest, BasicFormatting) {
    auto result = Format::format("Hello {}!", "World");
    EXPECT_EQ(result, "Hello World!");
    
    auto result2 = Format::format("Number: {}, String: {}", 42, "test");
    EXPECT_EQ(result2, "Number: 42, String: test");
    
    auto result3 = Format::format("Multiple: {} {} {}", 1, 2, 3);
    EXPECT_EQ(result3, "Multiple: 1 2 3");
}

// Test number formatting
TEST_F(ModernFormatTest, NumberFormatting) {
    auto result = Format::format_number(1234567);
    EXPECT_EQ(result, "1,234,567");
    
    auto result2 = Format::format_number(1000);
    EXPECT_EQ(result2, "1,000");
    
    auto result3 = Format::format_number(999);
    EXPECT_EQ(result3, "999");
    
    auto result4 = Format::format_number(0);
    EXPECT_EQ(result4, "0");
}

// Test bytes formatting
TEST_F(ModernFormatTest, BytesFormatting) {
    auto result = Format::format_bytes(1024);
    EXPECT_EQ(result, "1.00 KB");
    
    auto result2 = Format::format_bytes(1048576);
    EXPECT_EQ(result2, "1.00 MB");
    
    auto result3 = Format::format_bytes(500);
    EXPECT_EQ(result3, "500 B");
    
    auto result4 = Format::format_bytes(1073741824);
    EXPECT_EQ(result4, "1.00 GB");
}

// Test percentage formatting
TEST_F(ModernFormatTest, PercentageFormatting) {
    auto result = Format::format_percentage(0.5);
    EXPECT_EQ(result, "50.00%");
    
    auto result2 = Format::format_percentage(0.123, 1);
    EXPECT_EQ(result2, "12.3%");
    
    auto result3 = Format::format_percentage(1.0);
    EXPECT_EQ(result3, "100.00%");
    
    auto result4 = Format::format_percentage(0.0);
    EXPECT_EQ(result4, "0.00%");
}

// Test progress bar
TEST_F(ModernFormatTest, ProgressBar) {
    auto result = Format::format_progress_bar(0.5);
    EXPECT_TRUE(result.find("50.00%") != std::string::npos);
    EXPECT_TRUE(result.find("[") != std::string::npos);
    EXPECT_TRUE(result.find("]") != std::string::npos);
    
    auto result2 = Format::format_progress_bar(0.0);
    EXPECT_TRUE(result2.find("0.00%") != std::string::npos);
    
    auto result3 = Format::format_progress_bar(1.0);
    EXPECT_TRUE(result3.find("100.00%") != std::string::npos);
}

// Test address formatting
TEST_F(ModernFormatTest, AddressFormatting) {
    std::string long_address = "0x1234567890abcdef1234567890abcdef12345678";
    auto result = Format::format_address(long_address);
    EXPECT_EQ(result.length(), 20);
    EXPECT_TRUE(result.find("...") != std::string::npos);
    
    std::string short_address = "0x1234";
    auto result2 = Format::format_address(short_address);
    EXPECT_EQ(result2, short_address);
}

// Test string utilities
TEST_F(ModernFormatTest, StringUtilities) {
    // Test join
    std::vector<std::string> vec = {"a", "b", "c"};
    auto join_result = StringUtils::join(vec, ",");
    EXPECT_EQ(join_result, "a,b,c");
    
    // Test split
    auto split_result = StringUtils::split("a,b,c", ",");
    EXPECT_EQ(split_result.size(), 3);
    EXPECT_EQ(split_result[0], "a");
    EXPECT_EQ(split_result[1], "b");
    EXPECT_EQ(split_result[2], "c");
    
    // Test trim
    auto trim_result = StringUtils::trim("  hello world  ");
    EXPECT_EQ(trim_result, "hello world");
    
    // Test to_lower
    auto lower_result = StringUtils::to_lower("HELLO WORLD");
    EXPECT_EQ(lower_result, "hello world");
    
    // Test to_upper
    auto upper_result = StringUtils::to_upper("hello world");
    EXPECT_EQ(upper_result, "HELLO WORLD");
    
    // Test starts_with
    EXPECT_TRUE(StringUtils::starts_with("hello world", "hello"));
    EXPECT_FALSE(StringUtils::starts_with("hello world", "world"));
    
    // Test ends_with
    EXPECT_TRUE(StringUtils::ends_with("hello world", "world"));
    EXPECT_FALSE(StringUtils::ends_with("hello world", "hello"));
    
    // Test replace_all
    auto replace_result = StringUtils::replace_all("hello world", "o", "0");
    EXPECT_EQ(replace_result, "hell0 w0rld");
}

// Test table formatting
TEST_F(ModernFormatTest, TableFormatting) {
    auto result = Format::format_table_row("Name", "Age", "City");
    EXPECT_TRUE(result.find("|") != std::string::npos);
    EXPECT_TRUE(result.find("Name") != std::string::npos);
    EXPECT_TRUE(result.find("Age") != std::string::npos);
    EXPECT_TRUE(result.find("City") != std::string::npos);
}

// Test duration formatting
TEST_F(ModernFormatTest, DurationFormatting) {
    auto duration = std::chrono::milliseconds(1500);
    auto result = Format::format_duration(duration);
    EXPECT_TRUE(result.find("1s") != std::string::npos);
    EXPECT_TRUE(result.find("500ms") != std::string::npos);
    
    auto hours = std::chrono::hours(2) + std::chrono::minutes(30) + std::chrono::seconds(45);
    auto result2 = Format::format_duration(hours);
    EXPECT_TRUE(result2.find("2h") != std::string::npos);
    EXPECT_TRUE(result2.find("30m") != std::string::npos);
    EXPECT_TRUE(result2.find("45s") != std::string::npos);
}

// Test colored formatting
TEST_F(ModernFormatTest, ColoredFormatting) {
    auto result = Format::format_colored("\033[32m", "Hello {}", "World");
    EXPECT_TRUE(result.find("\033[32m") != std::string::npos);
    EXPECT_TRUE(result.find("\033[0m") != std::string::npos);
    EXPECT_TRUE(result.find("Hello World") != std::string::npos);
}

// Test timestamp formatting
TEST_F(ModernFormatTest, TimestampFormatting) {
    auto now = std::chrono::system_clock::now();
    auto result = Format::format_timestamp(now);
    EXPECT_TRUE(result.find("-") != std::string::npos); // Should contain date
    EXPECT_TRUE(result.find(":") != std::string::npos); // Should contain time
}

// Test edge cases
TEST_F(ModernFormatTest, EdgeCases) {
    // Empty format string
    auto result = Format::format("");
    EXPECT_EQ(result, "");
    
    // No arguments
    auto result2 = Format::format("Hello");
    EXPECT_EQ(result2, "Hello");
    
    // Very large numbers
    auto result3 = Format::format_number(999999999999);
    EXPECT_TRUE(result3.find(",") != std::string::npos);
    
    // Zero bytes
    auto result4 = Format::format_bytes(0);
    EXPECT_EQ(result4, "0 B");
    
    // Empty strings in string utilities
    EXPECT_EQ(StringUtils::trim(""), "");
    EXPECT_EQ(StringUtils::to_lower(""), "");
    EXPECT_EQ(StringUtils::to_upper(""), "");
}

// Test performance
TEST_F(ModernFormatTest, Performance) {
    const int iterations = 1000;
    
    auto start = std::chrono::high_resolution_clock::now();
    for (int i = 0; i < iterations; ++i) {
        Format::format("Number: {}, String: {}", i, "test");
    }
    auto end = std::chrono::high_resolution_clock::now();
    
    auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(end - start);
    EXPECT_LT(duration.count(), 1000); // Should complete in less than 1 second
}

int main(int argc, char **argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
