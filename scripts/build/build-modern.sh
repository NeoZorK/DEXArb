#!/bin/bash

# Modern Build Script for NeoZorKDEXArb
# Created by Rostyslav S. on 26.02.2025

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
    if ! command -v g++ &> /dev/null && ! command -v clang++ &> /dev/null; then
        log "ERROR" "No C++ compiler found. Please install g++ or clang++."
        exit 1
    fi
    
    # Check for libcurl (macOS specific)
    if [[ "$OSTYPE" == "darwin"* ]]; then
        # On macOS, check if curl is available
        if ! command -v curl &> /dev/null; then
            log "ERROR" "curl not found. Please install curl via Homebrew: brew install curl"
            exit 1
        fi
        
        # Check if curl development headers are available
        if [ ! -f "/usr/local/include/curl/curl.h" ] && [ ! -f "/opt/homebrew/include/curl/curl.h" ]; then
            log "WARN" "curl development headers not found in standard locations"
            log "INFO" "CMake will attempt to find curl automatically"
        fi
    else
        # On Linux, use pkg-config if available
        if command -v pkg-config &> /dev/null; then
            if ! pkg-config --exists libcurl; then
                log "ERROR" "libcurl development package not found. Please install libcurl-dev or libcurl-devel."
                exit 1
            fi
        else
            log "WARN" "pkg-config not found, skipping libcurl check"
        fi
    fi
    
    log "INFO" "All dependencies satisfied"
}

# Function to detect OS and set appropriate flags
detect_os() {
    log "INFO" "Detecting operating system..."
    
    if [[ "$OSTYPE" == "linux-gnu"* ]]; then
        OS="linux"
        log "INFO" "Detected Linux"
    elif [[ "$OSTYPE" == "darwin"* ]]; then
        OS="macos"
        log "INFO" "Detected macOS"
    elif [[ "$OSTYPE" == "cygwin" ]] || [[ "$OSTYPE" == "msys" ]]; then
        OS="windows"
        log "INFO" "Detected Windows (Cygwin/MSYS)"
    else
        OS="unknown"
        log "WARN" "Unknown OS type: $OSTYPE"
    fi
}

# Function to create build directory
setup_build() {
    log "INFO" "Setting up build environment..."
    
    # Create build directory
    if [ -d "build" ]; then
        log "INFO" "Cleaning existing build directory..."
        rm -rf build
    fi
    
    mkdir -p build
    cd build
    
    log "INFO" "Build directory created: $(pwd)"
}

# Function to configure with CMake
configure_project() {
    log "INFO" "Configuring project with CMake..."
    
    local cmake_flags=""
    
    # Add OS-specific flags
    case $OS in
        "macos")
            cmake_flags="-DCMAKE_BUILD_TYPE=Release"
            log "INFO" "Using macOS-specific configuration"
            ;;
        "linux")
            cmake_flags="-DCMAKE_BUILD_TYPE=Release"
            log "INFO" "Using Linux-specific configuration"
            ;;
        "windows")
            cmake_flags="-DCMAKE_BUILD_TYPE=Release"
            log "INFO" "Using Windows-specific configuration"
            ;;
        *)
            cmake_flags="-DCMAKE_BUILD_TYPE=Release"
            log "WARN" "Using generic configuration for unknown OS"
            ;;
    esac
    
    # Configure with CMake
    if ! cmake .. $cmake_flags; then
        log "ERROR" "CMake configuration failed"
        exit 1
    fi
    
    log "INFO" "CMake configuration completed successfully"
}

# Function to build the project
build_project() {
    log "INFO" "Building project..."
    
    # Get number of CPU cores for parallel build
    local cores=$(nproc 2>/dev/null || sysctl -n hw.ncpu 2>/dev/null || echo 4)
    log "INFO" "Using $cores CPU cores for parallel build"
    
    # Build with parallel compilation
    if ! make -j$cores; then
        log "ERROR" "Build failed"
        exit 1
    fi
    
    log "INFO" "Build completed successfully"
}

# Function to run tests
run_tests() {
    log "INFO" "Running tests..."
    
    if [ -f "NeoZorKDEXArb" ]; then
        log "INFO" "Testing executable..."
        
        # Test help command
        if ! ./NeoZorKDEXArb -h &> /dev/null; then
            log "ERROR" "Help command test failed"
            exit 1
        fi
        
        # Test version command
        if ! ./NeoZorKDEXArb -v &> /dev/null; then
            log "ERROR" "Version command test failed"
            exit 1
        fi
        
        log "INFO" "Basic tests passed"
    else
        log "WARN" "Executable not found, skipping tests"
    fi
}

# Function to install
install_project() {
    log "INFO" "Installing project..."
    
    if ! make install; then
        log "ERROR" "Installation failed"
        exit 1
    fi
    
    log "INFO" "Installation completed successfully"
}

# Function to create deployment package
create_package() {
    log "INFO" "Creating deployment package..."
    
    local package_name="NeoZorKDEXArb-$(date +%Y%m%d-%H%M%S)"
    local package_dir="../$package_name"
    
    mkdir -p "$package_dir"
    
    # Copy executable
    if [ -f "NeoZorKDEXArb" ]; then
        cp NeoZorKDEXArb "$package_dir/"
        log "INFO" "Executable copied to package"
    fi
    
    # Copy documentation
    if [ -d "../docs" ]; then
        cp -r ../docs "$package_dir/"
        log "INFO" "Documentation copied to package"
    fi
    
    # Copy configuration files
    if [ -d "../config" ]; then
        cp -r ../config "$package_dir/"
        log "INFO" "Configuration files copied to package"
    fi
    
    # Create package archive
    cd ..
    tar -czf "${package_name}.tar.gz" "$package_name"
    
    log "INFO" "Deployment package created: ${package_name}.tar.gz"
    
    # Clean up temporary directory
    rm -rf "$package_name"
}

# Main build process
main() {
    log "INFO" "Starting modern build process for NeoZorKDEXArb..."
    
    # Store original directory
    local original_dir=$(pwd)
    
    # Check dependencies
    check_dependencies
    
    # Detect OS
    detect_os
    
    # Setup build environment
    setup_build
    
    # Configure project
    configure_project
    
    # Build project
    build_project
    
    # Run tests
    run_tests
    
    # Install (optional)
    if [[ "${1:-}" == "--install" ]]; then
        install_project
    fi
    
    # Create package (optional)
    if [[ "${1:-}" == "--package" ]]; then
        create_package
    fi
    
    # Return to original directory
    cd "$original_dir"
    
    log "INFO" "Build process completed successfully!"
    log "INFO" "Executable location: build/NeoZorKDEXArb"
    
    # Show usage
    echo
    echo -e "${CYAN}Usage examples:${NC}"
    echo -e "  ${GREEN}./build-modern.sh${NC}          # Build only"
    echo -e "  ${GREEN}./build-modern.sh --install${NC} # Build and install"
    echo -e "  ${GREEN}./build-modern.sh --package${NC} # Build and create package"
    echo
}

# Run main function with all arguments
main "$@"
