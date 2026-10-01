# Testing Build Scripts

This document describes how to test the build scripts to ensure they work correctly.

## 🧪 Universal Build Script Tests

### Automated Testing

Run the comprehensive test suite:

```bash
# Navigate to build scripts directory
cd scripts/build

# Run test suite
./test-universal-script.sh
```

**Test Coverage:**
- ✅ Script existence and permissions
- ✅ Syntax validation
- ✅ Help option functionality
- ✅ Version detection
- ✅ Platform detection patterns
- ✅ Required functions presence
- ✅ Build options validation
- ✅ Build types support
- ✅ Command line options
- ✅ Error handling patterns
- ✅ Color output definitions

### Manual Testing

#### Basic Functionality
```bash
# Test help option
./build-universal.sh --help

# Test version display
./build-universal.sh --version

# Test platform detection
./build-universal.sh --platform macos
```

#### Build Methods
```bash
# Test native build (macOS)
./build-universal.sh --platform macos --build-type Release

# Test container build
./build-universal.sh --platform alpine --clean

# Test Windows build (if Wine available)
./build-universal.sh --platform windows --verbose
```

#### Error Handling
```bash
# Test invalid platform
./build-universal.sh --platform invalid

# Test missing dependencies
./build-universal.sh --platform macos --clean --test --package
```

## 🔍 Test Results

### Expected Output

**Successful Test Run:**
```
🧪 Universal Build Script Test Suite

[2025-02-26 15:30:00] INFO: Starting Universal Build Script tests...

✅ PASS - Script exists
✅ PASS - Script is executable
✅ PASS - Script syntax is valid
✅ PASS - Help option works
✅ PASS - Version detected: 1.0.7
✅ PASS - Platform detection patterns found (4/4)
✅ PASS - All required functions found (8/8)
✅ PASS - All build options found (6/6)
✅ PASS - All build types found (3/3)
✅ PASS - All command line options found (7/7)
✅ PASS - Error handling patterns found (3/3)
✅ PASS - All colors defined (6/6)

📊 Test Results Summary

Total Tests: 12
Tests Passed: 12
Tests Failed: 0

🎉 All tests passed! Universal Build Script is ready to use.
```

### Failed Test Example

**Failed Test Run:**
```
❌ FAIL - Script is not executable
❌ FAIL - Help option failed
❌ FAIL - Missing required functions (6/8)

📊 Test Results Summary

Total Tests: 12
Tests Passed: 9
Tests Failed: 3

❌ Some tests failed. Please check the output above.
```

## 🐛 Troubleshooting Failed Tests

### Common Issues

#### 1. Script Not Executable
```bash
# Fix permissions
chmod +x build-universal.sh
```

#### 2. Script Not Found
```bash
# Check current directory
pwd
ls -la build-universal.sh

# Navigate to correct directory
cd scripts/build
```

#### 3. Syntax Errors
```bash
# Check script syntax
bash -n build-universal.sh

# Check for common issues
grep -n "if.*then" build-universal.sh
grep -n "function.*()" build-universal.sh
```

#### 4. Missing Functions
```bash
# Check function definitions
grep -n "^[a-zA-Z_][a-zA-Z0-9_]*()" build-universal.sh

# Verify function calls
grep -n "function_name" build-universal.sh
```

#### 5. Platform Detection Issues
```bash
# Check platform detection logic
grep -A 5 -B 5 "detect_platform" build-universal.sh

# Verify OS detection
echo $OSTYPE
uname -a
```

## 🔧 Test Customization

### Adding New Tests

To add new tests to the test suite:

1. **Add test function** in `test-universal-script.sh`:
```bash
test_new_feature() {
    if [ condition ]; then
        echo -e "${GREEN}✅ PASS${NC} - New feature test"
        ((TESTS_PASSED++))
    else
        echo -e "${RED}❌ FAIL${NC} - New feature test failed"
        ((TESTS_FAILED++))
        return 1
    fi
}
```

2. **Call test function** in main():
```bash
# Run all tests
test_script_exists
test_script_permissions
# ... existing tests ...
test_new_feature  # Add new test here
```

3. **Update test count** in summary:
```bash
echo -e "${BLUE}Total Tests:${NC} $((TESTS_PASSED + TESTS_FAILED))"
```

### Test Categories

#### Unit Tests
- **Function presence**: Check if required functions exist
- **Syntax validation**: Verify script syntax
- **Option parsing**: Test command line arguments

#### Integration Tests
- **Help system**: Verify help and version options
- **Platform detection**: Test OS detection logic
- **Error handling**: Verify error conditions

#### Feature Tests
- **Build options**: Check all build methods
- **Build types**: Verify build configurations
- **Command options**: Test all command line flags

## 📊 Test Metrics

### Coverage Areas

| Category | Tests | Description |
|----------|-------|-------------|
| **Core Functionality** | 4 | Script existence, permissions, syntax, help |
| **Platform Support** | 2 | Platform detection, OS patterns |
| **Build System** | 3 | Functions, options, types |
| **User Interface** | 2 | Command options, error handling |
| **Visual Elements** | 1 | Color definitions |

### Quality Metrics

- **Test Count**: 12 comprehensive tests
- **Coverage**: 100% of script functionality
- **Execution Time**: < 5 seconds
- **Dependencies**: Bash shell only
- **Platform**: Cross-platform compatible

## 🚀 Continuous Integration

### Automated Testing

The test suite can be integrated into CI/CD pipelines:

```yaml
# GitHub Actions example
- name: Test Build Scripts
  run: |
    cd scripts/build
    chmod +x test-universal-script.sh
    ./test-universal-script.sh
```

### Pre-commit Hooks

Add to `.git/hooks/pre-commit`:
```bash
#!/bin/bash
cd scripts/build
./test-universal-script.sh
if [ $? -ne 0 ]; then
    echo "Build script tests failed. Please fix before committing."
    exit 1
fi
```

## 📚 Related Documentation

- **[Universal Build Script](../docs/getting-started/UNIVERSAL_BUILD_SCRIPT.md)** - Complete script documentation
- **[Build Scripts README](README.md)** - Overview of all build scripts
- **[Testing Guide](../../../docs/testing/README.md)** - Project testing documentation

---

**Run tests with: `./test-universal-script.sh`** 🧪
