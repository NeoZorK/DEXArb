#include <iostream>
#include <sstream>
#include <string>
#include <cassert>

// Simple test framework
#define ASSERT_EQ(a, b) if ((a) != (b)) { std::cerr << "ASSERT_EQ failed: " << (a) << " != " << (b) << " at line " << __LINE__ << std::endl; return false; }
#define ASSERT_TRUE(a) if (!(a)) { std::cerr << "ASSERT_TRUE failed at line " << __LINE__ << std::endl; return false; }
#define ASSERT_FALSE(a) if ((a)) { std::cerr << "ASSERT_FALSE failed at line " << __LINE__ << std::endl; return false; }

#include "../../include/cli/help_display.h"

bool test_help_display_basic() {
    std::cout << "Testing HelpDisplay basic functionality..." << std::endl;
    
    std::ostringstream output;
    
    // Test show_help
    try {
        cli::HelpDisplay::show_help(output);
        std::string help_output = output.str();
        ASSERT_TRUE(!help_output.empty());
        std::cout << "✓ show_help works" << std::endl;
    } catch (const std::exception& e) {
        std::cout << "✗ show_help failed: " << e.what() << std::endl;
        return false;
    }
    
    // Test show_version
    output.str("");
    try {
        cli::HelpDisplay::show_version(output);
        std::string version_output = output.str();
        ASSERT_TRUE(!version_output.empty());
        std::cout << "✓ show_version works" << std::endl;
    } catch (const std::exception& e) {
        std::cout << "✗ show_version failed: " << e.what() << std::endl;
        return false;
    }
    
    // Test show_examples
    output.str("");
    try {
        cli::HelpDisplay::show_examples(output);
        std::string examples_output = output.str();
        ASSERT_TRUE(!examples_output.empty());
        std::cout << "✓ show_examples works" << std::endl;
    } catch (const std::exception& e) {
        std::cout << "✗ show_examples failed: " << e.what() << std::endl;
        return false;
    }
    
    // Test show_error
    output.str("");
    try {
        cli::HelpDisplay::show_error("Test error message", output);
        std::string error_output = output.str();
        ASSERT_TRUE(!error_output.empty());
        std::cout << "✓ show_error works" << std::endl;
    } catch (const std::exception& e) {
        std::cout << "✗ show_error failed: " << e.what() << std::endl;
        return false;
    }
    
    // Test show_info
    output.str("");
    try {
        cli::HelpDisplay::show_info("Test info message", output);
        std::string info_output = output.str();
        ASSERT_TRUE(!info_output.empty());
        std::cout << "✓ show_info works" << std::endl;
    } catch (const std::exception& e) {
        std::cout << "✗ show_info failed: " << e.what() << std::endl;
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

bool test_help_display_default_output() {
    std::cout << "Testing HelpDisplay default output..." << std::endl;
    
    // Test with default output (should not crash)
    try {
        cli::HelpDisplay::show_help();
        cli::HelpDisplay::show_version();
        cli::HelpDisplay::show_examples();
        cli::HelpDisplay::show_error("Test error");
        cli::HelpDisplay::show_info("Test info");
        std::cout << "✓ Default output methods work" << std::endl;
    } catch (const std::exception& e) {
        std::cout << "✗ Default output failed: " << e.what() << std::endl;
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
    if (test_help_display_consistency()) passed++;
    
    total++;
    if (test_help_display_default_output()) passed++;
    
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
