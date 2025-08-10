#!/bin/bash

# NeoZorKDEXArb Build and Deploy Script
# This script builds the project, commits changes, and pushes to repository

set -e  # Exit on any error

# Colors for output
RED='\033[0;31m'
GREEN='\033[0;32m'
YELLOW='\033[1;33m'
BLUE='\033[0;34m'
NC='\033[0m' # No Color

# Function to print colored output
print_status() {
    echo -e "${BLUE}[INFO]${NC} $1"
}

print_success() {
    echo -e "${GREEN}[SUCCESS]${NC} $1"
}

print_warning() {
    echo -e "${YELLOW}[WARNING]${NC} $1"
}

print_error() {
    echo -e "${RED}[ERROR]${NC} $1"
}

# Function to check if command exists
command_exists() {
    command -v "$1" >/dev/null 2>&1
}

# Function to detect OS
detect_os() {
    case "$(uname -s)" in
        Linux*)     echo "linux";;
        Darwin*)    echo "macos";;
        CYGWIN*)    echo "windows";;
        MINGW*)     echo "windows";;
        *)          echo "unknown";;
    esac
}

# Function to check dependencies
check_dependencies() {
    print_status "Checking dependencies..."
    
    local missing_deps=()
    
    if ! command_exists cmake; then
        missing_deps+=("cmake")
    fi
    
    if ! command_exists git; then
        missing_deps+=("git")
    fi
    
    # Check for C++ compiler
    if ! command_exists g++ && ! command_exists clang++ && ! command_exists cl; then
        missing_deps+=("C++ compiler (g++, clang++, or cl)")
    fi
    
    if [ ${#missing_deps[@]} -ne 0 ]; then
        print_error "Missing dependencies: ${missing_deps[*]}"
        print_status "Please install missing dependencies:"
        
        local os=$(detect_os)
        case $os in
            "macos")
                print_status "macOS: brew install cmake curl"
                ;;
            "linux")
                print_status "Ubuntu/Debian: sudo apt install build-essential cmake libcurl4-openssl-dev git"
                print_status "CentOS/RHEL: sudo yum install gcc-c++ cmake libcurl-devel git"
                ;;
            "windows")
                print_status "Windows: Install Visual Studio with C++ support and CMake"
                ;;
        esac
        exit 1
    fi
    
    print_success "All dependencies found"
}

# Function to get current git branch
get_current_branch() {
    git branch --show-current 2>/dev/null || echo "main"
}

# Function to check git status
check_git_status() {
    print_status "Checking git status..."
    
    if ! git rev-parse --git-dir > /dev/null 2>&1; then
        print_error "Not a git repository"
        exit 1
    fi
    
    local current_branch=$(get_current_branch)
    print_status "Current branch: $current_branch"
    
    # Check for uncommitted changes
    if ! git diff-index --quiet HEAD --; then
        print_warning "You have uncommitted changes"
        read -p "Do you want to commit them before building? (y/N): " -n 1 -r
        echo
        if [[ $REPLY =~ ^[Yy]$ ]]; then
            read -p "Enter commit message: " commit_msg
            if [ -z "$commit_msg" ]; then
                commit_msg="Auto-commit before build"
            fi
            git add .
            git commit -m "$commit_msg"
            print_success "Changes committed"
        fi
    fi
    
    # Pull latest changes
    print_status "Pulling latest changes..."
    git pull origin "$current_branch" || print_warning "Could not pull latest changes"
}

# Function to build project
build_project() {
    print_status "Building project..."
    
    # Create build directory if it doesn't exist
    if [ ! -d "build" ]; then
        mkdir build
        print_status "Created build directory"
    fi
    
    cd build
    
    # Configure with CMake
    print_status "Configuring with CMake..."
    if ! cmake ..; then
        print_error "CMake configuration failed"
        exit 1
    fi
    
    # Build project
    print_status "Building project..."
    local build_type="Release"
    if [ "$1" = "debug" ]; then
        build_type="Debug"
    fi
    
    if ! cmake --build . --config "$build_type"; then
        print_error "Build failed"
        exit 1
    fi
    
    # Copy binary to bin directory
    if [ -f "NeoZorKDEXArb" ]; then
        cp NeoZorKDEXArb ../bin/
        print_success "Binary copied to bin/ directory"
    elif [ -f "NeoZorKDEXArb.exe" ]; then
        cp NeoZorKDEXArb.exe ../bin/
        print_success "Binary copied to bin/ directory"
    else
        print_warning "Binary not found in build directory"
    fi
    
    cd ..
}

# Function to run tests (placeholder)
run_tests() {
    print_status "Running tests..."
    # Add test execution here when tests are implemented
    print_warning "Tests not implemented yet"
}

# Function to commit and push
commit_and_push() {
    local current_branch=$(get_current_branch)
    
    print_status "Preparing to commit and push..."
    
    # Check if there are changes to commit
    if git diff-index --quiet HEAD --; then
        print_warning "No changes to commit"
        return 0
    fi
    
    # Add all changes
    git add .
    
    # Get commit message
    local commit_msg="$1"
    if [ -z "$commit_msg" ]; then
        commit_msg="Auto-build: $(date '+%Y-%m-%d %H:%M:%S')"
    fi
    
    # Commit changes
    if git commit -m "$commit_msg"; then
        print_success "Changes committed"
    else
        print_error "Commit failed"
        return 1
    fi
    
    # Push to remote
    print_status "Pushing to remote repository..."
    if git push origin "$current_branch"; then
        print_success "Successfully pushed to $current_branch"
    else
        print_error "Push failed"
        return 1
    fi
}

# Function to show usage
show_usage() {
    echo "Usage: $0 [OPTIONS] [COMMIT_MESSAGE]"
    echo ""
    echo "Options:"
    echo "  -h, --help          Show this help message"
    echo "  -d, --debug         Build in debug mode"
    echo "  -t, --test          Run tests after build"
    echo "  -c, --commit        Commit and push changes"
    echo "  -a, --all           Build, test, commit and push (default)"
    echo ""
    echo "Examples:"
    echo "  $0                           # Build, test, commit and push"
    echo "  $0 -d                        # Build in debug mode"
    echo "  $0 -c \"Update version 1.0.5\"  # Build and commit with message"
    echo "  $0 --test                    # Build and run tests only"
}

# Main script
main() {
    local build_type="release"
    local run_tests_flag=false
    local commit_flag=true
    local commit_msg=""
    
    # Parse command line arguments
    while [[ $# -gt 0 ]]; do
        case $1 in
            -h|--help)
                show_usage
                exit 0
                ;;
            -d|--debug)
                build_type="debug"
                shift
                ;;
            -t|--test)
                run_tests_flag=true
                shift
                ;;
            -c|--commit)
                commit_flag=true
                shift
                ;;
            -a|--all)
                commit_flag=true
                run_tests_flag=true
                shift
                ;;
            -*)
                print_error "Unknown option: $1"
                show_usage
                exit 1
                ;;
            *)
                commit_msg="$1"
                shift
                ;;
        esac
    done
    
    print_status "Starting NeoZorKDEXArb build and deploy process..."
    print_status "Build type: $build_type"
    print_status "Run tests: $run_tests_flag"
    print_status "Commit and push: $commit_flag"
    
    # Check dependencies
    check_dependencies
    
    # Check git status
    check_git_status
    
    # Build project
    build_project "$build_type"
    
    # Run tests if requested
    if [ "$run_tests_flag" = true ]; then
        run_tests
    fi
    
    # Commit and push if requested
    if [ "$commit_flag" = true ]; then
        commit_and_push "$commit_msg"
    fi
    
    print_success "Build and deploy process completed successfully!"
}

# Run main function with all arguments
main "$@"
