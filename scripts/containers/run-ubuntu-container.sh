#!/bin/bash

# Ubuntu Container Runner for NeoZorKDEXArb
# Runs the application in Ubuntu container with glibc
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

# Build for Ubuntu if not exists
build_ubuntu() {
    local build_dir="build-ubuntu"
    
    if [ ! -f "$build_dir/bin/NeoZorKDEXArb" ]; then
        log "INFO" "Ubuntu build not found. Building application..."
        
        # Use Apple Container with Ubuntu 24.04 (noble)
        container run --rm -v "$(pwd):/workspace" -w /workspace \
            ubuntu:noble sh -c "
                apt-get update
                apt-get install -y \
                    build-essential \
                    cmake \
                    libcurl4-openssl-dev \
                    nlohmann-json3-dev \
                    git \
                    wget \
                    software-properties-common
                
                # Install modern CMake if needed
                cmake_version=\$(cmake --version | head -n1 | grep -o '[0-9]\+\.[0-9]\+' | head -1)
                if [ \"\$(echo \"\$cmake_version < 3.28\" | bc -l 2>/dev/null || echo 'true')\" = 'true' ]; then
                    echo 'Installing modern CMake...'
                    wget -O cmake.sh https://github.com/Kitware/CMake/releases/download/v3.28.1/cmake-3.28.1-linux-aarch64.sh
                    chmod +x cmake.sh
                    ./cmake.sh --skip-license --prefix=/usr/local
                    export PATH=/usr/local/bin:\$PATH
                fi
                
                cd /workspace
                mkdir -p $build_dir
                cd $build_dir
                
                cmake .. \
                    -DCMAKE_BUILD_TYPE=Release \
                    -DBUILD_TESTING=OFF
                
                make -j$(nproc 2>/dev/null || echo "4") NeoZorKDEXArb
            "
        
        log "INFO" "Ubuntu build completed: $build_dir/bin/NeoZorKDEXArb"
    else
        log "INFO" "Ubuntu build found: $build_dir/bin/NeoZorKDEXArb"
    fi
}

# Run application in Ubuntu container
run_ubuntu_container() {
    local build_dir="build-ubuntu"
    local exe_path="$build_dir/bin/NeoZorKDEXArb"
    
    if [ ! -f "$exe_path" ]; then
        log "ERROR" "Executable not found: $exe_path"
        exit 1
    fi
    
    log "INFO" "Starting NeoZorKDEXArb in Ubuntu Linux container..."
    log "INFO" "Executable: $exe_path"
    
    # Run in Ubuntu container with necessary dependencies
    container run --rm -it \
        -v "$(pwd):/workspace" \
        -w /workspace \
        ubuntu:noble sh -c "
            # Install runtime dependencies
            apt-get update
            apt-get install -y \
                libstdc++6 \
                libcurl4 \
                ca-certificates
            
            echo '=== Running NeoZorKDEXArb in Ubuntu Linux (glibc) ==='
            echo 'Platform: \$(uname -a)'
            echo 'Libc: \$(ldd $exe_path | head -1)'
            echo '====================================================='
            
            # Run the application
            $exe_path \"\$@\"
        " "$@"
}

# Main function
main() {
    local force_build=false
    local verbose=false
    
    # Parse command line arguments
    while [[ $# -gt 0 ]]; do
        case $1 in
            -h|--help)
                echo "Usage: $0 [OPTIONS] [--] [APP_ARGS...]"
                echo ""
                echo "Options:"
                echo "  -h, --help     Show this help message"
                echo "  -b, --build    Force rebuild Ubuntu binary"
                echo "  -v, --verbose  Enable verbose output"
                echo "  --             Pass remaining arguments to the application"
                echo ""
                echo "Examples:"
                echo "  $0                    # Run with default help"
                echo "  $0 -- --help         # Show app help"
                echo "  $0 -- -version       # Show app version"
                echo "  $0 -- -scan ethereum 1000  # Scan Ethereum"
                echo "  $0 --build           # Force rebuild"
                exit 0
                ;;
            -b|--build)
                force_build=true
                shift
                ;;
            -v|--verbose)
                verbose=true
                shift
                ;;
            --)
                shift
                break
                ;;
            *)
                echo "Unknown option: $1"
                echo "Use -h or --help for usage information"
                exit 1
                ;;
        esac
    done
    
    # Set verbose mode
    if [ "$verbose" = true ]; then
        set -x
    fi
    
    # Check prerequisites
    check_container_cli
    
    # Build if needed
    if [ "$force_build" = true ] || [ ! -f "build-ubuntu/bin/NeoZorKDEXArb" ]; then
        build_ubuntu
    fi
    
    # Run the application
    run_ubuntu_container "$@"
}

# Run main function with all arguments
main "$@"
