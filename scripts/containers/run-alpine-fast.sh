#!/bin/bash

# Fast Alpine Linux Container Runner for NeoZorKDEXArb
# Runs the application in Alpine Linux container with musl
# Uses cached container for faster startup
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

# Create or use cached Alpine container with dependencies
setup_cached_container() {
    local container_name="neozork-alpine-cache"
    
    # Always create a fresh container for now
    log "INFO" "Creating Alpine container with dependencies..."
    
    # Stop and remove existing container if exists
    container stop "$container_name" 2>/dev/null || true
    container rm "$container_name" 2>/dev/null || true
    
            container run -d --name "$container_name" \
            -v "$(pwd):/workspace" \
            -w /workspace \
            alpine:latest sh -c "
                apk add --no-cache \
                    libstdc++ \
                    libgcc \
                    libcurl \
                    ca-certificates \
                    libcares \
                    nghttp2-libs \
                    libidn2 \
                    libpsl \
                    openssl \
                    zstd-libs \
                    brotli-libs \
                    zlib
                
                # Keep container running
                tail -f /dev/null
            "
    
    log "INFO" "Container created: $container_name"
}

# Run application in cached Alpine container
run_in_cached_container() {
    local build_dir="build-alpine"
    local exe_path="$build_dir/bin/NeoZorKDEXArb"
    local container_name="neozork-alpine-cache"
    
    if [ ! -f "$exe_path" ]; then
        log "ERROR" "Executable not found: $exe_path"
        exit 1
    fi
    
    log "INFO" "Starting NeoZorKDEXArb in cached Alpine Linux container..."
    log "INFO" "Executable: $exe_path"
    
    # Execute in cached container
    container exec -it "$container_name" sh -c "
        cd /workspace
        
        echo '=== Running NeoZorKDEXArb in Alpine Linux (musl) ==='
        echo 'Platform: \$(uname -a)'
        echo 'Libc: \$(ldd $exe_path | head -1)'
        echo '====================================================='
        
        # Execute with any provided arguments
        $exe_path $*
    "
}

# Clean up cached container
cleanup_container() {
    local container_name="neozork-alpine-cache"
    
    log "INFO" "Cleaning up container: $container_name"
    container stop "$container_name" 2>/dev/null || true
    container rm "$container_name" 2>/dev/null || true
    log "INFO" "Container removed"
}

# Show help
show_help() {
    cat << EOF
Usage: $0 [OPTIONS] [--] [APP_ARGS...]

Options:
    -h, --help          Show this help message
    -c, --cleanup       Clean up cached container
    -v, --verbose       Enable verbose output
    --                  Pass remaining arguments to the application

Examples:
    $0                    # Run with default settings
    $0 --help            # Show this help
    $0 --cleanup         # Clean up cached container
    $0 -- --scan         # Run with --scan argument
    $0 -- --help         # Show application help

EOF
}

# Main function
main() {
    local cleanup=false
    local verbose=false
    local app_args=()
    
    # Parse command line arguments
    while [[ $# -gt 0 ]]; do
        case $1 in
            -h|--help)
                show_help
                exit 0
                ;;
            -c|--cleanup)
                cleanup=true
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
    
    # Handle cleanup
    if [ "$cleanup" = true ]; then
        cleanup_container
        exit 0
    fi
    
    # Setup cached container
    setup_cached_container
    
    # Run in container
    if [ ${#app_args[@]} -gt 0 ]; then
        run_in_cached_container "${app_args[@]}"
    else
        run_in_cached_container
    fi
}

# Run main function with all arguments
main "$@"
