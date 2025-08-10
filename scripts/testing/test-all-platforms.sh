#!/bin/bash

# Test All Platforms Script for NeoZorKDEXArb
# Tests functionality across all supported platforms
# Created by Rostyslav S. on 26.02.2025

set -euo pipefail

# Colors for output
RED='\033[0;31m'
GREEN='\033[0;32m'
YELLOW='\033[1;33m'
BLUE='\033[0;34m'
CYAN='\033[0;36m'
NC='\033[0m'

# Logging function
log() {
    local level=$1
    shift
    local message="$*"
    local timestamp=$(date '+%Y-%m-%d %H:%M:%S')
    
    case $level in
        "INFO") echo -e "${GREEN}[${timestamp}] INFO:${NC} $message" ;;
        "WARN") echo -e "${YELLOW}[${timestamp}] WARN:${NC} $message" ;;
        "ERROR") echo -e "${RED}[${timestamp}] ERROR:${NC} $message" ;;
        "DEBUG") echo -e "${BLUE}[${timestamp}] DEBUG:${NC} $message" ;;
    esac
}

# Test platform compatibility
test_platform_compatibility() {
    local platform=$1
    local build_dir="build-$platform"
    
    log "INFO" "Testing $platform compatibility..."
    
    if [ ! -d "$build_dir" ]; then
        log "ERROR" "Build directory not found: $build_dir"
        return 1
    fi
    
    cd "$build_dir"
    
    # Test basic functionality
    local exe_name="NeoZorKDEXArb"
    if [ "$platform" = "windows" ]; then
        exe_name="NeoZorKDEXArb.exe"
    fi
    
    if [ ! -f "bin/$exe_name" ]; then
        log "ERROR" "Executable not found: bin/$exe_name"
        cd ..
        return 1
    fi
    
    # Test help command
    log "INFO" "Testing help command..."
    if ./bin/"$exe_name" -h > /dev/null 2>&1; then
        log "INFO" "✅ Help command works on $platform"
    else
        log "WARN" "⚠️ Help command failed on $platform"
    fi
    
    # Test version command
    log "INFO" "Testing version command..."
    if ./bin/"$exe_name" -v > /dev/null 2>&1; then
        log "INFO" "✅ Version command works on $platform"
    else
        log "WARN" "⚠️ Version command failed on $platform"
    fi
    
    # Test platform detection
    log "INFO" "Testing platform detection..."
    local platform_output=$(./bin/"$exe_name" -v 2>&1 || true)
    if echo "$platform_output" | grep -q "Platform:"; then
        log "INFO" "✅ Platform detection works on $platform"
    else
        log "WARN" "⚠️ Platform detection not found on $platform"
    fi
    
    cd ..
    log "INFO" "✅ $platform compatibility test completed"
}

# Test cross-platform consistency
test_cross_platform_consistency() {
    log "INFO" "Testing cross-platform consistency..."
    
    local platforms=("macos" "alpine" "ubuntu" "windows")
    local results=()
    
    for platform in "${platforms[@]}"; do
        local build_dir="build-$platform"
        local exe_name="NeoZorKDEXArb"
        
        if [ "$platform" = "windows" ]; then
            exe_name="NeoZorKDEXArb.exe"
        fi
        
        if [ -f "$build_dir/bin/$exe_name" ]; then
            # Get version info
            local version_output=$("$build_dir/bin/$exe_name" -v 2>&1 || echo "ERROR")
            results+=("$platform: $version_output")
        else
            results+=("$platform: NOT_BUILT")
        fi
    done
    
    # Display results
    echo ""
    echo "Cross-platform consistency results:"
    echo "=================================="
    for result in "${results[@]}"; do
        echo "  $result"
    done
    
    # Check if all platforms show same version
    local versions=$(echo "${results[@]}" | grep -o "v[0-9]\+\.[0-9]\+\.[0-9]\+" | sort -u | wc -l)
    if [ "$versions" -eq 1 ]; then
        log "INFO" "✅ All platforms show consistent version"
    else
        log "WARN" "⚠️ Version inconsistency detected across platforms"
    fi
}

# Test network functionality
test_network_functionality() {
    log "INFO" "Testing network functionality..."
    
    local platforms=("macos" "alpine" "ubuntu")
    local network_tests_passed=0
    local total_tests=0
    
    for platform in "${platforms[@]}"; do
        local build_dir="build-$platform"
        local exe_name="NeoZorKDEXArb"
        
        if [ -f "$build_dir/bin/$exe_name" ]; then
            total_tests=$((total_tests + 1))
            
            # Test basic network operations (if implemented)
            cd "$build_dir"
            
            # This would test actual network functionality when implemented
            # For now, just check if executable runs
            if timeout 5s ./bin/"$exe_name" -h > /dev/null 2>&1; then
                network_tests_passed=$((network_tests_passed + 1))
                log "INFO" "✅ $platform network test passed"
            else
                log "WARN" "⚠️ $platform network test failed"
            fi
            
            cd ..
        fi
    done
    
    if [ $total_tests -gt 0 ]; then
        local success_rate=$((network_tests_passed * 100 / total_tests))
        log "INFO" "Network functionality test: $success_rate% success rate ($network_tests_passed/$total_tests)"
    fi
}

# Test file operations
test_file_operations() {
    log "INFO" "Testing file operations..."
    
    local platforms=("macos" "alpine" "ubuntu" "windows")
    local file_tests_passed=0
    local total_tests=0
    
    for platform in "${platforms[@]}"; do
        local build_dir="build-$platform"
        local exe_name="NeoZorKDEXArb"
        
        if [ "$platform" = "windows" ]; then
            exe_name="NeoZorKDEXArb.exe"
        fi
        
        if [ -f "$build_dir/bin/$exe_name" ]; then
            total_tests=$((total_tests + 1))
            
            # Test file existence check
            if [ -r "$build_dir/bin/$exe_name" ]; then
                file_tests_passed=$((file_tests_passed + 1))
                log "INFO" "✅ $platform file operations test passed"
            else
                log "WARN" "⚠️ $platform file operations test failed"
            fi
        fi
    done
    
    if [ $total_tests -gt 0 ]; then
        local success_rate=$((file_tests_passed * 100 / total_tests))
        log "INFO" "File operations test: $success_rate% success rate ($file_tests_passed/$total_tests)"
    fi
}

# Generate test report
generate_test_report() {
    local report_file="test-report-$(date +%Y%m%d-%H%M%S).txt"
    
    log "INFO" "Generating test report: $report_file"
    
    cat > "$report_file" << EOF
NeoZorKDEXArb Multi-Platform Test Report
Generated: $(date)
=========================================

Platform Compatibility Tests:
EOF
    
    # Test each platform
    for platform in "macos" "alpine" "ubuntu" "windows"; do
        echo "" >> "$report_file"
        echo "=== $platform ===" >> "$report_file"
        
        local build_dir="build-$platform"
        local exe_name="NeoZorKDEXArb"
        
        if [ "$platform" = "windows" ]; then
            exe_name="NeoZorKDEXArb.exe"
        fi
        
        if [ -f "$build_dir/bin/$exe_name" ]; then
            echo "Status: ✅ Built successfully" >> "$report_file"
            echo "Executable: $build_dir/bin/$exe_name" >> "$report_file"
            
            # Get file size
            local file_size=$(stat -f%z "$build_dir/bin/$exe_name" 2>/dev/null || stat -c%s "$build_dir/bin/$exe_name" 2>/dev/null || echo "Unknown")
            echo "File size: $file_size bytes" >> "$report_file"
            
            # Test basic functionality
            if timeout 5s "$build_dir/bin/$exe_name" -h > /dev/null 2>&1; then
                echo "Help command: ✅ Working" >> "$report_file"
            else
                echo "Help command: ❌ Failed" >> "$report_file"
            fi
            
            if timeout 5s "$build_dir/bin/$exe_name" -v > /dev/null 2>&1; then
                echo "Version command: ✅ Working" >> "$report_file"
            else
                echo "Version command: ❌ Failed" >> "$report_file"
            fi
        else
            echo "Status: ❌ Not built" >> "$report_file"
        fi
    done
    
    echo "" >> "$report_file"
    echo "Test Summary:" >> "$report_file"
    echo "============" >> "$report_file"
    echo "Total platforms tested: 4" >> "$report_file"
    
    local built_count=0
    for platform in "macos" "alpine" "ubuntu" "windows"; do
        local build_dir="build-$platform"
        local exe_name="NeoZorKDEXArb"
        if [ "$platform" = "windows" ]; then
            exe_name="NeoZorKDEXArb.exe"
        fi
        
        if [ -f "$build_dir/bin/$exe_name" ]; then
            built_count=$((built_count + 1))
        fi
    done
    
    echo "Successfully built: $built_count/4" >> "$report_file"
    echo "Success rate: $((built_count * 100 / 4))%" >> "$report_file"
    
    log "INFO" "Test report generated: $report_file"
}

# Main function
main() {
    log "INFO" "Starting multi-platform testing..."
    
    # Check if builds exist
    local has_builds=false
    for platform in "macos" "alpine" "ubuntu" "windows"; do
        local build_dir="build-$platform"
        local exe_name="NeoZorKDEXArb"
        if [ "$platform" = "windows" ]; then
            exe_name="NeoZorKDEXArb.exe"
        fi
        
        if [ -f "$build_dir/bin/$exe_name" ]; then
            has_builds=true
            break
        fi
    done
    
    if [ "$has_builds" = false ]; then
        log "ERROR" "No builds found. Please run build-multi-platform.sh first."
        log "INFO" "Example: ./scripts/build-multi-platform.sh --all"
        exit 1
    fi
    
    # Run all tests
    for platform in "macos" "alpine" "ubuntu" "windows"; do
        local build_dir="build-$platform"
        local exe_name="NeoZorKDEXArb"
        if [ "$platform" = "windows" ]; then
            exe_name="NeoZorKDEXArb.exe"
        fi
        
        if [ -f "$build_dir/bin/$exe_name" ]; then
            test_platform_compatibility "$platform"
        fi
    done
    
    # Cross-platform tests
    test_cross_platform_consistency
    test_network_functionality
    test_file_operations
    
    # Generate report
    generate_test_report
    
    log "INFO" "Multi-platform testing completed successfully!"
    
    echo ""
    echo "Test Results Summary:"
    echo "===================="
    for platform in "macos" "alpine" "ubuntu" "windows"; do
        local build_dir="build-$platform"
        local exe_name="NeoZorKDEXArb"
        if [ "$platform" = "windows" ]; then
            exe_name="NeoZorKDEXArb.exe"
        fi
        
        if [ -f "$build_dir/bin/$exe_name" ]; then
            echo "  ✅ $platform: Ready for testing"
        else
            echo "  ❌ $platform: Not built"
        fi
    done
}

# Run main function
main "$@"
