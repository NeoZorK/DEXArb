#!/bin/bash

# Multi-Platform Build Script for NeoZorKDEXArb using Apple Container CLI
# Supports: macOS, Linux Alpine (musl), Linux Ubuntu (glibc), Windows
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

# Build for macOS (native)
build_macos() {
    local build_dir="build-macos"
    log "INFO" "Building for macOS (native)..."
    
    mkdir -p "$build_dir"
    cd "$build_dir"
    
    cmake .. \
        -DCMAKE_TOOLCHAIN_FILE=../vcpkg/scripts/buildsystems/vcpkg.cmake \
        -DVCPKG_TARGET_TRIPLET=x64-osx \
        -DCMAKE_BUILD_TYPE=Release \
        -DCMAKE_CXX_COMPILER=clang++
    
    make -j$(sysctl -n hw.ncpu 2>/dev/null || echo "4")
    cd ..
    
    log "INFO" "macOS build completed: $build_dir/bin/NeoZorKDEXArb"
}

# Build for Linux Alpine (musl)
build_alpine() {
    local build_dir="build-alpine"
    log "INFO" "Building for Linux Alpine (musl)..."
    
    # Use Apple Container with Alpine
    container run --rm -v "$(pwd):/workspace" -w /workspace \
        alpine:latest sh -c "
            apk add --no-cache \
                alpine-sdk \
                cmake \
                g++ \
                curl-dev \
                nlohmann-json-dev \
                gtest-dev \
                git
            
            cd /workspace
            mkdir -p $build_dir
            cd $build_dir
            
            cmake .. \
                -DCMAKE_TOOLCHAIN_FILE=../vcpkg/scripts/buildsystems/vcpkg.cmake \
                -DVCPKG_TARGET_TRIPLET=x64-linux-musl \
                -DCMAKE_BUILD_TYPE=Release
            
            make -j$(nproc 2>/dev/null || echo "4")
        "
    
    log "INFO" "Alpine build completed: $build_dir/bin/NeoZorKDEXArb"
}

# Build for Linux Ubuntu (glibc)
build_ubuntu() {
    local build_dir="build-ubuntu"
    log "INFO" "Building for Linux Ubuntu (glibc)..."
    
    # Use Apple Container with Ubuntu
    container run --rm -v "$(pwd):/workspace" -w /workspace \
        ubuntu:22.04 sh -c "
            apt-get update && apt-get install -y \
                build-essential \
                cmake \
                g++ \
                libcurl4-openssl-dev \
                nlohmann-json3-dev \
                libgtest-dev \
                git
            
            cd /workspace
            mkdir -p $build_dir
            cd $build_dir
            
            cmake .. \
                -DCMAKE_TOOLCHAIN_FILE=../vcpkg/scripts/buildsystems/vcpkg.cmake \
                -DVCPKG_TARGET_TRIPLET=x64-linux-ubuntu \
                -DCMAKE_BUILD_TYPE=Release
            
            make -j$(nproc 2>/dev/null || echo "4")
        "
    
    log "INFO" "Ubuntu build completed: $build_dir/bin/NeoZorKDEXArb"
}

# Build for Windows (cross-compilation)
build_windows() {
    local build_dir="build-windows"
    log "INFO" "Building for Windows (cross-compilation)..."
    
    # Use Apple Container with Ubuntu for cross-compilation
    container run --rm -v "$(pwd):/workspace" -w /workspace \
        ubuntu:22.04 sh -c "
            apt-get update && apt-get install -y \
                build-essential \
                cmake \
                g++ \
                gcc-mingw-w64 \
                g++-mingw-w64 \
                libcurl4-openssl-dev \
                nlohmann-json3-dev \
                libgtest-dev \
                git
            
            cd /workspace
            mkdir -p $build_dir
            cd $build_dir
            
            cmake .. \
                -DCMAKE_TOOLCHAIN_FILE=../vcpkg/scripts/buildsystems/vcpkg.cmake \
                -DVCPKG_TARGET_TRIPLET=x64-windows-msvc \
                -DCMAKE_BUILD_TYPE=Release \
                -DCMAKE_SYSTEM_NAME=Windows \
                -DCMAKE_C_COMPILER=x86_64-w64-mingw32-gcc \
                -DCMAKE_CXX_COMPILER=x86_64-w64-mingw32-g++
            
            make -j$(nproc 2>/dev/null || echo "4")
        "
    
    log "INFO" "Windows build completed: $build_dir/bin/NeoZorKDEXArb.exe"
}

# Run tests for a specific platform
run_tests() {
    local platform=$1
    local build_dir="build-$platform"
    
    log "INFO" "Running tests for $platform..."
    
    if [ ! -d "$build_dir" ]; then
        log "WARN" "Build directory not found: $build_dir"
        return 1
    fi
    
    cd "$build_dir"
    
    # Run available tests
    for test_exe in ModernResultTests ModernFormatTests AllFlagsAndResultsTests; do
        if [ -f "$test_exe" ]; then
            log "INFO" "Running $test_exe..."
            ./"$test_exe" || log "WARN" "Test $test_exe failed"
        fi
    done
    
    cd ..
}

# Create distribution packages
create_packages() {
    log "INFO" "Creating distribution packages..."
    
    local dist_dir="dist"
    mkdir -p "$dist_dir"
    
    # macOS package
    if [ -f "build-macos/bin/NeoZorKDEXArb" ]; then
        mkdir -p "$dist_dir/macos"
        cp "build-macos/bin/NeoZorKDEXArb" "$dist_dir/macos/"
        log "INFO" "macOS package created: $dist_dir/macos/"
    fi
    
    # Alpine package
    if [ -f "build-alpine/bin/NeoZorKDEXArb" ]; then
        mkdir -p "$dist_dir/alpine"
        cp "build-alpine/bin/NeoZorKDEXArb" "$dist_dir/alpine/"
        log "INFO" "Alpine package created: $dist_dir/alpine/"
    fi
    
    # Ubuntu package
    if [ -f "build-ubuntu/bin/NeoZorKDEXArb" ]; then
        mkdir -p "$dist_dir/ubuntu"
        cp "build-ubuntu/bin/NeoZorKDEXArb" "$dist_dir/ubuntu/"
        log "INFO" "Ubuntu package created: $dist_dir/ubuntu/"
    fi
    
    # Windows package
    if [ -f "build-windows/bin/NeoZorKDEXArb.exe" ]; then
        mkdir -p "$dist_dir/windows"
        cp "build-windows/bin/NeoZorKDEXArb.exe" "$dist_dir/windows/"
        log "INFO" "Windows package created: $dist_dir/windows/"
    fi
    
    # Create README
    cat > "$dist_dir/README.txt" << EOF
NeoZorKDEXArb v1.0.7 - Multi-Platform Distribution

This package contains compiled binaries for multiple platforms:

- macOS: Native Apple Silicon/Intel build
- Alpine: Linux Alpine musl build
- Ubuntu: Linux Ubuntu glibc build  
- Windows: Cross-compiled Windows executable

Usage:
  ./NeoZorKDEXArb -h          # Show help
  ./NeoZorKDEXArb -v          # Show version
  ./NeoZorKDEXArb -scan ethereum 5000  # Scan Ethereum blockchain

For more information, see the main project documentation.
EOF
    
    log "INFO" "All distribution packages created in: $dist_dir"
}

# Show help
show_help() {
    echo "Usage: $0 [OPTIONS] [PLATFORMS...]"
    echo ""
    echo "Options:"
    echo "  --help, -h          Show this help message"
    echo "  --clean             Clean build directories before building"
    echo "  --test              Run tests after building"
    echo "  --package           Create distribution packages after building"
    echo "  --all               Build all platforms, test, and package"
    echo ""
    echo "Platforms:"
    echo "  macos               Build for macOS (native)"
    echo "  alpine              Build for Linux Alpine (musl)"
    echo "  ubuntu              Build for Linux Ubuntu (glibc)"
    echo "  windows             Build for Windows (cross-compilation)"
    echo "  all                 Build for all platforms"
    echo ""
    echo "Examples:"
    echo "  $0 macos            # Build only for macOS"
    echo "  $0 alpine ubuntu    # Build for Alpine and Ubuntu"
    echo "  $0 --all            # Build all platforms with tests and packages"
    echo "  $0 --clean --test   # Clean, build, and test current platform"
}

# Clean build directories
clean_builds() {
    log "INFO" "Cleaning build directories..."
    for platform in macos alpine ubuntu windows; do
        if [ -d "build-$platform" ]; then
            rm -rf "build-$platform"
            log "INFO" "Cleaned build-$platform"
        fi
    done
}

# Main function
main() {
    local platforms=()
    local clean_build=false
    local run_tests_flag=false
    local create_package_flag=false
    
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
            --test)
                run_tests_flag=true
                shift
                ;;
            --package)
                create_package_flag=true
                shift
                ;;
            --all)
                platforms=("macos" "alpine" "ubuntu" "windows")
                clean_build=true
                run_tests_flag=true
                create_package_flag=true
                shift
                ;;
            macos|alpine|ubuntu|windows)
                platforms+=("$1")
                shift
                ;;
            *)
                log "ERROR" "Unknown option: $1"
                show_help
                exit 1
                ;;
        esac
    done
    
    # Set defaults if no platforms specified
    if [ ${#platforms[@]} -eq 0 ]; then
        platforms=("macos")
    fi
    
    # Check dependencies
    check_container_cli
    
    # Clean if requested
    if [ "$clean_build" = true ]; then
        clean_builds
    fi
    
    # Build for each platform
    for platform in "${platforms[@]}"; do
        case $platform in
            "macos")
                build_macos
                if [ "$run_tests_flag" = true ]; then
                    run_tests "macos"
                fi
                ;;
            "alpine")
                build_alpine
                if [ "$run_tests_flag" = true ]; then
                    run_tests "alpine"
                fi
                ;;
            "ubuntu")
                build_ubuntu
                if [ "$run_tests_flag" = true ]; then
                    run_tests "ubuntu"
                fi
                ;;
            "windows")
                build_windows
                if [ "$run_tests_flag" = true ]; then
                    run_tests "windows"
                fi
                ;;
        esac
    done
    
    # Create packages if requested
    if [ "$create_package_flag" = true ]; then
        create_packages
    fi
    
    log "INFO" "Multi-platform build process completed successfully!"
    
    # Show results
    echo ""
    echo "Build results:"
    for platform in "${platforms[@]}"; do
        local build_dir="build-$platform"
        local exe_name="NeoZorKDEXArb"
        if [ "$platform" = "windows" ]; then
            exe_name="NeoZorKDEXArb.exe"
        fi
        
        if [ -f "$build_dir/bin/$exe_name" ]; then
            echo "  ✅ $platform: $build_dir/bin/$exe_name"
        else
            echo "  ❌ $platform: Build failed"
        fi
    done
}

# Run main function with all arguments
main "$@"
