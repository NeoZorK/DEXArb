#!/bin/bash

# Script to run all C++ tests
echo "Running all C++ tests for DEXArb project..."
echo "=========================================="

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

# Test executables
TESTS=(
    "test_modern_result"
    "test_modern_result_extended"
    "test_modern_format"
    "test_cli_commands"
    "test_help_display"
    "test_command_line_flags"
    "test_all_flags_and_results"
)

# Run each test
PASSED=0
FAILED=0

for test in "${TESTS[@]}"; do
    if [ -f "$test" ]; then
        echo ""
        echo "Running $test..."
        echo "----------------------------------------"
        
        if ./"$test"; then
            echo "✓ $test PASSED"
            ((PASSED++))
        else
            echo "✗ $test FAILED"
            ((FAILED++))
        fi
    else
        echo "✗ $test not found"
        ((FAILED++))
    fi
done

echo ""
echo "=========================================="
echo "Test Summary:"
echo "Passed: $PASSED"
echo "Failed: $FAILED"
echo "Total: $((PASSED + FAILED))"

if [ $FAILED -eq 0 ]; then
    echo "🎉 All tests passed!"
    exit 0
else
    echo "❌ Some tests failed!"
    exit 1
fi
