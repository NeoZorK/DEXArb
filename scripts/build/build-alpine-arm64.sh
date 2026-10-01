#!/bin/bash

# Alpine Linux ARM64 Build Script for NeoZorKDEXArb
# Builds the application for Alpine Linux with musl on ARM64
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

# Check Apple Container CLI
check_container_cli() {
    if ! command -v container &> /dev/null; then
        log "ERROR" "Apple Container CLI not found. Please install it first."
        log "INFO" "Install with: brew install --cask docker"
        exit 1
    fi
    log "INFO" "Apple Container CLI found: $(container --version | head -n1)"
}

# Build for Alpine ARM64
build_alpine_arm64() {
    local build_dir="build-alpine"
    
    log "INFO" "Building NeoZorKDEXArb for Alpine Linux ARM64..."
    
    # Use Apple Container with Alpine
    container run --rm -v "$(pwd):/workspace" -w /workspace \
        alpine:latest sh -c "
            # Install build dependencies
            apk add --no-cache \
                alpine-sdk \
                cmake \
                g++ \
                curl-dev \
                nlohmann-json \
                git
            
            # Set environment for ARM64
            export CFLAGS='-march=armv8-a'
            export CXXFLAGS='-march=armv8-a'
            
            cd /workspace
            mkdir -p $build_dir
            cd $build_dir
            
            # Configure with CMake
            cmake .. \
                -DCMAKE_BUILD_TYPE=Release \
                -DBUILD_TESTING=OFF \
                -DGTEST_ROOT='' \
                -DCMAKE_CXX_FLAGS='-march=armv8-a'
            
            # Build
            make -j\$(nproc 2>/dev/null || echo '4') NeoZorKDEXArb
            
            # Verify architecture
            echo '=== Build completed ==='
            echo 'Binary architecture:'
            file bin/NeoZorKDEXArb
            echo 'Dependencies:'
            ldd bin/NeoZorKDEXArb || echo 'Static binary or missing dependencies'
        "
    
    log "INFO" "Alpine ARM64 build completed: $build_dir/bin/NeoZorKDEXArb"
}

# Show help
show_help() {
    cat << EOF
Usage: $0 [OPTIONS]

Options:
    -h, --help          Show this help message
    -v, --verbose       Enable verbose output

Examples:
    $0                    # Build Alpine ARM64 version
    $0 --help            # Show this help
    $0 --verbose         # Build with verbose output

EOF
}

# Main function
main() {
    local verbose=false
    
    # Parse command line arguments
    while [[ $# -gt 0 ]]; do
        case $1 in
            -h|--help)
                show_help
                exit 0
                ;;
            -v|--verbose)
                verbose=true
                shift
                ;;
            -*)
                log "ERROR" "Unknown option: $1"
                show_help
                exit 1
                ;;
            *)
                log "ERROR" "Unexpected argument: $1"
                show_help
                exit 1
                ;;
        esac
    done
    
    # Set verbose mode
    if [ "$verbose" = true ]; then
        set -x
    fi
    
    # Check dependencies
    check_container_cli
    
    # Build
    build_alpine_arm64
    
    log "INFO" "Build completed successfully!"
}

# Run main function with all arguments
main "$@"
