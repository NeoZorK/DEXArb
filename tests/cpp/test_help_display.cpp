//
//  test_help_display.cpp
//  NeoZorKDEXArb
//
//  Created by Rostyslav S. on 10.08.2025.
//
#include <iostream>
#include <sstream>
#include <string>
#include <vector>
#include <cassert>

// Simple test framework
#define ASSERT_EQ(a, b) if ((a) != (b)) { std::cerr << "ASSERT_EQ failed: " << (a) << " != " << (b) << " at line " << __LINE__ << std::endl; return false; }
#define ASSERT_TRUE(a) if (!(a)) { std::cerr << "ASSERT_TRUE failed at line " << __LINE__ << std::endl; return false; }
#define ASSERT_FALSE(a) if ((a)) { std::cerr << "ASSERT_FALSE failed at line " << __LINE__ << std::endl; return false; }
#define ASSERT_STREQ(a, b) if ((a) != (b)) { std::cerr << "ASSERT_STREQ failed: " << (a) << " != " << (b) << " at line " << __LINE__ << std::endl; return false; }

#include "../../include/cli/help_display.h"

bool test_help_display_basic() {
    std::cout << "Testing HelpDisplay basic functionality..." << std::endl;
    
    // Test that we can create a HelpDisplay instance (though it's mostly static)
    // The class is designed to be used statically, so we test the static methods
    
    std::ostringstream output;
    
    // Test show_help method
    try {
        cli::HelpDisplay::show_help(output);
        std::string help_output = output.str();
        ASSERT_TRUE(!help_output.empty());
        std::cout << "✓ show_help works" << std::endl;
    } catch (const std::exception& e) {
        std::cout << "✗ show_help failed: " << e.what() << std::endl;
        return false;
    }
    
    // Test show_version method
    output.str(""); // Clear the stream
    try {
        cli::HelpDisplay::show_version(output);
        std::string version_output = output.str();
        ASSERT_TRUE(!version_output.empty());
        std::cout << "✓ show_version works" << std::endl;
    } catch (const std::exception& e) {
        std::cout << "✗ show_version failed: " << e.what() << std::endl;
        return false;
    }
    
    return true;
}

bool test_help_display_methods() {
    std::cout << "Testing HelpDisplay methods..." << std::endl;
    
    std::ostringstream output;
    
    // Test show_usage_examples
    try {
        cli::HelpDisplay::show_usage_examples(output);
        std::string usage_output = output.str();
        ASSERT_TRUE(!usage_output.empty());
        std::cout << "✓ show_usage_examples works" << std::endl;
    } catch (const std::exception& e) {
        std::cout << "✗ show_usage_examples failed: " << e.what() << std::endl;
        return false;
    }
    
    // Test show_supported_blockchains
    output.str("");
    try {
        cli::HelpDisplay::show_supported_blockchains(output);
        std::string blockchains_output = output.str();
        ASSERT_TRUE(!blockchains_output.empty());
        std::cout << "✓ show_supported_blockchains works" << std::endl;
    } catch (const std::exception& e) {
        std::cout << "✗ show_supported_blockchains failed: " << e.what() << std::endl;
        return false;
    }
    
    return true;
}

bool test_help_display_consistency() {
    std::cout << "Testing HelpDisplay consistency..." << std::endl;
    
    std::ostringstream output1, output2;
    
    // Test that multiple calls to the same method produce consistent output
    cli::HelpDisplay::show_help(output1);
    cli::HelpDisplay::show_help(output2);
    
    std::string help1 = output1.str();
    std::string help2 = output2.str();
    
    ASSERT_EQ(help1, help2);
    std::cout << "✓ Help output is consistent" << std::endl;
    
    // Test that different methods produce different output
    output1.str("");
    output2.str("");
    
    cli::HelpDisplay::show_help(output1);
    cli::HelpDisplay::show_version(output2);
    
    std::string general_help = output1.str();
    std::string version_help = output2.str();
    
    ASSERT_TRUE(general_help != version_help);
    std::cout << "✓ Different methods produce different output" << std::endl;
    
    return true;
}

bool test_help_display_error_handling() {
    std::cout << "Testing HelpDisplay error handling..." << std::endl;
    
    std::ostringstream output;
    
    // Test show_error with empty message
    try {
        cli::HelpDisplay::show_error("", output);
        std::string error_output = output.str();
        ASSERT_TRUE(!error_output.empty());
        std::cout << "✓ show_error handles empty message" << std::endl;
    } catch (const std::exception& e) {
        std::cout << "✗ show_error failed: " << e.what() << std::endl;
        return false;
    }
    
    // Test show_error with normal message
    output.str("");
    try {
        cli::HelpDisplay::show_error("Test error message", output);
        std::string error_output = output.str();
        ASSERT_TRUE(!error_output.empty());
        std::cout << "✓ show_error works with normal message" << std::endl;
    } catch (const std::exception& e) {
        std::cout << "✗ show_error failed: " << e.what() << std::endl;
        return false;
    }
    
    return true;
}

bool test_help_display_utility_methods() {
    std::cout << "Testing HelpDisplay utility methods..." << std::endl;
    
    std::ostringstream output;
    
    // Test show_separator
    try {
        cli::HelpDisplay::show_separator(output);
        std::string separator_output = output.str();
        ASSERT_TRUE(!separator_output.empty());
        std::cout << "✓ show_separator works" << std::endl;
    } catch (const std::exception& e) {
        std::cout << "✗ show_separator failed: " << e.what() << std::endl;
        return false;
    }
    
    // Test show_banner
    output.str("");
    try {
        cli::HelpDisplay::show_banner(output);
        std::string banner_output = output.str();
        ASSERT_TRUE(!banner_output.empty());
        std::cout << "✓ show_banner works" << std::endl;
    } catch (const std::exception& e) {
        std::cout << "✗ show_banner failed: " << e.what() << std::endl;
        return false;
    }
    
    return true;
}

bool test_help_display_table_methods() {
    std::cout << "Testing HelpDisplay table methods..." << std::endl;
    
    std::ostringstream output;
    
    // Test show_table_header
    std::vector<std::string> headers = {"Name", "Type", "Description"};
    try {
        cli::HelpDisplay::show_table_header(headers, output);
        std::string header_output = output.str();
        ASSERT_TRUE(!header_output.empty());
        std::cout << "✓ show_table_header works" << std::endl;
    } catch (const std::exception& e) {
        std::cout << "✗ show_table_header failed: " << e.what() << std::endl;
        return false;
    }
    
    // Test show_table_row
    output.str("");
    std::vector<std::string> row = {"Test", "String", "A test value"};
    try {
        cli::HelpDisplay::show_table_row(row, output);
        std::string row_output = output.str();
        ASSERT_TRUE(!row_output.empty());
        std::cout << "✓ show_table_row works" << std::endl;
    } catch (const std::exception& e) {
        std::cout << "✗ show_table_row failed: " << e.what() << std::endl;
        return false;
    }
    
    return true;
}

bool test_help_display_progress_bar() {
    std::cout << "Testing HelpDisplay progress bar..." << std::endl;
    
    std::ostringstream output;
    
    // Test show_progress_bar
    try {
        cli::HelpDisplay::show_progress_bar(50, 100, "Test Progress", output);
        std::string progress_output = output.str();
        ASSERT_TRUE(!progress_output.empty());
        std::cout << "✓ show_progress_bar works" << std::endl;
    } catch (const std::exception& e) {
        std::cout << "✗ show_progress_bar failed: " << e.what() << std::endl;
        return false;
    }
    
    // Test progress bar with edge cases
    output.str("");
    try {
        cli::HelpDisplay::show_progress_bar(0, 100, "", output);
        std::string progress_output = output.str();
        ASSERT_TRUE(!progress_output.empty());
        std::cout << "✓ show_progress_bar handles edge cases" << std::endl;
    } catch (const std::exception& e) {
        std::cout << "✗ show_progress_bar edge case failed: " << e.what() << std::endl;
        return false;
    }
    
    return true;
}

int main() {
    std::cout << "=== HelpDisplay Tests ===" << std::endl;
    
    int total = 0;
    int passed = 0;
    
    // Run all tests
    total++;
    if (test_help_display_basic()) passed++;
    
    total++;
    if (test_help_display_methods()) passed++;
    
    total++;
    if (test_help_display_consistency()) passed++;
    
    total++;
    if (test_help_display_error_handling()) passed++;
    
    total++;
    if (test_help_display_utility_methods()) passed++;
    
    total++;
    if (test_help_display_table_methods()) passed++;
    
    total++;
    if (test_help_display_progress_bar()) passed++;
    
    std::cout << "\n=== Test Results ===" << std::endl;
    std::cout << "Passed: " << passed << "/" << total << std::endl;
    
    if (passed == total) {
        std::cout << "✓ All tests passed!" << std::endl;
        return 0;
    } else {
        std::cout << "✗ Some tests failed!" << std::endl;
        return 1;
    }
}
