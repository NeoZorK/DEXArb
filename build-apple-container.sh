#!/bin/bash

# Modern Build Script for NeoZorKDEXArb using Apple Container CLI
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
    
    # Check for Apple Container CLI
    if ! command -v container &> /dev/null; then
        log "ERROR" "Apple Container CLI not found. Please install it from Xcode or Command Line Tools."
        log "INFO" "Install Command Line Tools: xcode-select --install"
        exit 1
    fi
    
    # Check Apple Container CLI version
    local container_version=$(container version 2>/dev/null | head -n1 || echo "unknown")
    log "INFO" "Apple Container CLI version: $container_version"
    
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
    if ! command -v clang++ &> /dev/null; then
        log "ERROR" "clang++ not found. Please install Xcode Command Line Tools."
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
    log "INFO" "Installing required packages via vcpkg..."
    ./vcpkg install curl nlohmann-json
    
    # Set CMAKE_TOOLCHAIN_FILE
    export CMAKE_TOOLCHAIN_FILE="$(pwd)/scripts/buildsystems/vcpkg.cmake"
    
    cd ..
    
    log "INFO" "vcpkg setup completed"
}

# Function to create Apple Container
create_container() {
    log "INFO" "Creating Apple Container for build environment..."
    
    # Create container configuration
    cat > Containerfile << 'EOF'
FROM swift:5.9-focal

# Install system dependencies
RUN apt-get update && apt-get install -y \
    build-essential \
    cmake \
    git \
    curl \
    libcurl4-openssl-dev \
    pkg-config \
    && rm -rf /var/lib/apt/lists/*

# Set working directory
WORKDIR /workspace

# Copy source code
COPY . .

# Install vcpkg
RUN git clone https://github.com/Microsoft/vcpkg.git && \
    cd vcpkg && \
    ./bootstrap-vcpkg.sh

# Install dependencies
RUN cd vcpkg && \
    ./vcpkg install curl nlohmann-json

# Build the project
RUN mkdir build && cd build && \
    cmake .. -DCMAKE_TOOLCHAIN_FILE=../vcpkg/scripts/buildsystems/vcpkg.cmake && \
    make -j$(nproc)

# Set entrypoint
ENTRYPOINT ["/bin/bash"]
EOF
    
    log "INFO" "Containerfile created"
}

# Function to build with Apple Container
build_with_container() {
    log "INFO" "Building with Apple Container..."
    
    # Create container
    if ! container build -t neozork-dexarb .; then
        log "ERROR" "Container build failed"
        exit 1
    fi
    
    # Run container to build project
    if ! container run --rm -v "$(pwd):/workspace" neozork-dexarb bash -c "
        cd /workspace && \
        mkdir -p build && cd build && \
        cmake .. -DCMAKE_TOOLCHAIN_FILE=../vcpkg/scripts/buildsystems/vcpkg.cmake && \
        make -j$(nproc)
    "; then
        log "ERROR" "Build in container failed"
        exit 1
    fi
    
    log "INFO" "Build completed successfully in container"
}

# Function to build locally with vcpkg
build_locally() {
    log "INFO" "Building locally with vcpkg..."
    
    # Create build directory
    if [ -d "build" ]; then
        log "INFO" "Cleaning existing build directory..."
        rm -rf build
    fi
    
    mkdir -p build
    cd build
    
    # Configure with CMake
    if ! cmake .. -DCMAKE_TOOLCHAIN_FILE=../vcpkg/scripts/buildsystems/vcpkg.cmake; then
        log "ERROR" "CMake configuration failed"
        exit 1
    fi
    
    # Build
    local cores=$(sysctl -n hw.ncpu 2>/dev/null || echo 4)
    log "INFO" "Using $cores CPU cores for parallel build"
    
    if ! make -j$cores; then
        log "ERROR" "Build failed"
        exit 1
    fi
    
    cd ..
    log "INFO" "Local build completed successfully"
}

# Function to run tests
run_tests() {
    log "INFO" "Running tests..."
    
    if [ -f "build/bin/NeoZorKDEXArb" ]; then
        log "INFO" "Testing executable..."
        
        # Test help command
        if ! ./build/bin/NeoZorKDEXArb -h &> /dev/null; then
            log "ERROR" "Help command test failed"
            exit 1
        fi
        
        # Test version command
        if ! ./build/bin/NeoZorKDEXArb -v &> /dev/null; then
            log "ERROR" "Version command test failed"
            exit 1
        fi
        
        log "INFO" "Basic tests passed"
    else
        log "WARN" "Executable not found, skipping tests"
    fi
    
    # Run Python tests
    if command -v uv &> /dev/null; then
        log "INFO" "Running Python tests with uv..."
        if ! uv run pytest tests -n auto; then
            log "WARN" "Python tests failed"
        fi
    elif command -v pytest &> /dev/null; then
        log "INFO" "Running Python tests with pytest..."
        if ! pytest tests -n auto; then
            log "WARN" "Python tests failed"
        fi
    else
        log "WARN" "Python testing tools not found"
    fi
}

# Function to create deployment package
create_package() {
    log "INFO" "Creating deployment package..."
    
    local package_name="NeoZorKDEXArb-$(date +%Y%m%d-%H%M%S)"
    local package_dir="$package_name"
    
    mkdir -p "$package_dir"
    
    # Copy executable
    if [ -f "build/bin/NeoZorKDEXArb" ]; then
        cp build/bin/NeoZorKDEXArb "$package_dir/"
        log "INFO" "Executable copied to package"
    fi
    
    # Copy documentation
    if [ -d "docs" ]; then
        cp -r docs "$package_dir/"
        log "INFO" "Documentation copied to package"
    fi
    
    # Copy configuration files
    if [ -d "config" ]; then
        cp -r config "$package_dir/"
        log "INFO" "Configuration files copied to package"
    fi
    
    # Create package archive
    tar -czf "${package_name}.tar.gz" "$package_name"
    
    log "INFO" "Deployment package created: ${package_name}.tar.gz"
    
    # Clean up temporary directory
    rm -rf "$package_name"
}

# Function to show usage
show_usage() {
    echo -e "${CYAN}Usage:${NC}"
    echo -e "  ${GREEN}$0${NC} [options]"
    echo
    echo -e "${CYAN}Options:${NC}"
    echo -e "  ${GREEN}--container${NC}     Build using Apple Container (default)"
    echo -e "  ${GREEN}--local${NC}         Build locally with vcpkg"
    echo -e "  ${GREEN}--package${NC}       Create deployment package after build"
    echo -e "  ${GREEN}--help${NC}          Show this help message"
    echo
    echo -e "${CYAN}Examples:${NC}"
    echo -e "  ${GREEN}$0${NC}                    # Build with container"
    echo -e "  ${GREEN}$0 --local${NC}            # Build locally"
    echo -e "  ${GREEN}$0 --container --package${NC} # Build with container and create package"
}

# Main build process
main() {
    local build_method="container"
    local create_package_flag=false
    
    # Parse command line arguments
    while [[ $# -gt 0 ]]; do
        case $1 in
            --container)
                build_method="container"
                shift
                ;;
            --local)
                build_method="local"
                shift
                ;;
            --package)
                create_package_flag=true
                shift
                ;;
            --help|-h)
                show_usage
                exit 0
                ;;
            *)
                log "ERROR" "Unknown option: $1"
                show_usage
                exit 1
                ;;
        esac
    done
    
    log "INFO" "Starting modern build process for NeoZorKDEXArb..."
    log "INFO" "Build method: $build_method"
    
    # Store original directory
    local original_dir=$(pwd)
    
    # Check dependencies
    check_dependencies
    
    # Setup vcpkg
    setup_vcpkg
    
    # Build based on method
    case $build_method in
        "container")
            create_container
            build_with_container
            ;;
        "local")
            build_locally
            ;;
        *)
            log "ERROR" "Unknown build method: $build_method"
            exit 1
            ;;
    esac
    
    # Run tests
    run_tests
    
    # Create package if requested
    if [ "$create_package_flag" = true ]; then
        create_package
    fi
    
    # Return to original directory
    cd "$original_dir"
    
    log "INFO" "Build process completed successfully!"
    log "INFO" "Executable location: build/bin/NeoZorKDEXArb"
    
    # Show usage
    echo
    echo -e "${CYAN}Usage examples:${NC}"
    echo -e "  ${GREEN}$0${NC}                    # Build with container"
    echo -e "  ${GREEN}$0 --local${NC}            # Build locally"
    echo -e "  ${GREEN}$0 --package${NC}          # Build and create package"
    echo
}

# Run main function with all arguments
main "$@"
