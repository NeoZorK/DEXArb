//
//  test_modern_format.cpp
//  NeoZorKDEXArb
//
//  Created by Rostyslav S. on 10.08.2025.
//
#include <iostream>
#include <string>
#include <vector>
#include <chrono>

// Simple test framework for now
#define ASSERT_EQ(a, b) if ((a) != (b)) { std::cerr << "ASSERT_EQ failed: " << (a) << " != " << (b) << std::endl; return false; }
#define ASSERT_TRUE(a) if (!(a)) { std::cerr << "ASSERT_TRUE failed" << std::endl; return false; }
#define ASSERT_FALSE(a) if (a) { std::cerr << "ASSERT_FALSE failed" << std::endl; return false; }

// Include our format utilities
#include "utils/modern_format.h"

using namespace modern;

static bool test_basic_formatting() {
    std::cout << "Testing basic formatting..." << std::endl;
    
    auto result = Format::format("Hello {}!", "World");
    ASSERT_EQ(result, "Hello World!");
    
    auto result2 = Format::format("Number: {}, String: {}", 42, "test");
    ASSERT_EQ(result2, "Number: 42, String: test");
    
    auto result3 = Format::format("Multiple: {} {} {}", 1, 2, 3);
    ASSERT_EQ(result3, "Multiple: 1 2 3");
    
    return true;
}

static bool test_number_formatting() {
    std::cout << "Testing number formatting..." << std::endl;
    
    std::cout << "Testing 1234567..." << std::endl;
    auto result = Format::format_number(1234567);
    std::cout << "Result: " << result << std::endl;
    ASSERT_EQ(result, "1,234,567");
    
    std::cout << "Testing 1000..." << std::endl;
    auto result2 = Format::format_number(1000);
    std::cout << "Result: " << result2 << std::endl;
    ASSERT_EQ(result2, "1,000");
    
    std::cout << "Testing 999..." << std::endl;
    auto result3 = Format::format_number(999);
    std::cout << "Result: " << result3 << std::endl;
    ASSERT_EQ(result3, "999");
    
    return true;
}

static bool test_bytes_formatting() {
    std::cout << "Testing bytes formatting..." << std::endl;
    
    auto result = Format::format_bytes(1024);
    ASSERT_EQ(result, "1.00 KB");
    
    auto result2 = Format::format_bytes(1048576);
    ASSERT_EQ(result2, "1.00 MB");
    
    auto result3 = Format::format_bytes(500);
    ASSERT_EQ(result3, "500 B");
    
    return true;
}

static bool test_percentage_formatting() {
    std::cout << "Testing percentage formatting..." << std::endl;
    
    auto result = Format::format_percentage(0.5);
    ASSERT_EQ(result, "50.00%");
    
    auto result2 = Format::format_percentage(0.123, 1);
    ASSERT_EQ(result2, "12.3%");
    
    return true;
}

static bool test_progress_bar() {
    std::cout << "Testing progress bar..." << std::endl;
    
    auto result = Format::format_progress_bar(0.5);
    ASSERT_TRUE(result.find("50.00%") != std::string::npos);
    ASSERT_TRUE(result.find("[") != std::string::npos);
    ASSERT_TRUE(result.find("]") != std::string::npos);
    
    return true;
}

static bool test_address_formatting() {
    std::cout << "Testing address formatting..." << std::endl;
    
    std::string long_address = "0x1234567890abcdef1234567890abcdef12345678";
    auto result = Format::format_address(long_address);
    ASSERT_TRUE(result.length() <= 20);
    ASSERT_TRUE(result.find("...") != std::string::npos);
    
    std::string short_address = "0x123";
    auto result2 = Format::format_address(short_address);
    ASSERT_EQ(result2, short_address);
    
    return true;
}

static bool test_string_utils() {
    std::cout << "Testing string utilities..." << std::endl;
    
    // Test join
    std::vector<std::string> vec = {"a", "b", "c"};
    auto result = StringUtils::join(vec, ",");
    ASSERT_EQ(result, "a,b,c");
    
    // Test split
    auto split_result = StringUtils::split("a,b,c", ",");
    ASSERT_EQ(split_result.size(), 3);
    ASSERT_EQ(split_result[0], "a");
    ASSERT_EQ(split_result[1], "b");
    ASSERT_EQ(split_result[2], "c");
    
    // Test trim
    auto trim_result = StringUtils::trim("  hello world  ");
    ASSERT_EQ(trim_result, "hello world");
    
    // Test to_lower
    auto lower_result = StringUtils::to_lower("HELLO WORLD");
    ASSERT_EQ(lower_result, "hello world");
    
    // Test to_upper
    auto upper_result = StringUtils::to_upper("hello world");
    ASSERT_EQ(upper_result, "HELLO WORLD");
    
    // Test starts_with
    ASSERT_TRUE(StringUtils::starts_with("hello world", "hello"));
    ASSERT_FALSE(StringUtils::starts_with("hello world", "world"));
    
    // Test ends_with
    ASSERT_TRUE(StringUtils::ends_with("hello world", "world"));
    ASSERT_FALSE(StringUtils::ends_with("hello world", "hello"));
    
    return true;
}

static bool test_table_formatting() {
    std::cout << "Testing table formatting..." << std::endl;
    
    auto result = Format::format_table_row("Name", "Age", "City");
    ASSERT_TRUE(result.find("|") != std::string::npos);
    ASSERT_TRUE(result.find("Name") != std::string::npos);
    ASSERT_TRUE(result.find("Age") != std::string::npos);
    ASSERT_TRUE(result.find("City") != std::string::npos);
    
    return true;
}

static bool test_duration_formatting() {
    std::cout << "Testing duration formatting..." << std::endl;
    
    auto duration = std::chrono::milliseconds(1500);
    auto result = Format::format_duration(duration);
    ASSERT_TRUE(result.find("1s") != std::string::npos);
    ASSERT_TRUE(result.find("500ms") != std::string::npos);
    
    return true;
}

int main() {
    std::cout << "Running modern format tests..." << std::endl;
    
    bool all_passed = true;
    
    all_passed &= test_basic_formatting();
    all_passed &= test_number_formatting();
    all_passed &= test_bytes_formatting();
    all_passed &= test_percentage_formatting();
    all_passed &= test_progress_bar();
    all_passed &= test_address_formatting();
    all_passed &= test_string_utils();
    all_passed &= test_table_formatting();
    all_passed &= test_duration_formatting();
    
    if (all_passed) {
        std::cout << "All tests passed!" << std::endl;
        return 0;
    } else {
        std::cout << "Some tests failed!" << std::endl;
        return 1;
    }
}
