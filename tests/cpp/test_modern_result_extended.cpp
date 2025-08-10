//
//  test_modern_result_extended.cpp
//  NeoZorKDEXArb
//
//  Created by Rostyslav S. on 10.08.2025.
//
#include <iostream>
#include <string>
#include <vector>
#include <memory>

// Simple test framework
#define ASSERT_EQ(a, b) if ((a) != (b)) { std::cerr << "ASSERT_EQ failed: " << (a) << " != " << (b) << " at line " << __LINE__ << std::endl; return false; }
#define ASSERT_TRUE(a) if (!(a)) { std::cerr << "ASSERT_TRUE failed at line " << __LINE__ << std::endl; return false; }
#define ASSERT_FALSE(a) if (a) { std::cerr << "ASSERT_FALSE failed at line " << __LINE__ << std::endl; return false; }
#define ASSERT_STREQ(a, b) if ((a) != (b)) { std::cerr << "ASSERT_STREQ failed: " << (a) << " != " << (b) << " at line " << __LINE__ << std::endl; return false; }

// Include our result utilities
#include "../../include/modern_result.h"

using namespace modern;

// Test data structures
struct TestData {
    int value;
    std::string name;
    
    TestData(int v, const std::string& n) : value(v), name(n) {}
    
    bool operator==(const TestData& other) const {
        return value == other.value && name == other.name;
    }
    
    bool operator!=(const TestData& other) const {
        return !(*this == other);
    }
};

static bool test_result_construction() {
    std::cout << "Testing Result construction..." << std::endl;
    
    // Test default constructor
    Result<int, std::string> default_result;
    ASSERT_FALSE(default_result.is_ok());
    ASSERT_TRUE(default_result.is_err());
    
    // Test value constructor
    Result<int, std::string> ok_result(42);
    ASSERT_TRUE(ok_result.is_ok());
    ASSERT_FALSE(ok_result.is_err());
    ASSERT_EQ(ok_result.unwrap(), 42);
    
    // Test error constructor
    Result<int, std::string> err_result("error message");
    ASSERT_FALSE(err_result.is_ok());
    ASSERT_TRUE(err_result.is_err());
    ASSERT_STREQ(err_result.unwrap_err(), "error message");
    
    // Test move constructor
    Result<int, std::string> move_result(std::move(ok_result));
    ASSERT_TRUE(move_result.is_ok());
    ASSERT_EQ(move_result.unwrap(), 42);
    
    return true;
}

static bool test_result_factory_methods() {
    std::cout << "Testing Result factory methods..." << std::endl;
    
    // Test ok() factory method
    auto ok_result = Result<int, std::string>::ok(123);
    ASSERT_TRUE(ok_result.is_ok());
    ASSERT_EQ(ok_result.unwrap(), 123);
    
    // Test err() factory method
    auto err_result = Result<int, std::string>::err("factory error");
    ASSERT_TRUE(err_result.is_err());
    ASSERT_STREQ(err_result.unwrap_err(), "factory error");
    
    // Test with different types
    auto string_result = Result<std::string, int>::ok("success");
    ASSERT_TRUE(string_result.is_ok());
    ASSERT_STREQ(string_result.unwrap(), "success");
    
    auto int_error_result = Result<std::string, int>::err(404);
    ASSERT_TRUE(int_error_result.is_err());
    ASSERT_EQ(int_error_result.unwrap_err(), 404);
    
    return true;
}

static bool test_result_operators() {
    std::cout << "Testing Result operators..." << std::endl;
    
    // Test bool operator
    Result<int, std::string> ok_result(42);
    Result<int, std::string> err_result("error");
    
    ASSERT_TRUE(static_cast<bool>(ok_result));
    ASSERT_FALSE(static_cast<bool>(err_result));
    
    // Test that different results are distinct
    Result<int, std::string> result1(42);
    Result<int, std::string> result2(100);
    
    ASSERT_TRUE(result1.is_ok() && result2.is_ok());
    ASSERT_EQ(result1.unwrap(), 42);
    ASSERT_EQ(result2.unwrap(), 100);
    
    return true;
}

static bool test_result_unwrap_methods() {
    std::cout << "Testing Result unwrap methods..." << std::endl;
    
    // Test unwrap() for ok result
    Result<int, std::string> ok_result(42);
    ASSERT_EQ(ok_result.unwrap(), 42);
    
    // Test unwrap() for error result (should throw)
    Result<int, std::string> err_result("error");
    try {
        err_result.unwrap();
        ASSERT_FALSE(true); // Should not reach here
    } catch (const std::runtime_error&) {
        ASSERT_TRUE(true); // Should catch exception
    }
    
    // Test unwrap_err() for error result
    ASSERT_STREQ(err_result.unwrap_err(), "error");
    
    // Test unwrap_err() for ok result (should throw)
    try {
        ok_result.unwrap_err();
        ASSERT_FALSE(true); // Should not reach here
    } catch (const std::runtime_error&) {
        ASSERT_TRUE(true); // Should catch exception
    }
    
    return true;
}

static bool test_result_unwrap_or() {
    std::cout << "Testing Result unwrap_or methods..." << std::endl;
    
    // Test unwrap_or() for ok result
    Result<int, std::string> ok_result(42);
    ASSERT_EQ(ok_result.unwrap_or(100), 42);
    
    // Test unwrap_or() for error result
    Result<int, std::string> err_result("error");
    ASSERT_EQ(err_result.unwrap_or(100), 100);
    
    // Test unwrap_or() with move semantics
    Result<std::string, int> string_result("hello");
    ASSERT_STREQ(string_result.unwrap_or("default"), "hello");
    
    Result<std::string, int> string_err_result(404);
    ASSERT_STREQ(string_err_result.unwrap_or("default"), "default");
    
    return true;
}

static bool test_result_copy_semantics() {
    std::cout << "Testing Result copy semantics..." << std::endl;
    
    // Test copy constructor
    Result<int, std::string> original(42);
    Result<int, std::string> copy(original);
    
    ASSERT_TRUE(copy.is_ok());
    ASSERT_EQ(copy.unwrap(), 42);
    ASSERT_TRUE(original.is_ok()); // Original should remain unchanged
    
    // Test copy assignment
    Result<int, std::string> assigned;
    assigned = original;
    
    ASSERT_TRUE(assigned.is_ok());
    ASSERT_EQ(assigned.unwrap(), 42);
    
    // Test copy assignment with error
    Result<int, std::string> error_original("error");
    assigned = error_original;
    
    ASSERT_TRUE(assigned.is_err());
    ASSERT_STREQ(assigned.unwrap_err(), "error");
    
    return true;
}

static bool test_result_move_semantics() {
    std::cout << "Testing Result move semantics..." << std::endl;
    
    // Test move constructor
    Result<std::string, int> original("hello");
    Result<std::string, int> moved(std::move(original));
    
    ASSERT_TRUE(moved.is_ok());
    ASSERT_STREQ(moved.unwrap(), "hello");
    
    // Test move assignment
    Result<std::string, int> assigned;
    assigned = std::move(moved);
    
    ASSERT_TRUE(assigned.is_ok());
    ASSERT_STREQ(assigned.unwrap(), "hello");
    
    return true;
}

static bool test_result_with_custom_types() {
    std::cout << "Testing Result with custom types..." << std::endl;
    
    // Test with custom struct
    TestData data1(42, "test");
    Result<TestData, std::string> custom_result(data1);
    
    ASSERT_TRUE(custom_result.is_ok());
    ASSERT_TRUE(custom_result.unwrap() == data1);
    
    // Test with custom error type
    Result<int, TestData> custom_error_result(TestData(404, "not found"));
    
    ASSERT_TRUE(custom_error_result.is_err());
    ASSERT_TRUE(custom_error_result.unwrap_err() == TestData(404, "not found"));
    
    return true;
}

static bool test_result_with_pointers() {
    std::cout << "Testing Result with pointers..." << std::endl;
    
    // Test with unique_ptr
    auto ptr = std::make_unique<int>(42);
    Result<std::unique_ptr<int>, std::string> ptr_result(std::move(ptr));
    
    ASSERT_TRUE(ptr_result.is_ok());
    ASSERT_EQ(*ptr_result.unwrap(), 42);
    
    // Test with shared_ptr
    auto shared_ptr = std::make_shared<std::string>("hello");
    Result<std::shared_ptr<std::string>, int> shared_result(shared_ptr);
    
    ASSERT_TRUE(shared_result.is_ok());
    ASSERT_EQ(*shared_result.unwrap(), "hello");
    
    return true;
}

static bool test_result_error_handling() {
    std::cout << "Testing Result error handling..." << std::endl;
    
    // Test error propagation
    auto create_error_result = []() -> Result<int, std::string> {
        return Result<int, std::string>::err("operation failed");
    };
    
    auto result = create_error_result();
    ASSERT_TRUE(result.is_err());
    ASSERT_STREQ(result.unwrap_err(), "operation failed");
    
    // Test chaining results
    auto chain_result = [](int value) -> Result<int, std::string> {
        if (value < 0) {
            return Result<int, std::string>::err("negative value");
        }
        return Result<int, std::string>::ok(value * 2);
    };
    
    auto positive_result = chain_result(5);
    ASSERT_TRUE(positive_result.is_ok());
    ASSERT_EQ(positive_result.unwrap(), 10);
    
    auto negative_result = chain_result(-5);
    ASSERT_TRUE(negative_result.is_err());
    ASSERT_STREQ(negative_result.unwrap_err(), "negative value");
    
    return true;
}

static bool test_result_edge_cases() {
    std::cout << "Testing Result edge cases..." << std::endl;
    
    // Test with empty string
    Result<std::string, int> empty_string_result("");
    ASSERT_TRUE(empty_string_result.is_ok());
    ASSERT_TRUE(empty_string_result.unwrap().empty());
    
    // Test with zero value
    Result<int, std::string> zero_result(0);
    ASSERT_TRUE(zero_result.is_ok());
    ASSERT_EQ(zero_result.unwrap(), 0);
    
    // Test with maximum values
    Result<uint64_t, std::string> max_result(UINT64_MAX);
    ASSERT_TRUE(max_result.is_ok());
    ASSERT_EQ(max_result.unwrap(), UINT64_MAX);
    
    // Test with minimum values
    Result<int64_t, std::string> min_result(INT64_MIN);
    ASSERT_TRUE(min_result.is_ok());
    ASSERT_EQ(min_result.unwrap(), INT64_MIN);
    
    return true;
}

int main() {
    std::cout << "Running Modern Result Extended Tests" << std::endl;
    std::cout << "===================================" << std::endl;
    
    int passed = 0;
    int total = 0;
    
    auto run_test = [&](const std::string& name, bool (*test_func)()) {
        total++;
        std::cout << "\nRunning test: " << name << std::endl;
        if (test_func()) {
            std::cout << "✓ PASS: " << name << std::endl;
            passed++;
        } else {
            std::cout << "✗ FAIL: " << name << std::endl;
        }
    };
    
    run_test("Result Construction", test_result_construction);
    run_test("Result Factory Methods", test_result_factory_methods);
    run_test("Result Operators", test_result_operators);
    run_test("Result Unwrap Methods", test_result_unwrap_methods);
    run_test("Result Unwrap Or", test_result_unwrap_or);
    run_test("Result Copy Semantics", test_result_copy_semantics);
    run_test("Result Move Semantics", test_result_move_semantics);
    run_test("Result with Custom Types", test_result_with_custom_types);
    run_test("Result with Pointers", test_result_with_pointers);
    run_test("Result Error Handling", test_result_error_handling);
    run_test("Result Edge Cases", test_result_edge_cases);
    
    std::cout << "\n" << std::string(50, '=') << std::endl;
    std::cout << "Test Summary:" << std::endl;
    std::cout << "Passed: " << passed << "/" << total << std::endl;
    
    if (passed == total) {
        std::cout << "All tests passed! ✓" << std::endl;
        return 0;
    } else {
        std::cout << "Some tests failed! ✗" << std::endl;
        return 1;
    }
}
