#!/bin/bash

# Simple Alpine Linux Container Runner for NeoZorKDEXArb
# Runs the application in Alpine Linux container with musl
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

# Build for Alpine if not exists (main app only, no tests)
build_alpine_main_only() {
    local build_dir="build-alpine"
    
    if [ ! -f "$build_dir/bin/NeoZorKDEXArb" ]; then
        log "INFO" "Alpine build not found. Building main application only..."
        
        # Use Apple Container with Alpine
        container run --rm -v "$(pwd):/workspace" -w /workspace \
            alpine:latest sh -c "
                apk add --no-cache \
                    alpine-sdk \
                    cmake \
                    g++ \
                    curl-dev \
                    nlohmann-json \
                    git
                
                cd /workspace
                mkdir -p $build_dir
                cd $build_dir
                
                cmake .. \
                    -DCMAKE_BUILD_TYPE=Release \
                    -DBUILD_TESTING=OFF
                
                make -j$(nproc 2>/dev/null || echo "4") NeoZorKDEXArb
            "
        
        log "INFO" "Alpine build completed: $build_dir/bin/NeoZorKDEXArb"
    else
        log "INFO" "Alpine build found: $build_dir/bin/NeoZorKDEXArb"
    fi
}

# Run application in Alpine container
run_alpine_container() {
    local build_dir="build-alpine"
    local exe_path="$build_dir/bin/NeoZorKDEXArb"
    
    if [ ! -f "$exe_path" ]; then
        log "ERROR" "Executable not found: $exe_path"
        exit 1
    fi
    
    log "INFO" "Starting NeoZorKDEXArb in Alpine Linux container..."
    log "INFO" "Executable: $exe_path"
    
    # Run in Alpine container with necessary dependencies
    container run --rm -it \
        -v "$(pwd):/workspace" \
        -w /workspace \
        alpine:latest sh -c "
            # Install runtime dependencies
            apk add --no-cache \
                libstdc++ \
                libgcc \
                libcurl \
                ca-certificates
            
            # Set working directory
            cd /workspace
            
            # Run the application
            echo '=== Running NeoZorKDEXArb in Alpine Linux (musl) ==='
            echo 'Platform: \$(uname -a)'
            echo 'Libc: \$(ldd $exe_path | head -1)'
            echo '====================================================='
            
            # Execute with any provided arguments
            $exe_path $*
        "
}

# Show help
show_help() {
    cat << EOF
Usage: $0 [OPTIONS] [--] [APP_ARGS...]

Options:
    -h, --help          Show this help message
    -b, --build         Force rebuild Alpine binary
    -v, --verbose       Enable verbose output
    --                  Pass remaining arguments to the application

Examples:
    $0                    # Run with default settings
    $0 --help            # Show this help
    $0 --build           # Force rebuild and run
    $0 -- --scan         # Run with --scan argument
    $0 -- --help         # Show application help

EOF
}

# Main function
main() {
    local force_build=false
    local verbose=false
    local app_args=()
    
    # Parse command line arguments
    while [[ $# -gt 0 ]]; do
        case $1 in
            -h|--help)
                show_help
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
                app_args+=("$@")
                break
                ;;
            -*)
                log "ERROR" "Unknown option: $1"
                show_help
                exit 1
                ;;
            *)
                app_args+=("$1")
                shift
                ;;
        esac
    done
    
    # Set verbose mode
    if [ "$verbose" = true ]; then
        set -x
    fi
    
    # Check dependencies
    check_container_cli
    
    # Build if needed or forced
    if [ "$force_build" = true ]; then
        log "INFO" "Force rebuild requested..."
        rm -rf build-alpine
    fi
    
    build_alpine_main_only
    
    # Run in container
    if [ ${#app_args[@]} -gt 0 ]; then
        run_alpine_container "${app_args[@]}"
    else
        run_alpine_container
    fi
}

# Run main function with all arguments
main "$@"
