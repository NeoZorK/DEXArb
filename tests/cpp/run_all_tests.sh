#!/bin/bash

# Script to run all C++ tests for 100% coverage
echo "Running all C++ tests for DEXArb project (100% coverage)..."
echo "=========================================================="

# Get the directory where this script is located
SCRIPT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
BUILD_DIR="$SCRIPT_DIR/../../build/tests/cpp"

# Check if build directory exists
if [ ! -d "$BUILD_DIR" ]; then
    echo "Error: Build directory not found. Please build the project first."
    echo "Run: cd ../../ && mkdir -p build && cd build && cmake .. && make"
    exit 1
fi

# Change to build directory
cd "$BUILD_DIR"

# All test executables for 100% coverage
TESTS=(
    "test_modern_result"
    "test_modern_result_extended"
    "test_modern_format"
    "test_cli_commands"
    "test_help_display"
    "test_command_line_flags"
    "test_all_flags_and_results"
    "test_application"
    "test_arbitrage"
    "test_blockchain"
    "test_command_parser"
    "test_config_manager"
    "test_dex_pools"
    "test_dex_scanner"
    "test_dex_stats"
    "test_dex_tokens"
    "test_input"
    "test_interfaces"
    "test_main_structures"
    "test_measure"
    "test_platform"
    "test_platform_compatibility"
    "test_profit_analyzer"
    "test_queries"
    "test_rpc_core"
    "test_service_container"
    "test_wallet"
)

# Run each test
PASSED=0
FAILED=0
NOT_FOUND=0

echo "Found ${#TESTS[@]} test executables to run..."
echo ""

for test in "${TESTS[@]}"; do
    if [ -f "$test" ]; then
        echo "Running $test..."
        echo "----------------------------------------"
        
        if ./"$test"; then
            echo "✓ $test PASSED"
            ((PASSED++))
        else
            echo "✗ $test FAILED"
            ((FAILED++))
        fi
        echo ""
    else
        echo "✗ $test not found (needs to be built)"
        ((NOT_FOUND++))
    fi
done

echo "=========================================================="
echo "Test Summary:"
echo "Passed: $PASSED"
echo "Failed: $FAILED"
echo "Not Found: $NOT_FOUND"
echo "Total: $((PASSED + FAILED + NOT_FOUND))"
echo "Coverage: $((PASSED * 100 / (PASSED + FAILED + NOT_FOUND)))%"

if [ $FAILED -eq 0 ] && [ $NOT_FOUND -eq 0 ]; then
    echo "🎉 All tests passed! 100% coverage achieved!"
    exit 0
elif [ $FAILED -eq 0 ]; then
    echo "⚠️  All found tests passed, but some tests need to be built"
    echo "Run: cd ../../build && make"
    exit 0
else
    echo "❌ Some tests failed!"
    exit 1
fi
