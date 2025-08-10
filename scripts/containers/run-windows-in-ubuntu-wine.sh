#!/bin/bash

# Windows Application Runner in Ubuntu Docker Container with Wine
# Runs Windows applications inside Ubuntu container using Wine
# Created by Rostyslav S. on 26.02.2025

set -euo pipefail

# Colors for output
RED='\033[0;31m'
GREEN='\033[0;32m'
YELLOW='\033[1;33m'
BLUE='\033[0;34m'
CYAN='\033[0;36m'
NC='\033[0m'

# Configuration
CONTAINER_NAME="dexarb-windows-wine"
IMAGE_NAME="ubuntu:24.04"
WORKSPACE_DIR="/workspace"
BUILD_DIR="build-windows-wine"
WINE_PREFIX="/root/.wine"

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

# Help function
show_help() {
    cat << EOF
Usage: $0 [OPTIONS] [--] [APP_ARGS...]

Options:
    --help, -h          Show this help message
    --version, -v       Show version information
    --create            Create and setup container with Wine
    --build             Build Windows application in container
    --run               Run Windows application in container
    --shell             Open interactive shell in container
    --stop              Stop container
    --delete            Delete container
    --status            Show container status
    --clean             Clean build artifacts
    --rebuild           Force rebuild container and application
    --wine-config       Configure Wine settings
    --wine-test         Test Wine installation

Examples:
    $0 --create                    # Create and setup container with Wine
    $0 --build                     # Build Windows application
    $0 --run                       # Run Windows application
    $0 --shell                     # Open interactive shell
    $0 --rebuild                   # Force rebuild everything
    $0 --wine-config               # Configure Wine
    $0 --wine-test                 # Test Wine installation
EOF
}

# Version function
show_version() {
    echo "Windows in Ubuntu Wine Container Runner v1.0.0"
    echo "For running Windows applications in Ubuntu Docker with Wine"
}

# Check if container exists
container_exists() {
    container list | grep -q "$CONTAINER_NAME" 2>/dev/null || return 1
}

# Check if container is running
container_running() {
    container list | grep "$CONTAINER_NAME" | grep -q "Running" 2>/dev/null || return 1
}

# Create and setup container with Wine
create_container() {
    log "INFO" "Creating Ubuntu container with Wine support..."
    
    if container_exists; then
        log "WARN" "Container $CONTAINER_NAME already exists"
        read -p "Do you want to recreate it? (y/N): " -n 1 -r
        echo
        if [[ $REPLY =~ ^[Yy]$ ]]; then
            delete_container
        else
            return 0
        fi
    fi
    
    # Create container
    log "INFO" "Creating container $CONTAINER_NAME..."
    container run \
        --name "$CONTAINER_NAME" \
        --volume "$(pwd):$WORKSPACE_DIR" \
        --detach \
        "$IMAGE_NAME" \
        sleep infinity
    
    # Wait for container to be ready
    sleep 5
    
    # Install Wine and dependencies
    log "INFO" "Installing Wine and dependencies..."
    container exec "$CONTAINER_NAME" bash -c "
        # Update package list
        apt-get update
        
        # Install Wine and Windows development tools
        apt-get install -y \\
            wine64 wine64-tools \\
            wine32 wine32-tools \\
            gcc-mingw-w64 g++-mingw-w64 \\
            cmake make git \\
            curl wget zip unzip tar \\
            pkg-config \\
            libssl-dev \\
            libcurl4-openssl-dev \\
            libjsoncpp-dev \\
            libboost-all-dev \\
            libgtest-dev \\
            libgmock-dev \\
            xvfb \\
            fonts-liberation \\
            ttf-mscorefonts-installer
        
        # Accept EULA for Microsoft fonts
        echo ttf-mscorefonts-installer msttcorefonts/accepted-mscorefonts-eula select true | debconf-set-selections
        
        # Install additional fonts
        apt-get install -y \\
            fonts-wine \\
            fonts-freefont-ttf
        
        # Clean up
        apt-get clean
        rm -rf /var/lib/apt/lists/*
    "
    
    # Configure Wine
    log "INFO" "Configuring Wine..."
    container exec "$CONTAINER_NAME" bash -c "
        # Set Wine prefix
        export WINEPREFIX=$WINE_PREFIX
        export WINEARCH=win64
        
        # Initialize Wine
        wineboot --init
        
        # Wait for Wine to finish initialization
        sleep 10
        
        # Configure Wine for better compatibility
        wine reg add 'HKCU\\\\Software\\\\Wine\\\\DllOverrides' /v 'msvcrt' /t REG_SZ /d 'native,builtin' /f
        wine reg add 'HKCU\\\\Software\\\\Wine\\\\DllOverrides' /v 'msvcp140' /t REG_SZ /d 'native,builtin' /f
        
        # Install Visual C++ Redistributable if needed
        echo 'Wine configuration completed'
    "
    
    log "INFO" "Container setup completed successfully"
}

# Configure Wine settings
configure_wine() {
    if ! container_exists; then
        log "ERROR" "Container $CONTAINER_NAME does not exist. Run --create first."
        return 1
    fi
    
    if ! container_running; then
        log "INFO" "Starting container..."
        container start "$CONTAINER_NAME"
        sleep 3
    fi
    
    log "INFO" "Configuring Wine settings..."
    
    container exec "$CONTAINER_NAME" bash -c "
        export WINEPREFIX=$WINE_PREFIX
        export WINEARCH=win64
        
        # Open Wine configuration
        echo 'Opening Wine configuration...'
        echo 'You can configure Wine settings here'
        
        # Set common Wine settings for better compatibility
        wine reg add 'HKCU\\\\Software\\\\Wine\\\\DllOverrides' /v 'msvcrt' /t REG_SZ /d 'native,builtin' /f
        wine reg add 'HKCU\\\\Software\\\\Wine\\\\DllOverrides' /v 'msvcp140' /t REG_SZ /d 'native,builtin' /f
        wine reg add 'HKCU\\\\Software\\\\Wine\\\\DllOverrides' /v 'vcruntime140' /t REG_SZ /d 'native,builtin' /f
        
        echo 'Wine configuration updated'
    "
}

# Test Wine installation
test_wine() {
    if ! container_exists; then
        log "ERROR" "Container $CONTAINER_NAME does not exist. Run --create first."
        return 1
    fi
    
    if ! container_running; then
        log "INFO" "Starting container..."
        container start "$CONTAINER_NAME"
        sleep 3
    fi
    
    log "INFO" "Testing Wine installation..."
    
    container exec "$CONTAINER_NAME" bash -c "
        export WINEPREFIX=$WINE_PREFIX
        export WINEARCH=win64
        
        echo '=== Wine Test Results ==='
        echo 'Wine version:'
        wine --version
        
        echo ''
        echo 'Wine architecture:'
        wine winecfg /v
        
        echo ''
        echo 'Testing basic Windows commands:'
        wine cmd /c 'echo Hello from Windows!'
        wine cmd /c 'ver'
        
        echo ''
        echo 'Wine prefix location: $WINE_PREFIX'
        echo 'Wine prefix contents:'
        ls -la $WINE_PREFIX/drive_c/windows/system32/ | head -10
        
        echo '=== End Wine Test ==='
    "
}

# Build Windows application
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
    
    log "INFO" "Building Windows application..."
    
    # Build application using MinGW cross-compiler
    container exec "$CONTAINER_NAME" bash -c "
        cd $WORKSPACE_DIR
        
        # Create build directory
        mkdir -p $BUILD_DIR
        cd $BUILD_DIR
        
        # Configure with CMake for Windows target
        cmake .. \\
            -DCMAKE_BUILD_TYPE=Release \\
            -DCMAKE_SYSTEM_NAME=Windows \\
            -DCMAKE_C_COMPILER=x86_64-w64-mingw32-gcc \\
            -DCMAKE_CXX_COMPILER=x86_64-w64-mingw32-g++ \\
            -DCMAKE_RC_COMPILER=x86_64-w64-mingw32-windres \\
            -DCMAKE_FIND_ROOT_PATH=/usr/x86_64-w64-mingw32 \\
            -DCMAKE_FIND_ROOT_PATH_MODE_PROGRAM=NEVER \\
            -DCMAKE_FIND_ROOT_PATH_MODE_LIBRARY=ONLY \\
            -DCMAKE_FIND_ROOT_PATH_MODE_INCLUDE=ONLY \\
            -DBUILD_TESTING=OFF
        
        # Build the application
        make -j\$(nproc 2>/dev/null || echo "4")
        
        echo 'Build completed successfully'
        echo 'Executable location: $BUILD_DIR/bin/NeoZorKDEXArb.exe'
    "
    
    log "INFO" "Build completed successfully"
}

# Run Windows application
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
    
    log "INFO" "Running Windows application..."
    
    # Check if executable exists and run it
    container exec "$CONTAINER_NAME" bash -c "
        cd $WORKSPACE_DIR
        
        if [ -f $BUILD_DIR/bin/NeoZorKDEXArb.exe ]; then
            echo '=== Running NeoZorKDEXArb in Windows (Wine) ==='
            echo 'Platform: \$(uname -a)'
            echo 'Wine version: \$(wine --version)'
            echo 'Executable: $BUILD_DIR/bin/NeoZorKDEXArb.exe'
            echo '==============================================='
            
            # Set Wine environment
            export WINEPREFIX=$WINE_PREFIX
            export WINEARCH=win64
            export DISPLAY=:99
            
            # Start virtual display
            Xvfb :99 -screen 0 1024x768x24 > /dev/null 2>&1 &
            sleep 2
            
            # Run the application with arguments
            cd $BUILD_DIR/bin
            wine NeoZorKDEXArb.exe \"\$@\"
        else
            echo 'Executable not found: $BUILD_DIR/bin/NeoZorKDEXArb.exe'
            echo 'Build the application first with --build'
            exit 1
        fi
    " "$@"
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

# Force rebuild everything
rebuild_everything() {
    log "INFO" "Force rebuilding everything..."
    delete_container
    create_container
    build_application
    log "INFO" "Rebuild completed successfully"
}

# Main function
main() {
    # Check prerequisites first
    check_container_cli
    
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
            shift
            run_application "$@"
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
        --rebuild)
            rebuild_everything
            ;;
        --wine-config)
            configure_wine
            ;;
        --wine-test)
            test_wine
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
