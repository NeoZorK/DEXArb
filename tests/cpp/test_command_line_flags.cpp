//
//  test_command_line_flags.cpp
//  NeoZorKDEXArb
//
//  Created by Rostyslav S. on 26.02.2025.
//
#include <iostream>
#include <string>
#include <vector>
#include <cassert>
#include <memory>

// Simple test framework
// #define ASSERT_EQ(a, b) if ((a) != (b)) { std::cerr << "ASSERT_EQ failed: " << (a) << " != " << (b) << " at line " << __LINE__ << std::endl; return false; }
#define ASSERT_TRUE(a) if (!(a)) { std::cerr << "ASSERT_TRUE failed at line " << __LINE__ << std::endl; return false; }
#define ASSERT_FALSE(a) if (a) { std::cerr << "ASSERT_FALSE failed at line " << __LINE__ << std::endl; return false; }
#define ASSERT_STREQ(a, b) if ((a) != (b)) { std::cerr << "ASSERT_STREQ failed: " << (a) << " != " << (b) << " at line " << __LINE__ << std::endl; return false; }

// Include our main structures and enums
#include "../../include/main.h"
#include "../../include/blockchain.h"

// Mock command line argument parser for testing
class MockCommandLineParser {
public:
    struct ParsedArgs {
        std::string flag;
        std::string blockchain;
        std::string value;
        std::string dex_name;
        std::string token_address;
        bool is_valid;
        std::string error_message;
    };
    
    static ParsedArgs parse_args(int argc, const std::vector<std::string>& args) {
        ParsedArgs result;
        result.is_valid = false;
        
        if (argc == 1) {
            // No arguments - show help
            result.flag = "-h";
            result.is_valid = true;
            return result;
        }
        
        if (argc == 2) {
            std::string flag = args[1];
            if (flag == "-h" || flag == "-help" || flag == "-v" || flag == "-version") {
                result.flag = flag;
                result.is_valid = true;
                return result;
            } else {
                result.error_message = "Insufficient arguments for flag: " + flag;
                return result;
            }
        }
        
        if (argc >= 3) {
            result.flag = args[1];
            result.blockchain = args[2];
            
            if (argc == 3) {
                // Commands that only need blockchain
                if (result.flag == "-showSCAN-CONFIG" || result.flag == "-showSCAN-STAT" || 
                    result.flag == "-showSCAN" || result.flag == "-showDEXES" || result.flag == "-showTOKENS") {
                    result.is_valid = true;
                    return result;
                } else {
                    result.error_message = "Invalid flag or insufficient arguments: " + result.flag;
                    return result;
                }
            }
            
            if (argc == 4) {
                result.value = args[3];
                
                if (result.flag == "-scan") {
                    // Validate scan range
                    try {
                        int scan_range = std::stoi(result.value);
                        if (scan_range >= 1000 && scan_range <= 1000000) {
                            result.is_valid = true;
                            return result;
                        } else {
                            result.error_message = "Invalid scan range: " + std::to_string(scan_range) + " (must be 1000-1000000)";
                            return result;
                        }
                    } catch (const std::exception&) {
                        result.error_message = "Invalid scan range format: " + result.value;
                        return result;
                    }
                } else if (result.flag == "-showPOOLS" || result.flag == "-showTOKENS" || result.flag == "-findTOKENS") {
                    result.dex_name = result.value;
                    result.is_valid = true;
                    return result;
                } else {
                    result.error_message = "Invalid flag or insufficient arguments: " + result.flag;
                    return result;
                }
            }
            
            if (argc == 5 && result.flag == "-findTOKEN") {
                result.dex_name = args[3];
                result.token_address = args[4];
                result.is_valid = true;
                return result;
            }
        }
        
        result.error_message = "Invalid usage with " + std::to_string(argc) + " arguments";
        return result;
    }
    
    static bool is_valid_blockchain(const std::string& blockchain) {
        std::vector<std::string> valid_blockchains = {
            "ethereum", "Ethereum", "bsc", "BSC", "polygon", "Polygon",
            "fantom", "Fantom", "avalanche", "Avalanche", "solana", "Solana"
        };
        
        return std::find(valid_blockchains.begin(), valid_blockchains.end(), blockchain) != valid_blockchains.end();
    }
    
    static bool is_valid_scan_range(const std::string& range) {
        try {
            int value = std::stoi(range);
            return value >= 1000 && value <= 1000000;
        } catch (const std::exception&) {
            return false;
        }
    }
    
    static bool is_valid_address(const std::string& address) {
        // Basic Ethereum address validation (0x + 40 hex chars)
        if (address.length() != 42 || address.substr(0, 2) != "0x") {
            return false;
        }
        
        for (size_t i = 2; i < address.length(); i++) {
            if (!isxdigit(address[i])) {
                return false;
            }
        }
        
        return true;
    }
};

// Test help flag
static bool test_help_flag() {
    std::cout << "Testing help flag..." << std::endl;
    
    // Test -h flag
    auto result1 = MockCommandLineParser::parse_args(2, {"neozork", "-h"});
    ASSERT_TRUE(result1.is_valid);
    ASSERT_STREQ(result1.flag, "-h");
    
    // Test -help flag
    auto result2 = MockCommandLineParser::parse_args(2, {"neozork", "-help"});
    ASSERT_TRUE(result2.is_valid);
    ASSERT_STREQ(result2.flag, "-help");
    
    // Test help without arguments
    auto result3 = MockCommandLineParser::parse_args(1, {"neozork"});
    ASSERT_TRUE(result3.is_valid);
    ASSERT_STREQ(result3.flag, "-h");
    
    return true;
}

// Test version flag
static bool test_version_flag() {
    std::cout << "Testing version flag..." << std::endl;
    
    // Test -v flag
    auto result1 = MockCommandLineParser::parse_args(2, {"neozork", "-v"});
    ASSERT_TRUE(result1.is_valid);
    ASSERT_STREQ(result1.flag, "-v");
    
    // Test -version flag
    auto result2 = MockCommandLineParser::parse_args(2, {"neozork", "-version"});
    ASSERT_TRUE(result2.is_valid);
    ASSERT_STREQ(result2.flag, "-version");
    
    return true;
}

// Test scan flag
static bool test_scan_flag() {
    std::cout << "Testing scan flag..." << std::endl;
    
    // Test valid scan command
    auto result1 = MockCommandLineParser::parse_args(4, {"neozork", "-scan", "ethereum", "5000"});
    ASSERT_TRUE(result1.is_valid);
    ASSERT_STREQ(result1.flag, "-scan");
    ASSERT_STREQ(result1.blockchain, "ethereum");
    ASSERT_STREQ(result1.value, "5000");
    
    // Test scan with different blockchain
    auto result2 = MockCommandLineParser::parse_args(4, {"neozork", "-scan", "bsc", "10000"});
    ASSERT_TRUE(result2.is_valid);
    ASSERT_STREQ(result2.blockchain, "bsc");
    ASSERT_STREQ(result2.value, "10000");
    
    // Test invalid scan range (too low)
    auto result3 = MockCommandLineParser::parse_args(4, {"neozork", "-scan", "ethereum", "500"});
    ASSERT_FALSE(result3.is_valid);
    ASSERT_TRUE(result3.error_message.find("Invalid scan range") != std::string::npos);
    
    // Test invalid scan range (too high)
    auto result4 = MockCommandLineParser::parse_args(4, {"neozork", "-scan", "ethereum", "2000000"});
    ASSERT_FALSE(result4.is_valid);
    ASSERT_TRUE(result4.error_message.find("Invalid scan range") != std::string::npos);
    
    // Test invalid scan range format
    auto result5 = MockCommandLineParser::parse_args(4, {"neozork", "-scan", "ethereum", "invalid"});
    ASSERT_FALSE(result5.is_valid);
    ASSERT_TRUE(result5.error_message.find("Invalid scan range format") != std::string::npos);
    
    return true;
}

// Test showDEXES flag
static bool test_show_dexes_flag() {
    std::cout << "Testing showDEXES flag..." << std::endl;
    
    // Test valid showDEXES command
    auto result1 = MockCommandLineParser::parse_args(3, {"neozork", "-showDEXES", "ethereum"});
    ASSERT_TRUE(result1.is_valid);
    ASSERT_STREQ(result1.flag, "-showDEXES");
    ASSERT_STREQ(result1.blockchain, "ethereum");
    
    // Test with different blockchain
    auto result2 = MockCommandLineParser::parse_args(3, {"neozork", "-showDEXES", "bsc"});
    ASSERT_TRUE(result2.is_valid);
    ASSERT_STREQ(result2.blockchain, "bsc");
    
    return true;
}

// Test showPOOLS flag
static bool test_show_pools_flag() {
    std::cout << "Testing showPOOLS flag..." << std::endl;
    
    // Test valid showPOOLS command
    auto result1 = MockCommandLineParser::parse_args(4, {"neozork", "-showPOOLS", "ethereum", "Uniswap"});
    ASSERT_TRUE(result1.is_valid);
    ASSERT_STREQ(result1.flag, "-showPOOLS");
    ASSERT_STREQ(result1.blockchain, "ethereum");
    ASSERT_STREQ(result1.dex_name, "Uniswap");
    
    // Test with different parameters
    auto result2 = MockCommandLineParser::parse_args(4, {"neozork", "-showPOOLS", "bsc", "PancakeSwap"});
    ASSERT_TRUE(result2.is_valid);
    ASSERT_STREQ(result2.blockchain, "bsc");
    ASSERT_STREQ(result2.dex_name, "PancakeSwap");
    
    return true;
}

// Test showTOKENS flag
static bool test_show_tokens_flag() {
    std::cout << "Testing showTOKENS flag..." << std::endl;
    
    // Test showTOKENS with blockchain only
    auto result1 = MockCommandLineParser::parse_args(3, {"neozork", "-showTOKENS", "ethereum"});
    ASSERT_TRUE(result1.is_valid);
    ASSERT_STREQ(result1.flag, "-showTOKENS");
    ASSERT_STREQ(result1.blockchain, "ethereum");
    
    // Test showTOKENS with DEX parameter
    auto result2 = MockCommandLineParser::parse_args(4, {"neozork", "-showTOKENS", "ethereum", "Uniswap"});
    ASSERT_TRUE(result2.is_valid);
    ASSERT_STREQ(result2.flag, "-showTOKENS");
    ASSERT_STREQ(result2.blockchain, "ethereum");
    ASSERT_STREQ(result2.dex_name, "Uniswap");
    
    return true;
}

// Test showSCAN flags
static bool test_show_scan_flags() {
    std::cout << "Testing showSCAN flags..." << std::endl;
    
    // Test showSCAN-CONFIG
    auto result1 = MockCommandLineParser::parse_args(3, {"neozork", "-showSCAN-CONFIG", "ethereum"});
    ASSERT_TRUE(result1.is_valid);
    ASSERT_STREQ(result1.flag, "-showSCAN-CONFIG");
    ASSERT_STREQ(result1.blockchain, "ethereum");
    
    // Test showSCAN
    auto result2 = MockCommandLineParser::parse_args(3, {"neozork", "-showSCAN", "ethereum"});
    ASSERT_TRUE(result2.is_valid);
    ASSERT_STREQ(result2.flag, "-showSCAN");
    ASSERT_STREQ(result2.blockchain, "ethereum");
    
    // Test showSCAN-STAT
    auto result3 = MockCommandLineParser::parse_args(3, {"neozork", "-showSCAN-STAT", "ethereum"});
    ASSERT_TRUE(result3.is_valid);
    ASSERT_STREQ(result3.flag, "-showSCAN-STAT");
    ASSERT_STREQ(result3.blockchain, "ethereum");
    
    return true;
}

// Test findTOKEN flag
static bool test_find_token_flag() {
    std::cout << "Testing findTOKEN flag..." << std::endl;
    
    // Test valid findTOKEN command
    auto result1 = MockCommandLineParser::parse_args(5, {"neozork", "-findTOKEN", "ethereum", "Uniswap", "0x1234567890abcdef"});
    ASSERT_TRUE(result1.is_valid);
    ASSERT_STREQ(result1.flag, "-findTOKEN");
    ASSERT_STREQ(result1.blockchain, "ethereum");
    ASSERT_STREQ(result1.dex_name, "Uniswap");
    ASSERT_STREQ(result1.token_address, "0x1234567890abcdef");
    
    // Test with different parameters
    auto result2 = MockCommandLineParser::parse_args(5, {"neozork", "-findTOKEN", "bsc", "PancakeSwap", "0xfedcba0987654321"});
    ASSERT_TRUE(result2.is_valid);
    ASSERT_STREQ(result2.blockchain, "bsc");
    ASSERT_STREQ(result2.dex_name, "PancakeSwap");
    ASSERT_STREQ(result2.token_address, "0xfedcba0987654321");
    
    return true;
}

// Test findTOKENS flag
static bool test_find_tokens_flag() {
    std::cout << "Testing findTOKENS flag..." << std::endl;
    
    // Test valid findTOKENS command
    auto result1 = MockCommandLineParser::parse_args(4, {"neozork", "-findTOKENS", "ethereum", "USDC"});
    ASSERT_TRUE(result1.is_valid);
    ASSERT_STREQ(result1.flag, "-findTOKENS");
    ASSERT_STREQ(result1.blockchain, "ethereum");
    ASSERT_STREQ(result1.dex_name, "USDC");
    
    return true;
}

// Test blockchain validation
static bool test_blockchain_validation() {
    std::cout << "Testing blockchain validation..." << std::endl;
    
    // Test valid blockchains
    ASSERT_TRUE(MockCommandLineParser::is_valid_blockchain("ethereum"));
    ASSERT_TRUE(MockCommandLineParser::is_valid_blockchain("Ethereum"));
    ASSERT_TRUE(MockCommandLineParser::is_valid_blockchain("bsc"));
    ASSERT_TRUE(MockCommandLineParser::is_valid_blockchain("BSC"));
    ASSERT_TRUE(MockCommandLineParser::is_valid_blockchain("polygon"));
    ASSERT_TRUE(MockCommandLineParser::is_valid_blockchain("Polygon"));
    ASSERT_TRUE(MockCommandLineParser::is_valid_blockchain("fantom"));
    ASSERT_TRUE(MockCommandLineParser::is_valid_blockchain("Fantom"));
    ASSERT_TRUE(MockCommandLineParser::is_valid_blockchain("avalanche"));
    ASSERT_TRUE(MockCommandLineParser::is_valid_blockchain("Avalanche"));
    ASSERT_TRUE(MockCommandLineParser::is_valid_blockchain("solana"));
    ASSERT_TRUE(MockCommandLineParser::is_valid_blockchain("Solana"));
    
    // Test invalid blockchains
    ASSERT_FALSE(MockCommandLineParser::is_valid_blockchain("invalid"));
    ASSERT_FALSE(MockCommandLineParser::is_valid_blockchain(""));
    ASSERT_FALSE(MockCommandLineParser::is_valid_blockchain("bitcoin"));
    
    return true;
}

// Test scan range validation
static bool test_scan_range_validation() {
    std::cout << "Testing scan range validation..." << std::endl;
    
    // Test valid ranges
    ASSERT_TRUE(MockCommandLineParser::is_valid_scan_range("1000"));
    ASSERT_TRUE(MockCommandLineParser::is_valid_scan_range("5000"));
    ASSERT_TRUE(MockCommandLineParser::is_valid_scan_range("10000"));
    ASSERT_TRUE(MockCommandLineParser::is_valid_scan_range("100000"));
    ASSERT_TRUE(MockCommandLineParser::is_valid_scan_range("1000000"));
    
    // Test invalid ranges
    ASSERT_FALSE(MockCommandLineParser::is_valid_scan_range("999"));
    ASSERT_FALSE(MockCommandLineParser::is_valid_scan_range("1000001"));
    ASSERT_FALSE(MockCommandLineParser::is_valid_scan_range("0"));
    ASSERT_FALSE(MockCommandLineParser::is_valid_scan_range("-1000"));
    ASSERT_FALSE(MockCommandLineParser::is_valid_scan_range("invalid"));
    ASSERT_FALSE(MockCommandLineParser::is_valid_scan_range(""));
    
    return true;
}

// Test address validation
static bool test_address_validation() {
    std::cout << "Testing address validation..." << std::endl;
    
    // Test valid addresses
    ASSERT_TRUE(MockCommandLineParser::is_valid_address("0x1234567890abcdef1234567890abcdef12345678"));
    ASSERT_TRUE(MockCommandLineParser::is_valid_address("0xABCDEFabcdef1234567890ABCDEFabcdef123456"));
    ASSERT_TRUE(MockCommandLineParser::is_valid_address("0x0000000000000000000000000000000000000000"));
    
    // Test invalid addresses
    ASSERT_FALSE(MockCommandLineParser::is_valid_address("0x123")); // Too short
    ASSERT_FALSE(MockCommandLineParser::is_valid_address("0x1234567890abcdef1234567890abcdef123456789")); // Too long
    ASSERT_FALSE(MockCommandLineParser::is_valid_address("1234567890abcdef1234567890abcdef12345678")); // No 0x prefix
    ASSERT_FALSE(MockCommandLineParser::is_valid_address("0x1234567890abcdef1234567890abcdef1234567g")); // Invalid char
    ASSERT_FALSE(MockCommandLineParser::is_valid_address(""));
    ASSERT_FALSE(MockCommandLineParser::is_valid_address("invalid"));
    
    return true;
}

// Test error handling
static bool test_error_handling() {
    std::cout << "Testing error handling..." << std::endl;
    
    // Test insufficient arguments for scan
    auto result1 = MockCommandLineParser::parse_args(3, {"neozork", "-scan", "ethereum"});
    ASSERT_FALSE(result1.is_valid);
    ASSERT_TRUE(result1.error_message.find("Invalid flag or insufficient arguments") != std::string::npos);
    
    // Test invalid flag
    auto result2 = MockCommandLineParser::parse_args(3, {"neozork", "-invalid", "ethereum"});
    ASSERT_FALSE(result2.is_valid);
    ASSERT_TRUE(result2.error_message.find("Invalid flag or insufficient arguments") != std::string::npos);
    
    // Test too many arguments
    auto result3 = MockCommandLineParser::parse_args(6, {"neozork", "-findTOKEN", "ethereum", "Uniswap", "0x123", "extra"});
    ASSERT_FALSE(result3.is_valid);
    ASSERT_TRUE(result3.error_message.find("Invalid usage") != std::string::npos);
    
    return true;
}

// Main test runner
int main() {
    std::cout << "Running command line flag tests..." << std::endl;
    std::cout << "===================================" << std::endl;
    
    std::vector<std::pair<std::string, std::function<bool()>>> tests = {
        {"Help Flag", test_help_flag},
        {"Version Flag", test_version_flag},
        {"Scan Flag", test_scan_flag},
        {"Show DEXES Flag", test_show_dexes_flag},
        {"Show POOLS Flag", test_show_pools_flag},
        {"Show TOKENS Flag", test_show_tokens_flag},
        {"Show SCAN Flags", test_show_scan_flags},
        {"Find TOKEN Flag", test_find_token_flag},
        {"Find TOKENS Flag", test_find_tokens_flag},
        {"Blockchain Validation", test_blockchain_validation},
        {"Scan Range Validation", test_scan_range_validation},
        {"Address Validation", test_address_validation},
        {"Error Handling", test_error_handling}
    };
    
    int passed = 0;
    size_t total = tests.size();
    
    for (const auto& [test_name, test_func] : tests) {
        std::cout << "\nRunning test: " << test_name << std::endl;
        try {
            if (test_func()) {
                std::cout << "✓ PASS: " << test_name << std::endl;
                passed++;
            } else {
                std::cout << "✗ FAIL: " << test_name << std::endl;
            }
        } catch (const std::exception& e) {
            std::cout << "✗ FAIL: " << test_name << " (Exception: " << e.what() << ")" << std::endl;
        } catch (...) {
            std::cout << "✗ FAIL: " << test_name << " (Unknown exception)" << std::endl;
        }
    }
    
    std::cout << "\n===================================" << std::endl;
    std::cout << "Test Summary: " << passed << "/" << total << " tests passed" << std::endl;
    
    if (static_cast<size_t>(passed) == total) {
        std::cout << "✓ All tests passed successfully!" << std::endl;
        return 0;
    } else {
        std::cout << "✗ Some tests failed!" << std::endl;
        return 1;
    }
}
