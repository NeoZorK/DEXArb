# C++ Tests for DEXArb Project

This directory contains comprehensive C++ unit tests for the DEXArb project.

## Test Coverage

The test suite covers the following components:

- **Modern Result Tests**: Testing the modern result wrapper implementation
- **CLI Commands**: Testing command-line interface functionality
- **Help Display**: Testing help and display utilities
- **Command Line Flags**: Testing command-line argument parsing
- **Modern Format**: Testing formatting utilities
- **All Flags and Results**: Comprehensive testing of all components

## Building Tests

### Prerequisites

- CMake 3.16 or higher
- C++20 compatible compiler
- Google Test framework
- libcurl development libraries

### Build Commands

```bash
# From project root
mkdir -p build
cd build
cmake ..
make -j$(nproc)  # On macOS: make -j$(sysctl -n hw.ncpu)
```

## Running Tests

### Option 1: Using the test runner script

```bash
# From project root
./tests/cpp/run_all_tests.sh
```

### Option 2: Using make target

```bash
# From build directory
make run_tests
```

### Option 3: Running individual tests

```bash
# From build/tests/cpp directory
./test_modern_result
./test_cli_commands
./test_help_display
./test_command_line_flags
./test_modern_format
./test_modern_result_extended
./test_all_flags_and_results
```

## Test Results

All tests should pass successfully. The test suite validates:

- Core functionality of all modules
- Error handling and edge cases
- Memory management and resource handling
- API consistency and correctness
- Performance characteristics

## Test Structure

Each test file follows Google Test conventions:

- Test classes inherit from `::testing::Test`
- Individual test methods use `TEST_F()` macro
- Setup and teardown in `SetUp()` and `TearDown()` methods
- Comprehensive assertions using Google Test macros

## Adding New Tests

When adding new functionality:

1. Create corresponding test file in this directory
2. Follow existing naming conventions
3. Ensure 100% test coverage for new features
4. Update this README with new test information
5. Verify all tests pass before committing

## Troubleshooting

### Common Issues

1. **Build failures**: Ensure all dependencies are installed
2. **Linker errors**: Check that source files are properly linked
3. **Test failures**: Review test output for specific failure details

### Dependencies

- **Google Test**: `brew install googletest` (macOS)
- **libcurl**: `brew install curl` (macOS)
- **CMake**: `brew install cmake` (macOS)

## Performance

Tests are designed to run quickly (typically under 2 seconds total) while maintaining comprehensive coverage. Performance tests are included where appropriate to ensure code quality.
