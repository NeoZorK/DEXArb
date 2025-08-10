#!/bin/bash

# Windows Wine Container Runner for NeoZorKDEXArb
# Minimal version for basic functionality testing
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

# Configuration
CONTAINER_NAME="dexarb-windows-wine"
IMAGE_NAME="ubuntu:24.04"
WORKSPACE_DIR="/workspace"
BUILD_DIR="build-windows-wine"

# Help function
show_help() {
    cat << EOF
Usage: $0 [OPTIONS]

Options:
    --help, -h          Show this help message
    --version, -v       Show version information
    --create            Create and start container
    --build             Build application in container
    --run               Run application in container
    --shell             Open shell in container
    --stop              Stop container
    --delete            Delete container
    --status            Show container status
    --clean             Clean build artifacts

Examples:
    $0 --create         # Create and start container
    $0 --build          # Build application
    $0 --run            # Run application
    $0 --shell          # Open interactive shell
    $0 --clean          # Clean build artifacts
EOF
}

# Version function
show_version() {
    echo "Windows Wine Container Runner v1.0.0"
    echo "For NeoZorKDEXArb application"
}

# Check if container exists
container_exists() {
    container list | grep -q "$CONTAINER_NAME" 2>/dev/null || return 1
}

# Check if container is running
container_running() {
    container list | grep "$CONTAINER_NAME" | grep -q "Running" 2>/dev/null || return 1
}

# Create and start container
create_container() {
    log "INFO" "Creating container $CONTAINER_NAME..."
    
    if container_exists; then
        log "WARN" "Container $CONTAINER_NAME already exists"
        return 0
    fi
    
    # Create container
    container run "$IMAGE_NAME" \
        --name "$CONTAINER_NAME" \
        --volume "$(pwd):$WORKSPACE_DIR" \
        --detach \
        sleep infinity
    
    log "INFO" "Container created successfully"
    
    # Wait for container to be ready
    sleep 5
    
    # Install basic dependencies
    log "INFO" "Installing basic dependencies..."
    container exec "$CONTAINER_NAME" bash -c "
        apt-get update && apt-get install -y \\
        wine64 wine64-tools \\
        gcc-mingw-w64 g++-mingw-w64 \\
        cmake make git \\
        curl zip unzip tar \\
        pkg-config \\
        libssl-dev \\
        libcurl4-openssl-dev \\
        libjsoncpp-dev \\
        libboost-all-dev \\
        libgtest-dev \\
        libgmock-dev
    "
    
    log "INFO" "Container setup completed"
}

# Build application
build_application() {
    if ! container_exists; then
        log "ERROR" "Container $CONTAINER_NAME does not exist. Run --create first."
        return 1
    fi
    
    if ! container_running; then
        log "INFO" "Starting container..."
        container start "$CONTAINER_NAME"
        sleep 3
    fi
    
    log "INFO" "Building application..."
    
    # Build application using system packages
    container exec "$CONTAINER_NAME" bash -c "
        cd $WORKSPACE_DIR && \\
        mkdir -p $BUILD_DIR && \\
        cd $BUILD_DIR && \\
        cmake .. \\
            -DCMAKE_BUILD_TYPE=Release \\
            -DCMAKE_CXX_FLAGS='-O2 -Wall -Wextra' \\
            -DBUILD_TESTING=OFF && \\
        make -j\$(nproc)
    "
    
    log "INFO" "Build completed successfully"
}

# Run application
run_application() {
    if ! container_exists; then
        log "ERROR" "Container $CONTAINER_NAME does not exist. Run --create first."
        return 1
    fi
    
    if ! container_running; then
        log "INFO" "Starting container..."
        container start "$CONTAINER_NAME"
        sleep 3
    fi
    
    log "INFO" "Running application..."
    
    # Check if executable exists
    container exec "$CONTAINER_NAME" bash -c "
        if [ -f $WORKSPACE_DIR/$BUILD_DIR/bin/NeoZorKDEXArb ]; then
            echo 'Running NeoZorKDEXArb...'
            cd $WORKSPACE_DIR/$BUILD_DIR/bin && \\
            wine NeoZorKDEXArb.exe
        else
            echo 'Executable not found. Build the application first with --build'
            exit 1
        fi
    "
}

# Open shell in container
open_shell() {
    if ! container_exists; then
        log "ERROR" "Container $CONTAINER_NAME does not exist. Run --create first."
        return 1
    fi
    
    if ! container_running; then
        log "INFO" "Starting container..."
        container start "$CONTAINER_NAME"
        sleep 3
    fi
    
    log "INFO" "Opening shell in container..."
    container exec "$CONTAINER_NAME" bash
}

# Stop container
stop_container() {
    if container_exists && container_running; then
        log "INFO" "Stopping container $CONTAINER_NAME..."
        container stop "$CONTAINER_NAME"
        log "INFO" "Container stopped"
    else
        log "WARN" "Container is not running"
    fi
}

# Delete container
delete_container() {
    if container_exists; then
        log "INFO" "Deleting container $CONTAINER_NAME..."
        container delete "$CONTAINER_NAME"
        log "INFO" "Container deleted"
    else
        log "WARN" "Container does not exist"
    fi
}

# Show container status
show_status() {
    if container_exists; then
        log "INFO" "Container status:"
        container list | grep "$CONTAINER_NAME" || true
    else
        log "INFO" "Container $CONTAINER_NAME does not exist"
    fi
}

# Clean build artifacts
clean_build() {
    if container_exists && container_running; then
        log "INFO" "Cleaning build artifacts..."
        container exec "$CONTAINER_NAME" bash -c "
            cd $WORKSPACE_DIR && \\
            rm -rf $BUILD_DIR
        "
        log "INFO" "Build artifacts cleaned"
    else
        log "WARN" "Container is not available for cleaning"
    fi
}

# Main function
main() {
    if [ $# -eq 0 ]; then
        show_help
        exit 1
    fi
    
    case "$1" in
        --help|-h)
            show_help
            ;;
        --version|-v)
            show_version
            ;;
        --create)
            create_container
            ;;
        --build)
            build_application
            ;;
        --run)
            run_application
            ;;
        --shell)
            open_shell
            ;;
        --stop)
            stop_container
            ;;
        --delete)
            delete_container
            ;;
        --status)
            show_status
            ;;
        --clean)
            clean_build
            ;;
        *)
            log "ERROR" "Unknown option: $1"
            show_help
            exit 1
            ;;
    esac
}

# Run main function
main "$@"
