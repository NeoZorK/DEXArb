//
//  test_modern_result.cpp
//  NeoZorKDEXArb
//
//  Created by Rostyslav S. on 10.08.2025.
//
#include <gtest/gtest.h>
#include "modern_result.h"
#include <string>
#include <vector>

using namespace modern;

class ModernResultTest : public ::testing::Test {
protected:
    void SetUp() override {}
    void TearDown() override {}
};

// Test basic Result construction
TEST_F(ModernResultTest, BasicConstruction) {
    // Test OK result
    auto ok_result = Result<int>::ok(42);
    EXPECT_TRUE(ok_result.is_ok());
    EXPECT_FALSE(ok_result.is_err());
    EXPECT_EQ(ok_result.unwrap(), 42);
    
    // Test error result
    auto err_result = Result<int>::err("test error");
    EXPECT_FALSE(err_result.is_ok());
    EXPECT_TRUE(err_result.is_err());
    EXPECT_EQ(err_result.unwrap_err(), "test error");
}

// Test Result with different types
TEST_F(ModernResultTest, DifferentTypes) {
    // String result with int error type
    auto string_result = Result<std::string, int>::ok("hello world");
    EXPECT_TRUE(string_result.is_ok());
    EXPECT_EQ(string_result.unwrap(), "hello world");
    
    // Vector result with string error type
    auto vector_result = Result<std::vector<int>, std::string>::ok({1, 2, 3, 4, 5});
    EXPECT_TRUE(vector_result.is_ok());
    auto vec = vector_result.unwrap();
    EXPECT_EQ(vec.size(), 5);
    EXPECT_EQ(vec[0], 1);
    EXPECT_EQ(vec[4], 5);
}

// Test unwrap_or
TEST_F(ModernResultTest, UnwrapOr) {
    auto ok_result = Result<int>::ok(42);
    auto err_result = Result<int>::err("error");
    
    EXPECT_EQ(ok_result.unwrap_or(0), 42);
    EXPECT_EQ(err_result.unwrap_or(0), 0);
}

// Test unwrap_or_else
TEST_F(ModernResultTest, UnwrapOrElse) {
    auto ok_result = Result<int>::ok(42);
    auto err_result = Result<int>::err("error");
    
    EXPECT_EQ(ok_result.unwrap_or_else([]() { return 100; }), 42);
    EXPECT_EQ(err_result.unwrap_or_else([]() { return 100; }), 100);
}

// Test value_ptr and error_ptr
TEST_F(ModernResultTest, PointerAccess) {
    auto ok_result = Result<int>::ok(42);
    auto err_result = Result<int>::err("error");
    
    EXPECT_NE(ok_result.value_ptr(), nullptr);
    EXPECT_EQ(*ok_result.value_ptr(), 42);
    EXPECT_EQ(ok_result.error_ptr(), nullptr);
    
    EXPECT_EQ(err_result.value_ptr(), nullptr);
    EXPECT_NE(err_result.error_ptr(), nullptr);
    EXPECT_EQ(*err_result.error_ptr(), "error");
}

// Test move semantics
TEST_F(ModernResultTest, MoveSemantics) {
    auto original = Result<std::string, int>::ok("hello");
    auto moved = std::move(original);
    
    EXPECT_TRUE(moved.is_ok());
    EXPECT_EQ(moved.unwrap(), "hello");
    
    // Original should be in valid but unspecified state
    // We can't test this directly, but we can ensure it doesn't crash
}

// Test copy semantics
TEST_F(ModernResultTest, CopySemantics) {
    auto original = Result<std::string, int>::ok("hello");
    auto copied = original;
    
    EXPECT_TRUE(original.is_ok());
    EXPECT_TRUE(copied.is_ok());
    EXPECT_EQ(original.unwrap(), "hello");
    EXPECT_EQ(copied.unwrap(), "hello");
}

// Test error handling
TEST_F(ModernResultTest, ErrorHandling) {
    auto err_result = Result<int>::err("test error");
    
    EXPECT_THROW(err_result.unwrap(), std::runtime_error);
    EXPECT_NO_THROW(err_result.unwrap_err());
    
    auto ok_result = Result<int>::ok(42);
    EXPECT_THROW(ok_result.unwrap_err(), std::runtime_error);
    EXPECT_NO_THROW(ok_result.unwrap());
}

// Test utility functions
TEST_F(ModernResultTest, UtilityFunctions) {
    auto ok_result = Result<int>::ok(42);
    auto err_result = Result<int>::err("error");
    
    EXPECT_TRUE(ok_result.is_ok());
    EXPECT_TRUE(err_result.is_err());
    EXPECT_EQ(ok_result.unwrap(), 42);
    EXPECT_EQ(err_result.unwrap_err(), "error");
}

// Test type aliases
TEST_F(ModernResultTest, TypeAliases) {
    ResultT<int> ok_result = ResultT<int>::ok(42);
    ResultStr<int> err_result = ResultStr<int>::err("error");
    
    EXPECT_TRUE(ok_result.is_ok());
    EXPECT_TRUE(err_result.is_err());
    EXPECT_EQ(ok_result.unwrap(), 42);
    EXPECT_EQ(err_result.unwrap_err(), "error");
}

// Test complex types
TEST_F(ModernResultTest, ComplexTypes) {
    struct TestStruct {
        int value;
        std::string name;
        
        TestStruct(int v, std::string n) : value(v), name(std::move(n)) {}
    };
    
    auto ok_result = Result<TestStruct, std::string>::ok(TestStruct(42, "test"));
    EXPECT_TRUE(ok_result.is_ok());
    auto struct_val = ok_result.unwrap();
    EXPECT_EQ(struct_val.value, 42);
    EXPECT_EQ(struct_val.name, "test");
}

// Test boolean conversion
TEST_F(ModernResultTest, BooleanConversion) {
    auto ok_result = Result<int>::ok(42);
    auto err_result = Result<int>::err("error");
    
    EXPECT_TRUE(static_cast<bool>(ok_result));
    EXPECT_FALSE(static_cast<bool>(err_result));
}

int main(int argc, char** argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
