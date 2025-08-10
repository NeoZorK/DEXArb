#include <gtest/gtest.h>
#include <iostream>
#include <string>
#include <vector>
#include <memory>

// Include the modern_utils header
#include "utils/modern_utils.h"

using namespace modern_utils;

class ModernUtilsTest : public ::testing::Test {
protected:
    void SetUp() override {
        // Setup test data
    }
    
    void TearDown() override {
        // Cleanup if needed
    }
};

// Test Logger class
TEST_F(ModernUtilsTest, LoggerBasicFunctionality) {
    // Test that logger methods don't crash
    Logger::debug("Debug message");
    Logger::info("Info message");
    Logger::warning("Warning message");
    Logger::error("Error message");
    
    // Test log method with different levels
    Logger::log(Logger::Level::DEBUG, "Debug via log method");
    Logger::log(Logger::Level::INFO, "Info via log method");
    Logger::log(Logger::Level::WARNING, "Warning via log method");
    Logger::log(Logger::Level::ERROR, "Error via log method");
}

// Test Logger level enum
TEST_F(ModernUtilsTest, LoggerLevelEnum) {
    EXPECT_EQ(static_cast<int>(Logger::Level::DEBUG), 0);
    EXPECT_EQ(static_cast<int>(Logger::Level::INFO), 1);
    EXPECT_EQ(static_cast<int>(Logger::Level::WARNING), 2);
    EXPECT_EQ(static_cast<int>(Logger::Level::ERROR), 3);
}

// Test JsonUtils extract_value method
TEST_F(ModernUtilsTest, JsonUtilsExtractValue) {
    std::string json = R"({"name": "test", "value": 123, "active": true})";
    
    // Test string extraction
    EXPECT_EQ(JsonUtils::extract_value(json, "name"), "test");
    
    // Test numeric extraction
    EXPECT_EQ(JsonUtils::extract_value(json, "value"), "123");
    
    // Test boolean extraction
    EXPECT_EQ(JsonUtils::extract_value(json, "active"), "true");
    
    // Test non-existent key
    EXPECT_EQ(JsonUtils::extract_value(json, "nonexistent"), "");
    
    // Test empty JSON
    EXPECT_EQ(JsonUtils::extract_value("", "name"), "");
    
    // Test JSON with whitespace
    std::string json_with_whitespace = R"({  "name"  :  "test"  })";
    EXPECT_EQ(JsonUtils::extract_value(json_with_whitespace, "name"), "test");
}

// Test JsonUtils has_key method
TEST_F(ModernUtilsTest, JsonUtilsHasKey) {
    std::string json = R"({"name": "test", "value": 123})";
    
    EXPECT_TRUE(JsonUtils::has_key(json, "name"));
    EXPECT_TRUE(JsonUtils::has_key(json, "value"));
    EXPECT_FALSE(JsonUtils::has_key(json, "nonexistent"));
    EXPECT_FALSE(JsonUtils::has_key("", "name"));
}

// Test JsonUtils is_valid_json method
TEST_F(ModernUtilsTest, JsonUtilsIsValidJson) {
    // Valid JSON
    EXPECT_TRUE(JsonUtils::is_valid_json(R"({"name": "test"})"));
    EXPECT_TRUE(JsonUtils::is_valid_json(R"({"name": "test", "value": 123})"));
    EXPECT_TRUE(JsonUtils::is_valid_json(R"({"nested": {"key": "value"}})"));
    
    // Invalid JSON
    EXPECT_FALSE(JsonUtils::is_valid_json(""));
    EXPECT_FALSE(JsonUtils::is_valid_json("invalid json"));
    EXPECT_FALSE(JsonUtils::is_valid_json("{unclosed"));
    EXPECT_FALSE(JsonUtils::is_valid_json("unclosed}"));
}

// Test JsonUtils with complex JSON
TEST_F(ModernUtilsTest, JsonUtilsComplexJson) {
    std::string complex_json = R"({
        "user": {
            "id": 12345,
            "name": "John Doe",
            "email": "john@example.com",
            "active": true,
            "roles": ["admin", "user"]
        },
        "settings": {
            "theme": "dark",
            "notifications": false
        }
    })";
    
    // Test nested extraction
    EXPECT_EQ(JsonUtils::extract_value(complex_json, "user"), "");
    EXPECT_EQ(JsonUtils::extract_value(complex_json, "id"), "");
    EXPECT_EQ(JsonUtils::extract_value(complex_json, "name"), "");
    
    // Test has_key with complex JSON
    EXPECT_TRUE(JsonUtils::has_key(complex_json, "user"));
    EXPECT_TRUE(JsonUtils::has_key(complex_json, "settings"));
    EXPECT_FALSE(JsonUtils::has_key(complex_json, "nonexistent"));
}

// Test JsonUtils with edge cases
TEST_F(ModernUtilsTest, JsonUtilsEdgeCases) {
    // JSON with special characters
    std::string special_json = R"({"key": "value with \"quotes\" and \\backslashes"})";
    EXPECT_TRUE(JsonUtils::is_valid_json(special_json));
    
    // JSON with numbers
    std::string number_json = R"({"int": 123, "float": 123.456, "hex": 0xFF})";
    EXPECT_TRUE(JsonUtils::is_valid_json(number_json));
    
    // JSON with boolean values
    std::string bool_json = R"({"true": true, "false": false})";
    EXPECT_TRUE(JsonUtils::is_valid_json(bool_json));
    
    // JSON with null (simplified)
    std::string null_json = R"({"null": null})";
    EXPECT_TRUE(JsonUtils::is_valid_json(null_json));
}

// Test JsonUtils with malformed JSON
TEST_F(ModernUtilsTest, JsonUtilsMalformedJson) {
    // Missing quotes
    EXPECT_FALSE(JsonUtils::is_valid_json(R"({name: "test"})"));
    
    // Missing colon
    EXPECT_FALSE(JsonUtils::is_valid_json(R"({"name" "test"})"));
    
    // Missing comma
    EXPECT_FALSE(JsonUtils::is_valid_json(R"({"name": "test" "value": 123})"));
    
    // Unclosed brackets
    EXPECT_FALSE(JsonUtils::is_valid_json(R"({"name": "test")"));
    EXPECT_FALSE(JsonUtils::is_valid_json(R"({"name": "test"}"));
}

// Test JsonUtils with empty values
TEST_F(ModernUtilsTest, JsonUtilsEmptyValues) {
    std::string empty_json = R"({"empty_string": "", "zero": 0, "null": null})";
    
    EXPECT_EQ(JsonUtils::extract_value(empty_json, "empty_string"), "");
    EXPECT_EQ(JsonUtils::extract_value(empty_json, "zero"), "0");
    EXPECT_EQ(JsonUtils::extract_value(empty_json, "null"), "");
}

// Test JsonUtils with very long JSON
TEST_F(ModernUtilsTest, JsonUtilsLongJson) {
    std::string long_key = std::string(1000, 'a');
    std::string long_value = std::string(1000, 'b');
    std::string long_json = "{\"" + long_key + "\": \"" + long_value + "\"}";
    
    EXPECT_TRUE(JsonUtils::is_valid_json(long_json));
    EXPECT_EQ(JsonUtils::extract_value(long_json, long_key), long_value);
    EXPECT_TRUE(JsonUtils::has_key(long_json, long_key));
}

// Test JsonUtils with unicode characters
TEST_F(ModernUtilsTest, JsonUtilsUnicode) {
    std::string unicode_json = R"({"name": "José", "city": "São Paulo", "emoji": "🚀"})";
    
    EXPECT_TRUE(JsonUtils::is_valid_json(unicode_json));
    EXPECT_EQ(JsonUtils::extract_value(unicode_json, "name"), "José");
    EXPECT_EQ(JsonUtils::extract_value(unicode_json, "city"), "São Paulo");
    EXPECT_EQ(JsonUtils::extract_value(unicode_json, "emoji"), "🚀");
}

// Test JsonUtils with nested objects
TEST_F(ModernUtilsTest, JsonUtilsNestedObjects) {
    std::string nested_json = R"({
        "level1": {
            "level2": {
                "level3": "deep_value"
            }
        }
    })";
    
    EXPECT_TRUE(JsonUtils::is_valid_json(nested_json));
    EXPECT_TRUE(JsonUtils::has_key(nested_json, "level1"));
    // Note: extract_value won't work for nested objects with current implementation
}

// Test JsonUtils with arrays (simplified)
TEST_F(ModernUtilsTest, JsonUtilsArrays) {
    std::string array_json = R"({"items": ["item1", "item2", "item3"]})";
    
    EXPECT_TRUE(JsonUtils::is_valid_json(array_json));
    EXPECT_TRUE(JsonUtils::has_key(array_json, "items"));
    // Note: extract_value won't work for arrays with current implementation
}

// Test performance
TEST_F(ModernUtilsTest, JsonUtilsPerformance) {
    std::string json = R"({"key1": "value1", "key2": "value2", "key3": "value3"})";
    const int iterations = 1000;
    
    auto start = std::chrono::high_resolution_clock::now();
    
    for (int i = 0; i < iterations; ++i) {
        JsonUtils::extract_value(json, "key1");
        JsonUtils::has_key(json, "key2");
        JsonUtils::is_valid_json(json);
    }
    
    auto end = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(end - start);
    
    // Should complete in reasonable time (less than 1 second)
    EXPECT_LT(duration.count(), 1000);
}

// Test error handling
TEST_F(ModernUtilsTest, JsonUtilsErrorHandling) {
    // Test with very large strings
    std::string large_json = std::string(10000, '{') + std::string(10000, '}');
    EXPECT_FALSE(JsonUtils::is_valid_json(large_json));
    
    // Test with null pointers (if applicable)
    // This is mainly to ensure the functions don't crash
}

int main(int argc, char **argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
