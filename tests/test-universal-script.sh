#!/bin/bash

# Test script for Universal Build Script
# Tests basic functionality without actually building

set -euo pipefail

# Colors for output
RED='\033[0;31m'
GREEN='\033[0;32m'
YELLOW='\033[1;33m'
BLUE='\033[0;34m'
CYAN='\033[0;36m'
NC='\033[0m' # No Color

# Test counter
TESTS_PASSED=0
TESTS_FAILED=0

# Logging function
log() {
    local level=$1
    shift
    local message="$*"
    local timestamp=$(date '+%Y-%m-%d %H:%M:%S')
    
    case $level in
        "INFO")
            echo -e "${GREEN}[${timestamp}] INFO:${NC} $message"
            ;;
        "WARN")
            echo -e "${YELLOW}[${timestamp}] WARN:${NC} $message"
            ;;
        "ERROR")
            echo -e "${RED}[${timestamp}] ERROR:${NC} $message"
            ;;
        "SUCCESS")
            echo -e "${CYAN}[${timestamp}] SUCCESS:${NC} $message"
            ;;
    esac
}

# Test function
run_test() {
    local test_name="$1"
    local test_command="$2"
    local expected_exit="$3"
    
    echo -e "${BLUE}Testing:${NC} $test_name"
    
    if eval "$test_command" > /dev/null 2>&1; then
        local exit_code=$?
        if [ "$exit_code" -eq "$expected_exit" ]; then
            echo -e "${GREEN}✅ PASS${NC} - $test_name"
            ((TESTS_PASSED++))
        else
            echo -e "${RED}❌ FAIL${NC} - $test_name (expected exit $expected_exit, got $exit_code)"
            ((TESTS_FAILED++))
        fi
    else
        local exit_code=$?
        if [ "$exit_code" -eq "$expected_exit" ]; then
            echo -e "${GREEN}✅ PASS${NC} - $test_name"
            ((TESTS_PASSED++))
        else
            echo -e "${RED}❌ FAIL${NC} - $test_name (expected exit $expected_exit, got $exit_code)"
            ((TESTS_FAILED++))
        fi
    fi
}

# Test script existence
test_script_exists() {
    if [ -f "../scripts/build/build-universal.sh" ]; then
        echo -e "${GREEN}✅ PASS${NC} - Script exists"
        ((TESTS_PASSED++))
    else
        echo -e "${RED}❌ FAIL${NC} - Script not found"
        ((TESTS_FAILED++))
        return 1
    fi
}

# Test script permissions
test_script_permissions() {
    if [ -x "../scripts/build/build-universal.sh" ]; then
        echo -e "${GREEN}✅ PASS${NC} - Script is executable"
        ((TESTS_PASSED++))
    else
        echo -e "${RED}❌ FAIL${NC} - Script is not executable"
        ((TESTS_FAILED++))
        return 1
    fi
}

# Test script syntax
test_script_syntax() {
    if bash -n "../scripts/build/build-universal.sh" 2>/dev/null; then
        echo -e "${GREEN}✅ PASS${NC} - Script syntax is valid"
        ((TESTS_PASSED++))
    else
        echo -e "${RED}❌ FAIL${NC} - Script syntax errors found"
        ((TESTS_FAILED++))
        return 1
    fi
}

# Test help option
test_help_option() {
    if ../scripts/build/build-universal.sh --help > /dev/null 2>&1; then
        echo -e "${GREEN}✅ PASS${NC} - Help option works"
        ((TESTS_PASSED++))
    else
        echo -e "${RED}❌ FAIL${NC} - Help option failed"
        ((TESTS_FAILED++))
        return 1
    fi
}

# Test version detection
test_version_detection() {
    local version=$(grep -o 'VERSION="[^"]*"' ../scripts/build/build-universal.sh | cut -d'"' -f2)
    if [ -n "$version" ]; then
        echo -e "${GREEN}✅ PASS${NC} - Version detected: $version"
        ((TESTS_PASSED++))
    else
        echo -e "${RED}❌ FAIL${NC} - Version not detected"
        ((TESTS_FAILED++))
        return 1
    fi
}

# Test platform detection
test_platform_detection() {
    local platform_patterns=("darwin" "linux-gnu" "alpine-release" "msys")
    local found_patterns=0
    
    for pattern in "${platform_patterns[@]}"; do
        if grep -q "$pattern" ../scripts/build/build-universal.sh; then
            ((found_patterns++))
        fi
    done
    
    if [ "$found_patterns" -ge 2 ]; then
        echo -e "${GREEN}✅ PASS${NC} - Platform detection patterns found ($found_patterns/4)"
        ((TESTS_PASSED++))
    else
        echo -e "${RED}❌ FAIL${NC} - Platform detection patterns missing ($found_patterns/4)"
        ((TESTS_FAILED++))
        return 1
    fi
}

# Test required functions
test_required_functions() {
    local required_functions=(
        "detect_platform"
        "check_dependencies"
        "build_native"
        "build_container"
        "build_windows"
        "run_tests"
        "create_package"
        "show_build_summary"
    )
    
    local found_functions=0
    
    for func in "${required_functions[@]}"; do
        if grep -q "$func" ../scripts/build/build-universal.sh; then
            ((found_functions++))
        fi
    done
    
    if [ "$found_functions" -eq "${#required_functions[@]}" ]; then
        echo -e "${GREEN}✅ PASS${NC} - All required functions found ($found_functions/${#required_functions[@]})"
        ((TESTS_PASSED++))
    else
        echo -e "${RED}❌ FAIL${NC} - Missing required functions ($found_functions/${#required_functions[@]})"
        ((TESTS_FAILED++))
        return 1
    fi
}

# Test build options
test_build_options() {
    local build_options=(
        "Native build"
        "Container build"
        "Cross-platform build"
        "Windows build"
        "Alpine container build"
        "Ubuntu container build"
    )
    
    local found_options=0
    
    for option in "${build_options[@]}"; do
        if grep -q "$option" ../scripts/build/build-universal.sh; then
            ((found_options++))
        fi
    done
    
    if [ "$found_options" -eq "${#build_options[@]}" ]; then
        echo -e "${GREEN}✅ PASS${NC} - All build options found ($found_options/${#build_options[@]})"
        ((TESTS_PASSED++))
    else
        echo -e "${RED}❌ FAIL${NC} - Missing build options ($found_options/${#build_options[@]})"
        ((TESTS_FAILED++))
        return 1
    fi
}

# Test build types
test_build_types() {
    local build_types=("Release" "Debug" "RelWithDebInfo")
    local found_types=0
    
    for type in "${build_types[@]}"; do
        if grep -q "$type" ../scripts/build/build-universal.sh; then
            ((found_types++))
        fi
    done
    
    if [ "$found_types" -eq "${#build_types[@]}" ]; then
        echo -e "${GREEN}✅ PASS${NC} - All build types found ($found_types/${#build_types[@]})"
        ((TESTS_PASSED++))
    else
        echo -e "${RED}❌ FAIL${NC} - Missing build types ($found_types/${#build_types[@]})"
        ((TESTS_FAILED++))
        return 1
    fi
}

# Test command line options
test_command_line_options() {
    local options=(
        "--help"
        "--platform"
        "--build-type"
        "--clean"
        "--test"
        "--package"
        "--verbose"
    )
    
    local found_options=0
    
    for option in "${options[@]}"; do
        if grep -q "$option" ../scripts/build/build-universal.sh; then
            ((found_options++))
        fi
    done
    
    if [ "$found_options" -eq "${#options[@]}" ]; then
        echo -e "${GREEN}✅ PASS${NC} - All command line options found ($found_options/${#options[@]})"
        ((TESTS_PASSED++))
    else
        echo -e "${RED}❌ FAIL${NC} - Missing command line options ($found_options/${#options[@]})"
        ((TESTS_FAILED++))
        return 1
    fi
}

# Test error handling
test_error_handling() {
    local error_patterns=("set -euo pipefail" "exit 1" "log \"ERROR\"")
    local found_patterns=0
    
    for pattern in "${error_patterns[@]}"; do
        if grep -q "$pattern" ../scripts/build/build-universal.sh; then
            ((found_patterns++))
        fi
    done
    
    if [ "$found_patterns" -eq "${#error_patterns[@]}" ]; then
        echo -e "${GREEN}✅ PASS${NC} - Error handling patterns found ($found_patterns/${#error_patterns[@]})"
        ((TESTS_PASSED++))
    else
        echo -e "${RED}❌ FAIL${NC} - Error handling patterns missing ($found_patterns/${#error_patterns[@]})"
        ((TESTS_FAILED++))
        return 1
    fi
}

# Test color output
test_color_output() {
    local colors=("RED=" "GREEN=" "YELLOW=" "BLUE=" "CYAN=" "NC=")
    local found_colors=0
    
    for color in "${colors[@]}"; do
        if grep -q "$color" ../scripts/build/build-universal.sh; then
            ((found_colors++))
        fi
    done
    
    if [ "$found_colors" -eq "${#colors[@]}" ]; then
        echo -e "${GREEN}✅ PASS${NC} - All colors defined ($found_colors/${#colors[@]})"
        ((TESTS_PASSED++))
    else
        echo -e "${RED}❌ FAIL${NC} - Missing color definitions ($found_colors/${#colors[@]})"
        ((TESTS_FAILED++))
        return 1
    fi
}

# Main test function
main() {
    echo -e "${CYAN}╔══════════════════════════════════════════════════════════════════════════════╗${NC}"
    echo -e "${CYAN}║${NC}                    ${GREEN}🧪 Universal Build Script Test Suite${NC}                        ${CYAN}║${NC}"
    echo -e "${CYAN}╚══════════════════════════════════════════════════════════════════════════════╝${NC}"
    echo ""
    
    log "INFO" "Starting Universal Build Script tests..."
    echo ""
    
    # Run all tests
    test_script_exists
    test_script_permissions
    test_script_syntax
    test_help_option
    test_version_detection
    test_platform_detection
    test_required_functions
    test_build_options
    test_build_types
    test_command_line_options
    test_error_handling
    test_color_output
    
    echo ""
    echo -e "${CYAN}╔══════════════════════════════════════════════════════════════════════════════╗${NC}"
    echo -e "${CYAN}║${NC}                    ${GREEN}📊 Test Results Summary${NC}                                ${CYAN}║${NC}"
    echo -e "${CYAN}╚══════════════════════════════════════════════════════════════════════════════╝${NC}"
    echo ""
    
    echo -e "${BLUE}Total Tests:${NC} $((TESTS_PASSED + TESTS_FAILED))"
    echo -e "${GREEN}Tests Passed:${NC} $TESTS_PASSED"
    echo -e "${RED}Tests Failed:${NC} $TESTS_FAILED"
    echo ""
    
    if [ "$TESTS_FAILED" -eq 0 ]; then
        echo -e "${GREEN}🎉 All tests passed! Universal Build Script is ready to use.${NC}"
        exit 0
    else
        echo -e "${RED}❌ Some tests failed. Please check the output above.${NC}"
        exit 1
    fi
}

# Run main function
main "$@"
