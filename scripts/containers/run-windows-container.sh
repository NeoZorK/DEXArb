#!/bin/bash

# Windows Container Runner for NeoZorKDEXArb
# Runs the application in Windows container with MSVC
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

# Build for Windows if not exists
build_windows() {
    local build_dir="build-windows"
    
    if [ ! -f "$build_dir/bin/NeoZorKDEXArb.exe" ]; then
        log "INFO" "Windows build not found. Building application..."
        
        # Use Apple Container with Windows Server Core
        container run --rm -v "$(pwd):/workspace" -w /workspace \
            mcr.microsoft.com/windows/servercore:ltsc2022 cmd /c "
                echo Installing build tools...
                
                # Install Chocolatey package manager
                Set-ExecutionPolicy Bypass -Scope Process -Force
                [System.Net.ServicePointManager]::SecurityProtocol = [System.Net.ServicePointManager]::SecurityProtocol -bor 3072
                iex ((New-Object System.Net.WebClient).DownloadString('https://community.chocolatey.org/install.ps1'))
                
                # Install required packages
                choco install -y cmake ninja visualstudio2022buildtools
                choco install -y curl
                
                # Refresh environment variables
                refreshenv
                
                # Install vcpkg
                if (!(Test-Path C:\vcpkg)) {
                    git clone https://github.com/Microsoft/vcpkg.git C:\vcpkg
                    C:\vcpkg\bootstrap-vcpkg.bat
                }
                
                # Install dependencies
                C:\vcpkg\vcpkg install curl:x64-windows nlohmann-json:x64-windows
                C:\vcpkg\vcpkg integrate install
                
                # Set environment variables
                set VCPKG_ROOT=C:\vcpkg
                set CMAKE_TOOLCHAIN_FILE=C:\vcpkg\scripts\buildsystems\vcpkg.cmake
                
                # Build application
                cd /workspace
                if (!(Test-Path $build_dir)) { mkdir $build_dir }
                cd $build_dir
                
                cmake .. -G \"Ninja\" ^
                    -DCMAKE_BUILD_TYPE=Release ^
                    -DBUILD_TESTING=OFF ^
                    -DCMAKE_TOOLCHAIN_FILE=%CMAKE_TOOLCHAIN_FILE% ^
                    -DVCPKG_TARGET_TRIPLET=x64-windows
                
                cmake --build . --target NeoZorKDEXArb
            "
        
        log "INFO" "Windows build completed: $build_dir/bin/NeoZorKDEXArb.exe"
    else
        log "INFO" "Windows build found: $build_dir/bin/NeoZorKDEXArb.exe"
    fi
}

# Run application in Windows container
run_windows_container() {
    local build_dir="build-windows"
    local exe_path="$build_dir/bin/NeoZorKDEXArb.exe"
    
    if [ ! -f "$exe_path" ]; then
        log "ERROR" "Executable not found: $exe_path"
        exit 1
    fi
    
    log "INFO" "Starting NeoZorKDEXArb in Windows container..."
    log "INFO" "Executable: $exe_path"
    
    # Run in Windows container with the executable
    container run --rm -v "$(pwd):/workspace" -w /workspace \
        mcr.microsoft.com/windows/servercore:ltsc2022 cmd /c "
            cd /workspace
            echo Running NeoZorKDEXArb...
            echo.
            $exe_path %*
        "
}

# Show help
show_help() {
    echo "Usage: $0 [OPTIONS] [ARGS...]"
    echo
    echo "Options:"
    echo "  --help, -h     Show this help message"
    echo "  --build, -b    Force rebuild the application"
    echo "  --clean        Clean build directory before building"
    echo "  --version      Show version information"
    echo
    echo "Examples:"
    echo "  $0                    # Run application with default settings"
    echo "  $0 --help            # Show help"
    echo "  $0 --build           # Force rebuild and run"
    echo "  $0 --clean --build   # Clean, rebuild and run"
    echo "  $0 --version         # Show version"
    echo
    echo "Arguments after -- are passed to the application:"
    echo "  $0 -- --help         # Pass --help to NeoZorKDEXArb"
    echo "  $0 -- --scan         # Pass --scan to NeoZorKDEXArb"
}

# Show version
show_version() {
    echo "Windows Container Runner for NeoZorKDEXArb"
    echo "Version: 1.0.7"
    echo "Created by Rostyslav S. on 26.02.2025"
    echo "Platform: Windows Server Core (LTSC 2022)"
}

# Main function
main() {
    local force_build=false
    local clean_build=false
    local show_help_flag=false
    local show_version_flag=false
    local app_args=()
    
    # Parse command line arguments
    while [[ $# -gt 0 ]]; do
        case $1 in
            --help|-h)
                show_help_flag=true
                shift
                ;;
            --build|-b)
                force_build=true
                shift
                ;;
            --clean)
                clean_build=true
                shift
                ;;
            --version)
                show_version_flag=true
                shift
                ;;
            --)
                shift
                app_args+=("$@")
                break
                ;;
            -*)
                log "WARN" "Unknown option: $1"
                shift
                ;;
            *)
                app_args+=("$1")
                shift
                ;;
        esac
    done
    
    # Show help or version if requested
    if [ "$show_help_flag" = true ]; then
        show_help
        exit 0
    fi
    
    if [ "$show_version_flag" = true ]; then
        show_version
        exit 0
    fi
    
    # Check prerequisites
    check_container_cli
    
    # Clean build directory if requested
    if [ "$clean_build" = true ]; then
        log "INFO" "Cleaning build directory..."
        rm -rf build-windows
        force_build=true
    fi
    
    # Build if needed
    if [ "$force_build" = true ] || [ ! -f "build-windows/bin/NeoZorKDEXArb.exe" ]; then
        build_windows
    fi
    
    # Run application
    run_windows_container "${app_args[@]}"
}

# Run main function
main "$@"
