# DEXArb Test Suite

This directory contains comprehensive C++ tests for the DEXArb application, ensuring code quality and reliability using Google Test framework.

## 🧪 Test Structure

### Test Files
- **`test_main.cpp`** - Tests for main application functions
- **`cpp/test_modern_format.cpp`** - Tests for modern formatting utilities
- **`cpp/test_modern_result.cpp`** - Tests for modern Result<T,E> class
- **`cpp/test_arbitrage.cpp`** - Tests for arbitrage functionality
- **`cpp/test_blockchain.cpp`** - Tests for blockchain functionality
- **`cpp/test_dex_pools.cpp`** - Tests for DEX pools management
- **`cpp/test_dex_tokens.cpp`** - Tests for token handling
- **`cpp/test_cli_commands.cpp`** - Tests for CLI command parsing

### Test Categories
- **Unit Tests** - Test individual functions and components
- **Integration Tests** - Test complete workflows and component interactions
- **Performance Tests** - Test execution time and resource usage

## 🚀 Running Tests

### Prerequisites
- CMake 3.28+
- Google Test framework
- C++23 compatible compiler

### Build Tests
```bash
mkdir cmake-build-debug
cd cmake-build-debug
cmake -G "Unix Makefiles" ..
```

### Run All Tests
```bash
# Build all test targets
cmake --build . --target ModernFormatTests
cmake --build . --target ModernResultTests
cmake --build . --target NeoZorKDEXArbTests

# Run individual test suites
./ModernFormatTests
./ModernResultTests
./NeoZorKDEXArbTests
```

### Run Tests with CTest
```bash
# Run all tests
ctest --output-on-failure

# Run specific test
ctest -R ModernFormatTests
```

## 📊 Test Coverage

The test suite provides comprehensive coverage for:
- ✅ Modern C++ utilities (Result<T,E>, formatting)
- ✅ Command line interface functionality
- ✅ Configuration management
- ✅ Blockchain type handling
- ✅ DEX pools and tokens
- ✅ Arbitrage calculations
- ✅ Error handling and validation

## 🎯 Test Goals

1. **100% Code Coverage** - All new features must have complete test coverage
2. **Fast Execution** - Tests run efficiently for quick feedback
3. **Reliable Results** - Tests are deterministic and repeatable
4. **Clear Documentation** - Each test has clear purpose and description

## 🔧 Test Configuration

### CMake Configuration
- Google Test integration
- Parallel build support
- Coverage reporting (when available)
- Warning handling and suppression

### Test Structure
- Test fixtures for common setup
- Mock objects for external dependencies
- Assertion macros for clear test logic

## 📝 Adding New Tests

### Test Naming Convention
- Test files: `test_<module_name>.cpp`
- Test classes: `Test<ClassName>`
- Test methods: `TEST_F(TestClass, TestName)`

### Test Structure
```cpp
TEST_F(TestClassName, TestMethodName) {
    // Arrange
    // Act
    // Assert
}
```

### Example Test
```cpp
TEST_F(ModernResultTest, BasicConstruction) {
    // Test successful result
    auto result = modern::Result<int, std::string>::ok(42);
    EXPECT_TRUE(result.is_ok());
    EXPECT_EQ(result.unwrap(), 42);
    
    // Test error result
    auto error = modern::Result<int, std::string>::err("error");
    EXPECT_TRUE(error.is_err());
    EXPECT_EQ(error.unwrap_err(), "error");
}
```

## 🚨 Troubleshooting

### Common Issues
1. **Build Errors** - Check CMake configuration and dependencies
2. **Test Failures** - Check test output for detailed error messages
3. **Linker Errors** - Verify all required libraries are linked

### Debug Mode
Build with debug information:
```bash
cmake -DCMAKE_BUILD_TYPE=Debug ..
```

### Test Isolation
Run single test executable:
```bash
./ModernFormatTests
```

## 📈 Continuous Integration

Tests are designed to run in CI/CD pipelines:
- Fast execution (< 1 second per test suite)
- Deterministic results
- Clear pass/fail status
- Comprehensive coverage reporting

## 🤝 Contributing

When adding new features:
1. Write tests first (TDD approach)
2. Ensure 100% coverage for new code
3. Follow existing test patterns
4. Update this README if needed
5. Use Google Test best practices
