#!/bin/bash

# DeployBins.sh - Build and Deploy Script for NeoZorKDEXArb on macOS
# Created by Rostyslav S. on 26.02.2025
# Updated for macOS compatibility

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
    
    # Check for git
    if ! command -v git &> /dev/null; then
        log "ERROR" "git not found. Please install git."
        exit 1
    fi
    
    # Check for clang++
    if ! command -v clang++ &> /dev/null; then
        log "ERROR" "clang++ not found. Please install Xcode Command Line Tools."
        log "INFO" "Install with: xcode-select --install"
        exit 1
    fi
    
    # Check for make
    if ! command -v make &> /dev/null; then
        log "ERROR" "make not found. Please install Xcode Command Line Tools."
        exit 1
    fi
    
    log "INFO" "All dependencies satisfied"
}

# Function to update repository
update_repository() {
    log "INFO" "Updating repository..."
    
    # Check if we're in a git repository
    if [ ! -d ".git" ]; then
        log "ERROR" "Not in a git repository. Please run this script from the project root."
        exit 1
    fi
    
    # Check for uncommitted changes
    if [ -n "$(git status --porcelain)" ]; then
        log "WARN" "You have uncommitted changes. Please commit or stash them first."
        git status --short
        echo ""
        read -p "Do you want to continue anyway? (y/N): " -n 1 -r
        echo ""
        if [[ ! $REPLY =~ ^[Yy]$ ]]; then
            log "INFO" "Aborted by user"
            exit 0
        fi
    fi
    
    # Pull latest changes
    log "INFO" "Pulling latest changes from remote..."
    if ! git pull; then
        log "ERROR" "Failed to pull latest changes"
        exit 1
    fi
    
    log "INFO" "Repository updated successfully"
}

# Function to build static version
build_static() {
    log "INFO" "Building static version..."
    
    # Create bin directory if it doesn't exist
    mkdir -p bin
    
    # Change to src directory
    cd src
    
    # Clean previous object files
    log "INFO" "Cleaning previous object files..."
    rm -f *.o
    
    # Compile object files
    log "INFO" "Compiling source files..."
    
    # Get list of all .cpp files
    local cpp_files=($(find . -name "*.cpp" -type f))
    
    if [ ${#cpp_files[@]} -eq 0 ]; then
        log "ERROR" "No .cpp files found in src directory"
        exit 1
    fi
    
    # Compile each .cpp file to object file
    for cpp_file in "${cpp_files[@]}"; do
        local obj_file="${cpp_file%.cpp}.o"
        log "INFO" "Compiling $cpp_file -> $obj_file"
        
        if ! clang++ -c "$cpp_file" -o "$obj_file" -std=c++17 -O2 -I../include; then
            log "ERROR" "Failed to compile $cpp_file"
            exit 1
        fi
    done
    
    # Link static executable
    log "INFO" "Linking static executable..."
    local obj_files=($(find . -name "*.o" -type f))
    
    if ! clang++ -static -O2 -s -o dexarb "${obj_files[@]}" -std=c++17 -L../vcpkg_installed/arm64-osx/lib; then
        log "ERROR" "Failed to link static executable"
        exit 1
    fi
    
    # Move to bin directory
    if ! mv dexarb ../bin/dexarb_static; then
        log "ERROR" "Failed to move static executable to bin directory"
        exit 1
    fi
    
    log "INFO" "Static build completed: bin/dexarb_static"
    cd ..
}

# Function to build dynamic version
build_dynamic() {
    log "INFO" "Building dynamic version..."
    
    # Change to src directory
    cd src
    
    # Link dynamic executable
    log "INFO" "Linking dynamic executable..."
    local obj_files=($(find . -name "*.o" -type f))
    
    if ! clang++ -O2 -s -o dexarb "${obj_files[@]}" -std=c++17 -L../vcpkg_installed/arm64-osx/lib; then
        log "ERROR" "Failed to link dynamic executable"
        exit 1
    fi
    
    # Move to bin directory
    if ! mv dexarb ../bin/dexarb_dynamic; then
        log "ERROR" "Failed to move dynamic executable to bin directory"
        exit 1
    fi
    
    log "INFO" "Dynamic build completed: bin/dexarb_dynamic"
    cd ..
}

# Function to build with CMake (alternative method)
build_cmake() {
    log "INFO" "Building with CMake..."
    
    # Create build directory
    mkdir -p build-deploy
    cd build-deploy
    
    # Configure with CMake
    log "INFO" "Configuring with CMake..."
    if ! cmake .. -DCMAKE_BUILD_TYPE=Release; then
        log "ERROR" "CMake configuration failed"
        exit 1
    fi
    
    # Build
    log "INFO" "Building with CMake..."
    local cpu_count=$(sysctl -n hw.ncpu 2>/dev/null || echo "4")
    log "INFO" "Using $cpu_count CPU cores for parallel build"
    
    if ! make -j"$cpu_count"; then
        log "ERROR" "CMake build failed"
        exit 1
    fi
    
    # Copy executables to bin directory
    if [ -f "NeoZorKDEXArb" ]; then
        cp NeoZorKDEXArb ../bin/dexarb_cmake
        log "INFO" "CMake build completed: bin/dexarb_cmake"
    fi
    
    cd ..
}

# Function to test executables
test_executables() {
    log "INFO" "Testing built executables..."
    
    local executables=("dexarb_static" "dexarb_dynamic" "dexarb_cmake")
    local test_passed=0
    
    for exe in "${executables[@]}"; do
        if [ -f "bin/$exe" ]; then
            log "INFO" "Testing $exe..."
            
            # Test help command
            if bin/"$exe" --help &> /dev/null; then
                log "INFO" "✅ $exe help command works"
                ((test_passed++))
            else
                log "WARN" "❌ $exe help command failed"
            fi
            
            # Test version command
            if bin/"$exe" -v &> /dev/null; then
                log "INFO" "✅ $exe version command works"
            else
                log "WARN" "❌ $exe version command failed"
            fi
        else
            log "WARN" "Executable not found: bin/$exe"
        fi
    done
    
    log "INFO" "Testing completed. $test_passed executables passed basic tests."
}

# Function to create deployment package
create_deployment_package() {
    log "INFO" "Creating deployment package..."
    
    local package_name="NeoZorKDEXArb-$(date +%Y%m%d-%H%M%S)"
    local package_dir="$package_name"
    
    # Create package directory
    mkdir -p "$package_dir"
    
    # Copy executables
    if [ -d "bin" ]; then
        cp -r bin "$package_dir/"
        log "INFO" "Executables copied to package"
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
    
    # Copy README
    if [ -f "README.md" ]; then
        cp README.md "$package_dir/"
        log "INFO" "README copied to package"
    fi
    
    # Create package archive
    tar -czf "${package_name}.tar.gz" "$package_name"
    
    log "INFO" "Deployment package created: ${package_name}.tar.gz"
    
    # Clean up temporary directory
    rm -rf "$package_name"
}

# Function to commit and push changes
commit_and_push() {
    log "INFO" "Committing and pushing changes..."
    
    # Check if there are changes to commit
    if [ -z "$(git status --porcelain)" ]; then
        log "INFO" "No changes to commit"
        return 0
    fi
    
    # Add all changes
    git add .
    
    # Commit with timestamp
    local commit_message="Build and deploy $(date '+%Y-%m-%d %H:%M:%S')"
    if ! git commit -m "$commit_message"; then
        log "ERROR" "Failed to commit changes"
        exit 1
    fi
    
    # Push to remote
    log "INFO" "Pushing to remote repository..."
    if ! git push origin main; then
        log "WARN" "Failed to push to main branch, trying master..."
        if ! git push origin master; then
            log "ERROR" "Failed to push to remote repository"
            exit 1
        fi
    fi
    
    log "INFO" "Changes committed and pushed successfully"
}

# Function to show help
show_help() {
    echo "Usage: $0 [OPTIONS]"
    echo ""
    echo "Options:"
    echo "  --help, -h          Show this help message"
    echo "  --static            Build only static version"
    echo "  --dynamic           Build only dynamic version"
    echo "  --cmake             Build only with CMake"
    echo "  --all               Build all versions (default)"
    echo "  --test              Test built executables"
    echo "  --package           Create deployment package"
    echo "  --deploy            Commit and push changes"
    echo "  --skip-update       Skip repository update"
    echo ""
    echo "Examples:"
    echo "  $0                  # Full build and deploy process"
    echo "  $0 --static         # Build only static version"
    echo "  $0 --test           # Build and test executables"
    echo "  $0 --package        # Build and create package"
}

# Main function
main() {
    local build_static_flag=false
    local build_dynamic_flag=false
    local build_cmake_flag=false
    local test_flag=false
    local package_flag=false
    local deploy_flag=false
    local skip_update=false
    
    # Parse command line arguments
    while [[ $# -gt 0 ]]; do
        case $1 in
            --help|-h)
                show_help
                exit 0
                ;;
            --static)
                build_static_flag=true
                shift
                ;;
            --dynamic)
                build_dynamic_flag=true
                shift
                ;;
            --cmake)
                build_cmake_flag=true
                shift
                ;;
            --test)
                test_flag=true
                shift
                ;;
            --package)
                package_flag=true
                shift
                ;;
            --deploy)
                deploy_flag=true
                shift
                ;;
            --skip-update)
                skip_update=true
                shift
                ;;
            --all)
                build_static_flag=true
                build_dynamic_flag=true
                build_cmake_flag=true
                test_flag=true
                package_flag=true
                deploy_flag=true
                shift
                ;;
            *)
                log "ERROR" "Unknown option: $1"
                show_help
                exit 1
                ;;
        esac
    done
    
    # Set defaults if no specific flags
    if [ "$build_static_flag" = false ] && [ "$build_dynamic_flag" = false ] && [ "$build_cmake_flag" = false ]; then
        build_static_flag=true
        build_dynamic_flag=true
        build_cmake_flag=true
        test_flag=true
        package_flag=true
        deploy_flag=true
    fi
    
    log "INFO" "Starting DEXArb build and deploy process..."
    
    # Check dependencies
    check_dependencies
    
    # Update repository if not skipped
    if [ "$skip_update" = false ]; then
        update_repository
    fi
    
    # Build executables
    if [ "$build_static_flag" = true ]; then
        build_static
    fi
    
    if [ "$build_dynamic_flag" = true ]; then
        build_dynamic
    fi
    
    if [ "$build_cmake_flag" = true ]; then
        build_cmake
    fi
    
    # Test executables
    if [ "$test_flag" = true ]; then
        test_executables
    fi
    
    # Create package
    if [ "$package_flag" = true ]; then
        create_deployment_package
    fi
    
    # Deploy changes
    if [ "$deploy_flag" = true ]; then
        commit_and_push
    fi
    
    log "INFO" "Build and deploy process completed successfully!"
    
    # Show results
    echo ""
    echo "Build results:"
    ls -la bin/ 2>/dev/null || echo "No bin directory found"
    
    if [ "$package_flag" = true ]; then
        echo ""
        echo "Package created:"
        ls -la *.tar.gz 2>/dev/null || echo "No package found"
    fi
}

# Run main function with all arguments
main "$@"
