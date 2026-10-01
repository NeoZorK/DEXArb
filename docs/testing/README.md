# Testing Guide

Welcome to the DEXArb testing guide! This section covers all aspects of testing the project, from unit tests to performance benchmarks and quality assurance.

## 🚀 Quick Start

### Run All Tests
```bash
# Build with tests enabled
mkdir build && cd build
cmake .. -DENABLE_TESTS=ON
make -j$(nproc)

# Run all tests
ctest --output-on-failure

# Run with verbose output
ctest --output-on-failure --verbose
```

### Run Specific Test Suites
```bash
# Run C++ unit tests
./tests/cpp/test_modern_utils
./tests/cpp/test_arbitrage
./tests/cpp/test_dex_scanner

# Run integration tests
./tests/integration/test_network_communication
./tests/integration/test_dex_integration

# Run performance tests
./tests/performance/test_scanning_performance
./tests/performance/test_memory_usage
```

## 📚 What You'll Find Here

This section covers:
- [**Testing Overview**](overview.md) - Testing strategy and framework
- [**Test Reports**](reports/README.md) - Current test status and results
- [**Performance Testing**](performance/README.md) - Benchmarking and profiling
- [**Testing Guide**](testing-guide.md) - How to write and run tests

## 🧪 Testing Strategy

### Test Pyramid
```
                    ┌─────────────────┐
                    │   E2E Tests     │ ← Few, slow, expensive
                    │   (5-10%)       │
                    └─────────────────┘
                           │
                    ┌─────────────────┐
                    │ Integration     │ ← Some, medium speed
                    │ Tests (15-20%)  │
                    └─────────────────┘
                           │
                    ┌─────────────────┐
                    │   Unit Tests    │ ← Many, fast, cheap
                    │   (70-80%)      │
                    └─────────────────┘
```

### Test Types

#### 1. Unit Tests
- **Purpose**: Test individual components in isolation
- **Scope**: Single class or function
- **Speed**: Fast (<1ms per test)
- **Dependencies**: Mocked external dependencies

#### 2. Integration Tests
- **Purpose**: Test component interactions
- **Scope**: Multiple components working together
- **Speed**: Medium (1-100ms per test)
- **Dependencies**: Real internal components, mocked external

#### 3. End-to-End Tests
- **Purpose**: Test complete workflows
- **Scope**: Full application from input to output
- **Speed**: Slow (100ms-10s per test)
- **Dependencies**: Real external services (test networks)

#### 4. Performance Tests
- **Purpose**: Measure performance characteristics
- **Scope**: Critical performance paths
- **Speed**: Variable (depends on test complexity)
- **Dependencies**: Controlled test environment

## 🏗️ Testing Framework

### C++ Testing (Google Test)
```cpp
#include <gtest/gtest.h>
#include "core/arbitrage.h"

class ArbitrageTest : public ::testing::Test {
protected:
    void SetUp() override {
        engine = std::make_unique<ArbitrageEngine>();
        testPools = createTestPools();
    }
    
    void TearDown() override {
        engine.reset();
        testPools.clear();
    }
    
    std::vector<Pool> createTestPools() {
        return {
            Pool{"Uniswap", {"USDC", "ETH"}, 1000000, 2000.0, 0.3},
            Pool{"SushiSwap", {"USDC", "ETH"}, 800000, 2005.0, 0.3}
        };
    }
    
    std::unique_ptr<ArbitrageEngine> engine;
    std::vector<Pool> testPools;
};

TEST_F(ArbitrageTest, BasicProfitCalculation) {
    auto opportunities = engine->findOpportunities(testPools, 0.1);
    
    ASSERT_FALSE(opportunities.empty());
    EXPECT_GT(opportunities[0].profit, 0.0);
    EXPECT_LT(opportunities[0].profit, 1.0);
}

TEST_F(ArbitrageTest, NoOpportunitiesBelowThreshold) {
    auto opportunities = engine->findOpportunities(testPools, 10.0);
    
    EXPECT_TRUE(opportunities.empty());
}
```

### Mock Objects
```cpp
#include <gmock/gmock.h>

class MockRPCClient : public IRPCClient {
public:
    MOCK_METHOD(Result<std::string>, call, 
        (const std::string& method, const json& params), (override));
    
    MOCK_METHOD(Result<json>, callJSON, 
        (const std::string& method, const json& params), (override));
    
    MOCK_METHOD(bool, connect, (), (override));
    MOCK_METHOD(void, disconnect, (), (override));
    MOCK_METHOD(bool, isConnected, (), (const, override));
};

TEST_F(ArbitrageTest, NetworkFailureHandling) {
    auto mockRPC = std::make_unique<MockRPCClient>();
    
    EXPECT_CALL(*mockRPC, call(_, _))
        .WillOnce(Return(Result<std::string>::error("Network error")));
    
    // Test error handling
    auto result = engine->scanWithRPC(*mockRPC);
    EXPECT_FALSE(result.isSuccess());
    EXPECT_EQ(result.error(), "Network error");
}
```

## 📊 Test Coverage

### Coverage Targets
- **Overall Coverage**: >90%
- **Core Components**: >95%
- **Utility Functions**: >85%
- **Network Layer**: >90%

### Generate Coverage Report
```bash
# Build with coverage
cmake .. -DENABLE_TESTS=ON -DENABLE_COVERAGE=ON
make -j$(nproc)

# Run tests
ctest --output-on-failure

# Generate coverage report
make coverage

# View in browser
open coverage/index.html
```

### Coverage Analysis
```bash
# Check specific file coverage
gcov -r src/core/arbitrage.cpp

# Generate detailed report
lcov --capture --directory . --output-file coverage.info
genhtml coverage.info --output-directory coverage_html
```

## 🚀 Performance Testing

### Benchmark Tests
```cpp
#include <benchmark/benchmark.h>

static void BM_ProfitCalculation(benchmark::State& state) {
    ArbitrageEngine engine;
    auto pools = createLargePoolSet(state.range(0));
    
    for (auto _ : state) {
        auto opportunities = engine.findOpportunities(pools, 0.1);
        benchmark::DoNotOptimize(opportunities);
    }
    
    state.SetComplexityN(state.range(0));
}

BENCHMARK(BM_ProfitCalculation)
    ->Range(10, 10000)
    ->Complexity(benchmark::oN);

BENCHMARK_MAIN();
```

### Memory Profiling
```bash
# Run with Valgrind
valgrind --tool=memcheck --leak-check=full ./tests/cpp/test_arbitrage

# Run with AddressSanitizer
export ASAN_OPTIONS="detect_leaks=1"
./tests/cpp/test_arbitrage

# Profile with Massif
valgrind --tool=massif ./tests/cpp/test_arbitrage
ms_print massif.out.* > memory_profile.txt
```

### Performance Benchmarks
```bash
# Run performance tests
./tests/performance/test_scanning_performance

# Generate performance report
./tests/performance/generate_report.py

# Compare with baseline
./tests/performance/compare_baseline.py
```

## 🔍 Test Data Management

### Test Fixtures
```cpp
class TestDataFixture {
public:
    static std::vector<Pool> getStandardPools() {
        return {
            Pool{"Uniswap", {"USDC", "ETH"}, 1000000, 2000.0, 0.3},
            Pool{"SushiSwap", {"USDC", "ETH"}, 800000, 2005.0, 0.3},
            Pool{"Balancer", {"USDC", "ETH"}, 1200000, 1998.0, 0.3}
        };
    }
    
    static std::vector<Token> getStandardTokens() {
        return {
            Token{"USDC", "0xA0b86a33E6441b8c4C8C8C8C8C8C8C8C8C8C8C8C", 6},
            Token{"ETH", "0xC02aaA39b223FE8D0A0e5C4F27eAD9083C756Cc2", 18}
        };
    }
};
```

### Test Database
```bash
# Setup test database
./tests/setup_test_db.sh

# Run tests with database
./tests/cpp/test_database_integration

# Cleanup test data
./tests/cleanup_test_db.sh
```

## 🧹 Test Maintenance

### Test Organization
```
tests/
├── cpp/                    # C++ unit tests
│   ├── core/              # Core component tests
│   ├── dex/               # DEX platform tests
│   ├── network/           # Network layer tests
│   └── utils/             # Utility function tests
├── integration/            # Integration tests
│   ├── network/           # Network integration tests
│   ├── dex/               # DEX integration tests
│   └── end_to_end/        # End-to-end workflow tests
├── performance/            # Performance tests
│   ├── benchmarks/        # Benchmark tests
│   ├── profiling/         # Profiling tests
│   └── stress/            # Stress tests
└── data/                  # Test data and fixtures
    ├── pools/             # Pool test data
    ├── tokens/            # Token test data
    └── networks/          # Network test data
```

### Test Naming Conventions
```cpp
// Test class naming: ComponentTest
class ArbitrageTest : public ::testing::Test {};

// Test method naming: MethodName_Scenario_ExpectedResult
TEST_F(ArbitrageTest, FindOpportunities_ValidPools_ReturnsOpportunities) {}
TEST_F(ArbitrageTest, FindOpportunities_EmptyPools_ReturnsEmpty) {}
TEST_F(ArbitrageTest, FindOpportunities_InvalidInput_ThrowsException) {}
```

### Test Documentation
```cpp
/**
 * @brief Tests arbitrage opportunity discovery
 * 
 * This test verifies that the arbitrage engine correctly identifies
 * profitable trading opportunities between different DEX pools.
 * 
 * Test scenarios:
 * - Valid pools with price differences
 * - Pools with insufficient liquidity
 * - Edge cases with extreme price variations
 * 
 * @see ArbitrageEngine::findOpportunities
 */
TEST_F(ArbitrageTest, FindOpportunities_ValidPools_ReturnsOpportunities) {
    // Test implementation
}
```

## 🔧 Continuous Integration

### CI Pipeline
```yaml
# .github/workflows/test.yml
name: Tests
on: [push, pull_request]

jobs:
  test:
    runs-on: ubuntu-latest
    steps:
      - uses: actions/checkout@v3
      - name: Install dependencies
        run: |
          sudo apt update
          sudo apt install build-essential cmake git
      - name: Build and test
        run: |
          mkdir build && cd build
          cmake .. -DENABLE_TESTS=ON
          make -j$(nproc)
          ctest --output-on-failure
      - name: Generate coverage
        run: |
          make coverage
          # Upload coverage to Codecov
```

### Pre-commit Hooks
```bash
#!/bin/bash
# .git/hooks/pre-commit

echo "Running pre-commit checks..."

# Format code
clang-format -i src/**/*.cpp include/**/*.h

# Run tests
cd build && make test

# Check coverage
make coverage
if [ $(grep -o '[0-9.]*%' coverage/index.html | head -1 | sed 's/%//') -lt 90 ]; then
    echo "Coverage below 90%"
    exit 1
fi

echo "Pre-commit checks passed"
```

## 📈 Test Metrics and Reporting

### Test Results Dashboard
```bash
# Generate test report
ctest --output-on-failure --verbose > test_results.txt

# Parse results
./tests/parse_results.py test_results.txt

# Generate HTML report
./tests/generate_html_report.py
```

### Performance Tracking
```bash
# Run performance benchmarks
./tests/performance/run_benchmarks.sh

# Compare with previous runs
./tests/performance/compare_results.py

# Generate performance report
./tests/performance/generate_report.py
```

## 🆘 Troubleshooting

### Common Test Issues

#### Test Failures
```bash
# Run specific failing test
./tests/cpp/test_arbitrage --gtest_filter="ArbitrageTest.FindOpportunities*"

# Run with debug output
./tests/cpp/test_arbitrage --gtest_filter="ArbitrageTest.FindOpportunities*" --gtest_also_run_disabled_tests
```

#### Build Issues
```bash
# Clean build
rm -rf build && mkdir build
cd build && cmake .. -DENABLE_TESTS=ON

# Check dependencies
ldd ./tests/cpp/test_arbitrage

# Debug CMake
cmake .. -DENABLE_TESTS=ON -DCMAKE_VERBOSE_MAKEFILE=ON
```

#### Performance Issues
```bash
# Profile test execution
perf record ./tests/cpp/test_arbitrage
perf report

# Check memory usage
valgrind --tool=massif ./tests/cpp/test_arbitrage
```

## 📚 Next Steps

After reading this testing guide:
- [Explore the testing overview](overview.md) for strategy details
- [Check test reports](reports/README.md) for current status
- [Learn about performance testing](performance/README.md) for benchmarks
- [Review the testing guide](testing-guide.md) for detailed procedures

---

*Ready to test? Let's ensure DEXArb is robust and reliable!*
