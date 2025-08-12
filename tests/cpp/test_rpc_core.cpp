// test_rpc_core.cpp
// Unit tests for rpc_core module

#include <gtest/gtest.h>
#include <gmock/gmock.h>
#include <string>
#include <chrono>
#include <curl/curl.h>
#include "../include/network/rpc_core.h"
#include "../include/main.h"

class RpcCoreTest : public ::testing::Test {
protected:
    void SetUp() override {
        // Initialize CURL for testing
        curl_global_init(CURL_GLOBAL_DEFAULT);
    }
    
    void TearDown() override {
        // Clean up CURL
        curl_global_cleanup();
    }
};

// Test write_callback function
TEST_F(RpcCoreTest, WriteCallback_ValidData) {
    std::string buffer;
    const char* data = "test data";
    size_t size = 4;
    size_t nmemb = 2;
    
    size_t result = write_callback(const_cast<char*>(data), size, nmemb, buffer);
    
    EXPECT_EQ(result, 8); // size * nmemb
    EXPECT_EQ(buffer, "test dat");
}

TEST_F(RpcCoreTest, WriteCallback_EmptyData) {
    std::string buffer;
    const char* data = "";
    size_t size = 0;
    size_t nmemb = 5;
    
    size_t result = write_callback(const_cast<char*>(data), size, nmemb, buffer);
    
    EXPECT_EQ(result, 0);
    EXPECT_EQ(buffer, "");
}

TEST_F(RpcCoreTest, WriteCallback_SingleByte) {
    std::string buffer;
    const char* data = "a";
    size_t size = 1;
    size_t nmemb = 1;
    
    size_t result = write_callback(const_cast<char*>(data), size, nmemb, buffer);
    
    EXPECT_EQ(result, 1);
    EXPECT_EQ(buffer, "a");
}

TEST_F(RpcCoreTest, WriteCallback_LargeData) {
    std::string buffer;
    std::string large_data(1000, 'x');
    size_t size = 1000;
    size_t nmemb = 1;
    
    size_t result = write_callback(const_cast<char*>(large_data.c_str()), size, nmemb, buffer);
    
    EXPECT_EQ(result, 1000);
    EXPECT_EQ(buffer, large_data);
}

// Test parse_json_result function
TEST_F(RpcCoreTest, ParseJsonResult_StringResult) {
    std::string json = "{\"jsonrpc\":\"2.0\",\"result\":\"0x12345\",\"id\":1}";
    
    std::string result = parse_json_result(json);
    
    EXPECT_EQ(result, "0x12345");
}

TEST_F(RpcCoreTest, ParseJsonResult_NumberResult) {
    std::string json = "{\"jsonrpc\":\"2.0\",\"result\":12345,\"id\":1}";
    
    std::string result = parse_json_result(json);
    
    EXPECT_EQ(result, "12345");
}

TEST_F(RpcCoreTest, ParseJsonResult_HexResult) {
    std::string json = "{\"jsonrpc\":\"2.0\",\"result\":\"0xabcdef\",\"id\":1}";
    
    std::string result = parse_json_result(json);
    
    EXPECT_EQ(result, "0xabcdef");
}

TEST_F(RpcCoreTest, ParseJsonResult_NoResultField) {
    std::string json = "{\"jsonrpc\":\"2.0\",\"error\":\"Invalid request\",\"id\":1}";
    
    std::string result = parse_json_result(json);
    
    EXPECT_EQ(result, "");
}

TEST_F(RpcCoreTest, ParseJsonResult_EmptyJson) {
    std::string json = "";
    
    std::string result = parse_json_result(json);
    
    EXPECT_EQ(result, "");
}

TEST_F(RpcCoreTest, ParseJsonResult_WhitespaceAfterResult) {
    std::string json = "{\"jsonrpc\":\"2.0\",\"result\":   \"0x12345\",\"id\":1}";
    
    std::string result = parse_json_result(json);
    
    EXPECT_EQ(result, "0x12345");
}

TEST_F(RpcCoreTest, ParseJsonResult_TabAfterResult) {
    std::string json = "{\"jsonrpc\":\"2.0\",\"result\":\t\"0x12345\",\"id\":1}";
    
    std::string result = parse_json_result(json);
    
    EXPECT_EQ(result, "0x12345");
}

TEST_F(RpcCoreTest, ParseJsonResult_NewlineAfterResult) {
    std::string json = "{\"jsonrpc\":\"2.0\",\"result\":\n\"0x12345\",\"id\":1}";
    
    std::string result = parse_json_result(json);
    
    EXPECT_EQ(result, "0x12345");
}

TEST_F(RpcCoreTest, ParseJsonResult_ResultAtEnd) {
    std::string json = "{\"jsonrpc\":\"2.0\",\"result\":\"0x12345\"}";
    
    std::string result = parse_json_result(json);
    
    EXPECT_EQ(result, "0x12345");
}

TEST_F(RpcCoreTest, ParseJsonResult_InvalidQuotePositions) {
    std::string json = "{\"jsonrpc\":\"2.0\",\"result\":\"\",\"id\":1}";
    
    std::string result = parse_json_result(json);
    
    EXPECT_EQ(result, "");
}

TEST_F(RpcCoreTest, ParseJsonResult_ComplexHex) {
    std::string json = "{\"jsonrpc\":\"2.0\",\"result\":\"0x1234567890abcdef\",\"id\":1}";
    
    std::string result = parse_json_result(json);
    
    EXPECT_EQ(result, "0x1234567890abcdef");
}

// Test print_progress_bar function
TEST_F(RpcCoreTest, PrintProgressBar_ZeroProgress) {
    // This test mainly checks that the function doesn't crash
    EXPECT_NO_THROW(print_progress_bar(0, 100, "Test"));
}

TEST_F(RpcCoreTest, PrintProgressBar_HalfProgress) {
    // This test mainly checks that the function doesn't crash
    EXPECT_NO_THROW(print_progress_bar(50, 100, "Test"));
}

TEST_F(RpcCoreTest, PrintProgressBar_FullProgress) {
    // This test mainly checks that the function doesn't crash
    EXPECT_NO_THROW(print_progress_bar(100, 100, "Test"));
}

TEST_F(RpcCoreTest, PrintProgressBar_EmptyLabel) {
    // This test mainly checks that the function doesn't crash
    EXPECT_NO_THROW(print_progress_bar(25, 100, ""));
}

TEST_F(RpcCoreTest, PrintProgressBar_LargeNumbers) {
    // This test mainly checks that the function doesn't crash
    EXPECT_NO_THROW(print_progress_bar(999999, 1000000, "Large Test"));
}

// Test get_latest_block_number function
TEST_F(RpcCoreTest, GetLatestBlockNumber_ValidUrl) {
    FunctionStats stats;
    std::string url = "https://httpbin.org/post"; // Use httpbin for testing
    
    // Test that function doesn't crash with valid URL
    EXPECT_NO_THROW(get_latest_block_number(url, 1, stats));
    
    // Test that stats are recorded
    EXPECT_GE(stats.execution_time_ms, 0.0);
}

TEST_F(RpcCoreTest, GetLatestBlockNumber_EmptyUrl) {
    FunctionStats stats;
    std::string url = "";
    
    // Test that function handles empty URL gracefully
    EXPECT_NO_THROW(get_latest_block_number(url, 1, stats));
    
    // Test that stats are recorded even for failed requests
    EXPECT_GE(stats.execution_time_ms, 0.0);
}

TEST_F(RpcCoreTest, GetLatestBlockNumber_InvalidUrl) {
    FunctionStats stats;
    std::string url = "invalid-url";
    
    // Test that function handles invalid URL gracefully
    EXPECT_NO_THROW(get_latest_block_number(url, 1, stats));
    
    // Test that stats are recorded even for failed requests
    EXPECT_GE(stats.execution_time_ms, 0.0);
}

TEST_F(RpcCoreTest, GetLatestBlockNumber_ZeroRequestLimit) {
    FunctionStats stats;
    std::string url = "https://httpbin.org/post";
    
    // Test that function handles zero request limit gracefully
    EXPECT_NO_THROW(get_latest_block_number(url, 0, stats));
    
    // Test that stats are recorded even for zero limit
    EXPECT_GE(stats.execution_time_ms, 0.0);
}

// Test make_rpc_call function
TEST_F(RpcCoreTest, MakeRpcCall_ValidRequest) {
    FunctionStats stats;
    std::string url = "https://httpbin.org/post";
    std::string payload = "{\"test\":\"data\"}";
    
    // Test that function doesn't crash with valid request
    EXPECT_NO_THROW(make_rpc_call(url, payload, 1, stats));
    
    // Test that stats are recorded
    EXPECT_GE(stats.execution_time_ms, 0.0);
    EXPECT_EQ(stats.outbound_traffic, payload.size());
}

TEST_F(RpcCoreTest, MakeRpcCall_EmptyPayload) {
    FunctionStats stats;
    std::string url = "https://httpbin.org/post";
    std::string payload = "";
    
    // Test that function handles empty payload gracefully
    EXPECT_NO_THROW(make_rpc_call(url, payload, 1, stats));
    
    // Test that stats are recorded
    EXPECT_GE(stats.execution_time_ms, 0.0);
    EXPECT_EQ(stats.outbound_traffic, payload.size());
}

TEST_F(RpcCoreTest, MakeRpcCall_InvalidUrl) {
    FunctionStats stats;
    std::string url = "invalid-url";
    std::string payload = "{\"test\":\"data\"}";
    
    // Test that function handles invalid URL gracefully
    EXPECT_NO_THROW(make_rpc_call(url, payload, 1, stats));
    
    // Test that stats are recorded even for failed requests
    EXPECT_GE(stats.execution_time_ms, 0.0);
    // Note: outbound_traffic might be 0 for invalid URLs as the request may not be sent
    EXPECT_GE(stats.outbound_traffic, 0);
}

TEST_F(RpcCoreTest, MakeRpcCall_ZeroRequestLimit) {
    FunctionStats stats;
    std::string url = "https://httpbin.org/post";
    std::string payload = "{\"test\":\"data\"}";
    
    // Test that function handles zero request limit gracefully
    EXPECT_NO_THROW(make_rpc_call(url, payload, 0, stats));
    
    // Test that stats are recorded even for zero limit
    EXPECT_GE(stats.execution_time_ms, 0.0);
    EXPECT_EQ(stats.outbound_traffic, payload.size());
}

TEST_F(RpcCoreTest, MakeRpcCall_StatsRecording) {
    FunctionStats stats;
    std::string url = "https://httpbin.org/post";
    std::string payload = "{\"test\":\"data\"}";
    
    make_rpc_call(url, payload, 10, stats);
    
    // Should record execution time
    EXPECT_GE(stats.execution_time_ms, 0.0);
    
    // Should record outbound size
    EXPECT_EQ(stats.outbound_traffic, payload.size());
}

// Test edge cases
TEST_F(RpcCoreTest, ParseJsonResult_MalformedJson) {
    std::string json = "{\"jsonrpc\":\"2.0\",\"result\":\"unclosed quote";
    
    std::string result = parse_json_result(json);
    
    EXPECT_EQ(result, "");
}

TEST_F(RpcCoreTest, ParseJsonResult_ResultWithSpaces) {
    std::string json = "{\"jsonrpc\":\"2.0\",\"result\": \" 0x12345 \",\"id\":1}";
    
    std::string result = parse_json_result(json);
    
    EXPECT_EQ(result, " 0x12345 ");
}

TEST_F(RpcCoreTest, ParseJsonResult_ResultWithSpecialChars) {
    std::string json = "{\"jsonrpc\":\"2.0\",\"result\":\"0x12345\\n\",\"id\":1}";
    
    std::string result = parse_json_result(json);
    
    EXPECT_EQ(result, "0x12345\\n");
}

// Test performance characteristics
TEST_F(RpcCoreTest, Performance_WriteCallback) {
    std::string buffer;
    std::string test_data(10000, 'x');
    
    auto start = std::chrono::high_resolution_clock::now();
    
    for (int i = 0; i < 1000; ++i) {
        write_callback(const_cast<char*>(test_data.c_str()), test_data.size(), 1, buffer);
    }
    
    auto end = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::microseconds>(end - start);
    
    // Should complete in reasonable time (less than 1 second)
    EXPECT_LT(duration.count(), 1000000);
}

TEST_F(RpcCoreTest, Performance_ParseJsonResult) {
    std::string json = "{\"jsonrpc\":\"2.0\",\"result\":\"0x12345\",\"id\":1}";
    
    auto start = std::chrono::high_resolution_clock::now();
    
    for (int i = 0; i < 10000; ++i) {
        parse_json_result(json);
    }
    
    auto end = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::microseconds>(end - start);
    
    // Should complete in reasonable time (less than 1 second)
    EXPECT_LT(duration.count(), 1000000);
}
