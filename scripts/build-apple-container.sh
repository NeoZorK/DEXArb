#!/bin/bash

# Modern Build Script for NeoZorKDEXArb for macOS
# Created by Rostyslav S. on 26.02.2025
# Updated to work without Apple Container

set -euo pipefail  # Exit on error, undefined vars, pipe failures

# Colors for output
RED='\033[0;31m'
GREEN='\033[0;32m'
YELLOW='\033[1;33m'
BLUE='\033[0;34m'
CYAN='\033[0;36m'
NC='\033[0m' # No Color

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
        "DEBUG")
            echo -e "${BLUE}[${timestamp}] DEBUG:${NC} $message"
            ;;
    esac
}

# Function to check dependencies
check_dependencies() {
    log "INFO" "Checking build dependencies..."
    
    # Check for CMake
    if ! command -v cmake &> /dev/null; then
        log "ERROR" "CMake not found. Please install CMake 3.28 or higher."
        log "INFO" "Install with: brew install cmake"
        exit 1
    fi
    
    # Check CMake version
    local cmake_version=$(cmake --version | head -n1 | cut -d' ' -f3)
    local required_version="3.28"
    
    if [ "$(printf '%s\n' "$required_version" "$cmake_version" | sort -V | head -n1)" != "$required_version" ]; then
        log "ERROR" "CMake version $cmake_version is too old. Required: $required_version or higher."
        exit 1
    fi
    
    # Check for C++ compiler
    if ! command -v clang++ &> /dev/null; then
        log "ERROR" "clang++ not found. Please install Xcode Command Line Tools."
        log "INFO" "Install with: xcode-select --install"
        exit 1
    fi
    
    # Check for git
    if ! command -v git &> /dev/null; then
        log "ERROR" "git not found. Please install git."
        exit 1
    fi
    
    # Check for vcpkg
    if [ ! -d "vcpkg" ]; then
        log "INFO" "vcpkg not found, will clone it..."
        git clone https://github.com/Microsoft/vcpkg.git
        cd vcpkg
        ./bootstrap-vcpkg.sh
        cd ..
    fi
    
    log "INFO" "All dependencies satisfied"
}

# Function to setup vcpkg
setup_vcpkg() {
    log "INFO" "Setting up vcpkg..."
    
    if [ ! -d "vcpkg" ]; then
        log "INFO" "Cloning vcpkg..."
        git clone https://github.com/Microsoft/vcpkg.git
    fi
    
    cd vcpkg
    
    if [ ! -f "vcpkg" ]; then
        log "INFO" "Bootstrapping vcpkg..."
        ./bootstrap-vcpkg.sh
    fi
    
    # Install required packages
    log "INFO" "Installing required packages from manifest..."
    ./vcpkg install --triplet=arm64-osx
    
    cd ..
    log "INFO" "vcpkg setup completed"
}

# Function to create build environment
create_build_environment() {
    log "INFO" "Creating build environment..."
    
    # Create build directory
    local build_dir="build-apple"
    if [ -d "$build_dir" ]; then
        log "INFO" "Cleaning existing build directory..."
        rm -rf "$build_dir"
    fi
    
    mkdir -p "$build_dir"
    log "INFO" "Build directory created: $build_dir"
}

# Function to build project
build_project() {
    local build_dir="build-apple"
    cd "$build_dir"
    
    log "INFO" "Configuring project with CMake..."
    
    # Configure with CMake
    cmake .. \
        -DCMAKE_TOOLCHAIN_FILE=../vcpkg/scripts/buildsystems/vcpkg.cmake \
        -DCMAKE_BUILD_TYPE=Release \
        -DCMAKE_CXX_COMPILER=clang++ \
        -DCMAKE_CXX_STANDARD=23
    
    if [ $? -ne 0 ]; then
        log "ERROR" "CMake configuration failed"
        exit 1
    fi
    
    log "INFO" "CMake configuration completed successfully"
    
    # Build project
    log "INFO" "Building project..."
    local cpu_count=$(sysctl -n hw.ncpu 2>/dev/null || echo "4")
    log "INFO" "Using $cpu_count CPU cores for parallel build"
    
    make -j"$cpu_count"
    
    if [ $? -ne 0 ]; then
        log "ERROR" "Build failed"
        exit 1
    fi
    
    log "INFO" "Build completed successfully"
    cd ..
}

# Function to run tests
run_tests() {
    log "INFO" "Running tests..."
    
    local build_dir="build-apple"
    local test_executable="$build_dir/NeoZorKDEXArbTests"
    
    if [ -f "$test_executable" ]; then
        log "INFO" "Running C++ tests..."
        cd "$build_dir"
        ./NeoZorKDEXArbTests
        cd ..
    else
        log "WARN" "Test executable not found, skipping tests"
    fi
}

# Function to create distribution package
create_package() {
    log "INFO" "Creating distribution package..."
    
    local build_dir="build-apple"
    local package_dir="dist"
    local executable="$build_dir/bin/NeoZorKDEXArb"
    
    if [ ! -f "$executable" ]; then
        log "ERROR" "Executable not found: $executable"
        exit 1
    fi
    
    # Create distribution directory
    mkdir -p "$package_dir"
    
    # Copy executable
    cp "$executable" "$package_dir/"
    
    # Copy configuration files if they exist
    if [ -f "config.json" ]; then
        cp config.json "$package_dir/"
    fi
    
    # Create README for distribution
    cat > "$package_dir/README.txt" << EOF
NeoZorKDEXArb v1.0.7 - macOS Distribution

This package contains the compiled DEX Arbitrage Scanner for macOS.

Usage:
  ./NeoZorKDEXArb -h          # Show help
  ./NeoZorKDEXArb -v          # Show version
  ./NeoZorKDEXArb -scan ethereum 5000  # Scan Ethereum blockchain

For more information, see the main project documentation.
EOF
    
    log "INFO" "Distribution package created in: $package_dir"
}

# Function to show help
show_help() {
    echo "Usage: $0 [OPTIONS]"
    echo ""
    echo "Options:"
    echo "  --help, -h          Show this help message"
    echo "  --clean             Clean build directory before building"
    echo "  --package           Create distribution package after build"
    echo "  --test              Run tests after build"
    echo "  --all               Build, test, and package (default)"
    echo ""
    echo "Examples:"
    echo "  $0                  # Build, test, and package"
    echo "  $0 --clean          # Clean and rebuild"
    echo "  $0 --package        # Build and create package"
    echo "  $0 --test           # Build and run tests"
}

# Main function
main() {
    local clean_build=false
    local create_package_flag=false
    local run_tests_flag=false
    
    # Parse command line arguments
    while [[ $# -gt 0 ]]; do
        case $1 in
            --help|-h)
                show_help
                exit 0
                ;;
            --clean)
                clean_build=true
                shift
                ;;
            --package)
                create_package_flag=true
                shift
                ;;
            --test)
                run_tests_flag=true
                shift
                ;;
            --all)
                clean_build=true
                create_package_flag=true
                run_tests_flag=true
                shift
                ;;
            *)
                log "ERROR" "Unknown option: $1"
                show_help
                exit 1
                ;;
        esac
    done
    
    # Set defaults if no specific flags
    if [ "$clean_build" = false ] && [ "$create_package_flag" = false ] && [ "$run_tests_flag" = false ]; then
        clean_build=true
        create_package_flag=true
        run_tests_flag=true
    fi
    
    log "INFO" "Starting modern build process for NeoZorKDEXArb..."
    log "INFO" "Build method: native macOS build"
    
    # Check dependencies
    check_dependencies
    
    # Setup vcpkg
    setup_vcpkg
    
    # Create build environment
    create_build_environment
    
    # Build project
    build_project
    
    # Run tests if requested
    if [ "$run_tests_flag" = true ]; then
        run_tests
    fi
    
    # Create package if requested
    if [ "$create_package_flag" = true ]; then
        create_package
    fi
    
    log "INFO" "Build process completed successfully!"
    
    if [ -f "build-apple/bin/NeoZorKDEXArb" ]; then
        log "INFO" "Executable location: build-apple/bin/NeoZorKDEXArb"
    fi
    
    echo ""
    echo "Usage examples:"
    echo "  $0                  # Build, test, and package"
    echo "  $0 --clean          # Clean and rebuild"
    echo "  $0 --package        # Build and create package"
    echo "  $0 --test           # Build and run tests"
}

# Run main function with all arguments
main "$@"
