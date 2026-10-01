#!/bin/bash

# Universal Interactive Build Script for NeoZorKDEXArb
# Supports: macOS, Linux, Windows (Wine), Alpine, Ubuntu containers
# Created by Rostyslav S. on 26.02.2025

set -euo pipefail

# Colors for output
RED='\033[0;31m'
GREEN='\033[0;32m'
YELLOW='\033[1;33m'
BLUE='\033[0;34m'
CYAN='\033[0;36m'
MAGENTA='\033[0;35m'
NC='\033[0m' # No Color

# Global variables
PROJECT_NAME="NeoZorKDEXArb"
VERSION="1.0.7"
BUILD_DIR=""
PLATFORM=""
BUILD_TYPE="Release"
CLEAN_BUILD=false
RUN_TESTS=false
CREATE_PACKAGE=false
VERBOSE=false
PROJECT_ROOT=""

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
            if [ "$VERBOSE" = true ]; then
                echo -e "${BLUE}[${timestamp}] DEBUG:${NC} $message"
            fi
            ;;
        "SUCCESS")
            echo -e "${CYAN}[${timestamp}] SUCCESS:${NC} $message"
            ;;
    esac
}

# Function to show banner
show_banner() {
    clear
    echo -e "${GREEN}🚀 Universal Build Script v${VERSION}${NC}"
    echo -e "${GREEN}NeoZorKDEXArb - DEX Arbitrage Scanner${NC}"
    echo ""
}

# Function to detect platform
detect_platform() {
    log "INFO" "Detecting platform..."
    
    if [[ "$OSTYPE" == "darwin"* ]]; then
        PLATFORM="macos"
        log "SUCCESS" "Detected platform: macOS"
    elif [[ "$OSTYPE" == "linux-gnu"* ]]; then
        if [ -f /etc/alpine-release ]; then
            PLATFORM="alpine"
            log "SUCCESS" "Detected platform: Alpine Linux"
        else
            PLATFORM="linux"
            log "SUCCESS" "Detected platform: Linux"
        fi
    elif [[ "$OSTYPE" == "msys" ]] || [[ "$OSTYPE" == "cygwin" ]]; then
        PLATFORM="windows"
        log "SUCCESS" "Detected platform: Windows (MSYS/Cygwin)"
    else
        PLATFORM="unknown"
        log "WARN" "Unknown platform: $OSTYPE"
    fi
}

# Function to get project root directory
get_project_root() {
    # Try to find the project root by looking for CMakeLists.txt
    local current_dir="$(pwd)"
    local max_depth=5
    local depth=0
    
    while [ "$depth" -lt "$max_depth" ] && [ "$current_dir" != "/" ]; do
        if [ -f "$current_dir/CMakeLists.txt" ] && [ -f "$current_dir/include/main.h" ]; then
            PROJECT_ROOT="$current_dir"
            log "DEBUG" "Found project root: $PROJECT_ROOT"
            return 0
        fi
        current_dir="$(dirname "$current_dir")"
        depth=$((depth + 1))
    done
    
    # Fallback: assume we're in scripts/build directory
    PROJECT_ROOT="$(dirname "$(dirname "$(pwd)")")"
    log "DEBUG" "Using fallback project root: $PROJECT_ROOT"
}

# Function to check dependencies
check_dependencies() {
    log "INFO" "Checking build dependencies..."
    
    local missing_deps=()
    
    # Check for git
    if ! command -v git &> /dev/null; then
        missing_deps+=("git")
    fi
    
    # Check for CMake
    if ! command -v cmake &> /dev/null; then
        missing_deps+=("cmake")
    fi
    
    # Check for make
    if ! command -v make &> /dev/null; then
        missing_deps+=("make")
    fi
    
    # Check for C++ compiler
    if ! command -v g++ &> /dev/null && ! command -v clang++ &> /dev/null; then
        missing_deps+=("c++-compiler")
    fi
    
    # Check for Docker (for container builds)
    if ! command -v docker &> /dev/null; then
        log "WARN" "Docker not found - container builds will be disabled"
    fi
    
    # Check for Wine (for Windows builds)
    if ! command -v wine &> /dev/null; then
        log "WARN" "Wine not found - Windows builds will be disabled"
    fi
    
    if [ ${#missing_deps[@]} -gt 0 ]; then
        log "ERROR" "Missing dependencies: ${missing_deps[*]}"
        show_install_instructions
        exit 1
    fi
    
    log "SUCCESS" "All dependencies satisfied"
}

# Function to show install instructions
show_install_instructions() {
    echo ""
    echo -e "${YELLOW}📋 Installation Instructions:${NC}"
    echo ""
    
    case $PLATFORM in
        "macos")
            echo -e "${BLUE}macOS:${NC}"
            echo "  brew install cmake git"
            echo "  xcode-select --install"
            ;;
        "linux")
            echo -e "${BLUE}Ubuntu/Debian:${NC}"
            echo "  sudo apt install build-essential cmake git"
            echo ""
            echo -e "${BLUE}CentOS/RHEL:${NC}"
            echo "  sudo yum groupinstall 'Development Tools'"
            echo "  sudo yum install cmake git"
            ;;
        "alpine")
            echo -e "${BLUE}Alpine:${NC}"
            echo "  apk add --no-cache build-base cmake git"
            ;;
        *)
            echo "  Please install: cmake, git, make, and a C++ compiler"
            ;;
    esac
}

# Function to show build options
show_build_options() {
    echo ""
    echo -e "${YELLOW}🎯 Build Options:${NC}"
    echo ""
    echo -e "${BLUE}1.${NC} Native build (${GREEN}recommended${NC})"
    echo -e "${BLUE}2.${NC} Container build (Docker)"
    echo -e "${BLUE}3.${NC} Cross-platform build"
    echo -e "${BLUE}4.${NC} Windows build (Wine)"
    echo -e "${BLUE}5.${NC} Alpine container build"
    echo -e "${BLUE}6.${NC} Ubuntu container build"
    echo -e "${BLUE}0.${NC} Exit"
    echo ""
}

# Function to get user choice
get_user_choice() {
    local prompt="$1"
    local min="$2"
    local max="$3"
    local choice
    
    while true; do
        echo -e "${YELLOW}$prompt${NC}" >&2
        if ! read -r choice; then
            # If read fails (e.g., no stdin), use default value
            choice="$min"
            echo "Using default choice: $choice" >&2
            echo "$choice"
            return 0
        fi
        
        if [[ "$choice" =~ ^[0-9]+$ ]] && [ "$choice" -ge "$min" ] && [ "$choice" -le "$max" ]; then
            echo "$choice"
            return 0
        else
            echo -e "${RED}Invalid choice. Please enter a number between $min and $max.${NC}" >&2
        fi
    done
}

# Function to get build configuration
get_build_config() {
    echo ""
    echo -e "${YELLOW}⚙️  Build Configuration:${NC}"
    echo ""
    
    # Build type
    echo -e "${BLUE}Build type:${NC}"
    echo "  1. Release (optimized)"
    echo "  2. Debug (with symbols)"
    echo "  3. RelWithDebInfo (release with debug info)"
    echo ""
    
    local build_type_choice=$(get_user_choice "Choose build type (1-3):" 1 3)
    case $build_type_choice in
        1) BUILD_TYPE="Release" ;;
        2) BUILD_TYPE="Debug" ;;
        3) BUILD_TYPE="RelWithDebInfo" ;;
    esac
    
    # Clean build
    echo ""
    echo -e "${YELLOW}Clean build directory before building? (y/N):${NC}"
    if ! read -r clean_choice; then
        clean_choice="n"
        echo "Using default: n"
    fi
    if [[ "$clean_choice" =~ ^[Yy]$ ]]; then
        CLEAN_BUILD=true
    fi
    
    # Run tests
    echo ""
    echo -e "${YELLOW}Run tests after build? (y/N):${NC}"
    if ! read -r test_choice; then
        test_choice="n"
        echo "Using default: n"
    fi
    if [[ "$test_choice" =~ ^[Yy]$ ]]; then
        RUN_TESTS=true
    fi
    
    # Create package
    echo ""
    echo -e "${YELLOW}Create distribution package? (y/N):${NC}"
    if ! read -r package_choice; then
        package_choice="n"
        echo "Using default: n"
    fi
    if [[ "$package_choice" =~ ^[Yy]$ ]]; then
        CREATE_PACKAGE=true
    fi
    
    # Verbose output
    echo ""
    echo -e "${YELLOW}Verbose output? (y/N):${NC}"
    if ! read -r verbose_choice; then
        verbose_choice="n"
        echo "Using default: n"
    fi
    if [[ "$verbose_choice" =~ ^[Yy]$ ]]; then
        VERBOSE=true
    fi
}

# Function to setup vcpkg (simplified)
setup_vcpkg() {
    log "INFO" "Setting up vcpkg..."
    
    # Change to project root directory
    cd "$PROJECT_ROOT"
    
    if [ ! -d "vcpkg" ]; then
        log "INFO" "Cloning vcpkg..."
        git clone https://github.com/Microsoft/vcpkg.git
    fi
    
    cd vcpkg
    
    if [ ! -f "vcpkg" ]; then
        log "INFO" "Bootstrapping vcpkg..."
        if [ -f "bootstrap-vcpkg.sh" ]; then
            ./bootstrap-vcpkg.sh
        elif [ -f "bootstrap-vcpkg.bat" ]; then
            ./bootstrap-vcpkg.bat
        else
            log "ERROR" "vcpkg bootstrap script not found"
            exit 1
        fi
    fi
    
    # Install required packages based on platform
    case $PLATFORM in
        "macos")
            ./vcpkg install --triplet=x64-osx curl nlohmann-json gtest
            ;;
        "linux"|"alpine")
            ./vcpkg install --triplet=x64-linux curl nlohmann-json gtest
            ;;
        "windows")
            ./vcpkg install --triplet=x64-windows curl nlohmann-json gtest
            ;;
    esac
    
    cd "$PROJECT_ROOT"
    log "SUCCESS" "vcpkg setup completed"
}

# Function to create build directory
create_build_directory() {
    local dir_name=$1
    BUILD_DIR="$PROJECT_ROOT/$dir_name"
    
    if [ "$CLEAN_BUILD" = true ] && [ -d "$BUILD_DIR" ]; then
        log "INFO" "Cleaning existing build directory: $BUILD_DIR"
        rm -rf "$BUILD_DIR"
    fi
    
    mkdir -p "$BUILD_DIR"
    log "SUCCESS" "Build directory created: $BUILD_DIR"
}

# Function to build natively
build_native() {
    log "INFO" "Building natively for $PLATFORM..."
    
    # Change to project root directory
    cd "$PROJECT_ROOT"
    
    create_build_directory "build-$PLATFORM"
    cd "$BUILD_DIR"
    
    # Configure with CMake
    log "INFO" "Configuring with CMake..."
    if [ "$VERBOSE" = true ]; then
        cmake .. -DCMAKE_BUILD_TYPE="$BUILD_TYPE" -DCMAKE_VERBOSE_MAKEFILE=ON
    else
        cmake .. -DCMAKE_BUILD_TYPE="$BUILD_TYPE"
    fi
    
    # Build
    log "INFO" "Building..."
    local cpu_count=$(nproc 2>/dev/null || sysctl -n hw.ncpu 2>/dev/null || echo 4)
    if [ "$VERBOSE" = true ]; then
        make -j"$cpu_count" VERBOSE=1
    else
        make -j"$cpu_count"
    fi
    
    cd "$PROJECT_ROOT"
    log "SUCCESS" "Native build completed successfully"
}

# Function to build in container
build_container() {
    local container_type="$1"
    log "INFO" "Building in $container_type container..."
    
    if ! command -v docker &> /dev/null; then
        log "ERROR" "Docker not found. Please install Docker first."
        exit 1
    fi
    
    create_build_directory "build-$container_type"
    
    local docker_cmd=""
    case $container_type in
        "alpine")
            docker_cmd="alpine:latest"
            ;;
        "ubuntu")
            docker_cmd="ubuntu:22.04"
            ;;
        *)
            log "ERROR" "Unknown container type: $container_type"
            exit 1
            ;;
    esac
    
    # Get absolute path to project root
    local project_root="$PROJECT_ROOT"
    local build_dir="$project_root/build-$container_type"
    
    docker run --rm -v "$project_root:/workspace" -w /workspace "$docker_cmd" sh -c "
        if [ \"$container_type\" = \"alpine\" ]; then
            apk add --no-cache build-base cmake git curl-dev gtest-dev
        else
            apt-get update && apt-get install -y build-essential cmake git libcurl4-openssl-dev libgtest-dev
        fi
        mkdir -p build-$container_type
        cd build-$container_type
        cmake .. -DCMAKE_BUILD_TYPE=$BUILD_TYPE
        make -j\$(nproc)
    "
    
    log "SUCCESS" "Container build completed successfully"
}

# Function to build for Windows
build_windows() {
    log "INFO" "Building for Windows using Wine..."
    
    create_build_directory "build-windows"
    cd "$BUILD_DIR"
    
    log "INFO" "Configuring with CMake for Windows..."
    
    cmake .. \
        -DCMAKE_BUILD_TYPE="$BUILD_TYPE" \
        -DCMAKE_SYSTEM_NAME=Windows \
        -DCMAKE_C_COMPILER=x86_64-w64-mingw32-gcc \
        -DCMAKE_CXX_COMPILER=x86_64-w64-mingw32-g++ \
        -DCMAKE_RC_COMPILER=x86_64-w64-mingw32-windres
    
    if [ $? -ne 0 ]; then
        log "ERROR" "CMake configuration failed"
        exit 1
    fi
    
    log "INFO" "Building for Windows..."
    mingw32-make
    
    if [ $? -ne 0 ]; then
        log "ERROR" "Windows build failed"
        exit 1
    fi
    
    log "SUCCESS" "Windows build completed successfully"
    cd "$PROJECT_ROOT"
}

# Function to run tests
run_tests() {
    if [ "$RUN_TESTS" = false ]; then
        return
    fi
    
    log "INFO" "Running tests..."
    
    # Check if we're in the build directory
    if [ ! -d "$BUILD_DIR" ]; then
        log "ERROR" "Build directory not found: $BUILD_DIR"
        return
    fi
    
    # Try to run tests using ctest first
    if [ -f "$BUILD_DIR/tests/cpp/CTestTestfile.cmake" ]; then
        log "INFO" "Running tests using CTest..."
        cd "$BUILD_DIR/tests/cpp"
        
        if ctest --output-on-failure --parallel 4; then
            log "SUCCESS" "All tests passed via CTest"
        else
            log "ERROR" "Some tests failed via CTest"
        fi
        
        cd "$PROJECT_ROOT"
        return
    fi
    
    # Fallback: Look for test executables manually
    local test_executables=()
    
    # Check for test executables in tests/cpp directory
    if [ -d "$BUILD_DIR/tests/cpp" ]; then
        for test_file in "$BUILD_DIR/tests/cpp"/test_*; do
            if [ -f "$test_file" ] && [ -x "$test_file" ]; then
                test_executables+=("$test_file")
            fi
        done
    fi
    
    # Also check for common test executable names in other locations
    if [ -f "$BUILD_DIR/AllTestsInMain" ] && [ -x "$BUILD_DIR/AllTestsInMain" ]; then
        test_executables+=("$BUILD_DIR/AllTestsInMain")
    fi
    
    if [ -f "$BUILD_DIR/tests/AllTestsInMain" ] && [ -x "$BUILD_DIR/tests/AllTestsInMain" ]; then
        test_executables+=("$BUILD_DIR/tests/AllTestsInMain")
    fi
    
    if [ -f "$BUILD_DIR/bin/AllTestsInMain" ] && [ -x "$BUILD_DIR/bin/AllTestsInMain" ]; then
        test_executables+=("$BUILD_DIR/bin/AllTestsInMain")
    fi
    
    if [ ${#test_executables[@]} -eq 0 ]; then
        log "WARN" "No test executables found, skipping tests"
        return
    fi
    
    log "INFO" "Found ${#test_executables[@]} test executable(s)"
    
    # Run all test executables
    cd "$BUILD_DIR"
    local test_count=0
    local passed_count=0
    
    for test_executable in "${test_executables[@]}"; do
        test_count=$((test_count + 1))
        log "INFO" "Running test $test_count/${#test_executables[@]}: $(basename "$test_executable")"
        
        if "$test_executable"; then
            passed_count=$((passed_count + 1))
            log "SUCCESS" "Test passed: $(basename "$test_executable")"
        else
            log "ERROR" "Test failed: $(basename "$test_executable")"
        fi
    done
    
    cd "$PROJECT_ROOT"
    
    if [ $passed_count -eq $test_count ]; then
        log "SUCCESS" "All $test_count tests passed"
    else
        log "WARN" "Tests completed: $passed_count/$test_count passed"
    fi
}

# Function to create package
create_package() {
    if [ "$CREATE_PACKAGE" = false ]; then
        return
    fi
    
    log "INFO" "Creating distribution package..."
    
    local package_dir="$PROJECT_ROOT/dist-$PLATFORM"
    mkdir -p "$package_dir"
    
    # Find the executable
    local executable=""
    if [ -f "$BUILD_DIR/dexarb" ]; then
        executable="$BUILD_DIR/dexarb"
    elif [ -f "$BUILD_DIR/bin/dexarb" ]; then
        executable="$BUILD_DIR/bin/dexarb"
    elif [ -f "$BUILD_DIR/dexarb.exe" ]; then
        executable="$BUILD_DIR/dexarb.exe"
    elif [ -f "$BUILD_DIR/$PROJECT_NAME" ]; then
        executable="$BUILD_DIR/$PROJECT_NAME"
    elif [ -f "$BUILD_DIR/bin/$PROJECT_NAME" ]; then
        executable="$BUILD_DIR/bin/$PROJECT_NAME"
    elif [ -f "$BUILD_DIR/${PROJECT_NAME}.exe" ]; then
        executable="$BUILD_DIR/${PROJECT_NAME}.exe"
    fi
    
    if [ -n "$executable" ] && [ -f "$executable" ]; then
        cp "$executable" "$package_dir/"
        echo "NeoZorKDEXArb v$VERSION" > "$package_dir/README.txt"
        echo "Built on: $(date)" >> "$package_dir/README.txt"
        echo "Platform: $PLATFORM" >> "$package_dir/README.txt"
        echo "Build type: $BUILD_TYPE" >> "$package_dir/README.txt"
        echo "" >> "$package_dir/README.txt"
        echo "Usage: ./$PROJECT_NAME [options]" >> "$package_dir/README.txt"
        echo "Run './$PROJECT_NAME -h' for help" >> "$package_dir/README.txt"
        
        log "SUCCESS" "Package created in: $package_dir"
    else
        log "ERROR" "Executable not found for packaging"
    fi
}

# Function to show build summary
show_build_summary() {
    echo ""
    echo -e "${GREEN}🎉 Build Summary:${NC}"
    echo ""
    echo -e "${BLUE}Platform:${NC} $PLATFORM"
    echo -e "${BLUE}Build type:${NC} $BUILD_TYPE"
    echo -e "${BLUE}Build directory:${NC} $BUILD_DIR"
    echo ""
    
    # Check if executable was created
    local executable=""
    if [ -f "$BUILD_DIR/dexarb" ]; then
        executable="$BUILD_DIR/dexarb"
    elif [ -f "$BUILD_DIR/bin/dexarb" ]; then
        executable="$BUILD_DIR/bin/dexarb"
    elif [ -f "$BUILD_DIR/dexarb.exe" ]; then
        executable="$BUILD_DIR/dexarb.exe"
    elif [ -f "$BUILD_DIR/$PROJECT_NAME" ]; then
        executable="$BUILD_DIR/$PROJECT_NAME"
    elif [ -f "$BUILD_DIR/bin/$PROJECT_NAME" ]; then
        executable="$BUILD_DIR/bin/$PROJECT_NAME"
    elif [ -f "$BUILD_DIR/${PROJECT_NAME}.exe" ]; then
        executable="$BUILD_DIR/${PROJECT_NAME}.exe"
    fi
    
    # Also check for the executable in the root build directory
    if [ -z "$executable" ] && [ -f "$BUILD_DIR/dexarb" ]; then
        executable="$BUILD_DIR/dexarb"
    fi
    
    if [ -n "$executable" ] && [ -f "$executable" ]; then
        echo -e "${GREEN}✅ Executable created:${NC} $executable"
        echo ""
        echo -e "${YELLOW}🚀 Quick test:${NC}"
        echo "  $executable -h                    # Show help"
        echo "  $executable -v                    # Show version"
        echo "  $executable -examples             # Show examples"
        echo "  $executable -scan fantom 1000     # Scan Fantom"
    else
        echo -e "${RED}❌ Executable not found at: $BUILD_DIR/dexarb${NC}"
        echo -e "${YELLOW}Checking if file exists...${NC}"
        ls -la "$BUILD_DIR/" | grep -E "(dexarb|$PROJECT_NAME)" || echo "No files found"
    fi
}

# Function to show help
show_help() {
    echo "Usage: $0 [OPTIONS]"
    echo ""
    echo "Options:"
    echo "  --help, -h          Show this help message"
    echo "  --platform PLATFORM Specify platform (macos, linux, alpine, windows)"
    echo "  --build-type TYPE   Specify build type (Release, Debug, RelWithDebInfo)"
    echo "  --clean             Clean build directory before building"
    echo "  --test              Run tests after build"
    echo "  --package           Create distribution package after build"
    echo "  --verbose           Enable verbose output"
    echo ""
    echo "Examples:"
    echo "  $0                  # Interactive build"
    echo "  $0 --platform macos --build-type Release"
    echo "  $0 --clean --test --package"
}

# Function to parse command line arguments
parse_arguments() {
    while [[ $# -gt 0 ]]; do
        case $1 in
            --help|-h)
                show_help
                exit 0
                ;;
            --platform)
                PLATFORM="$2"
                shift 2
                ;;
            --build-type)
                BUILD_TYPE="$2"
                shift 2
                ;;
            --clean)
                CLEAN_BUILD=true
                shift
                ;;
            --test)
                RUN_TESTS=true
                shift
                ;;
            --package)
                CREATE_PACKAGE=true
                shift
                ;;
            --verbose)
                VERBOSE=true
                shift
                ;;
            *)
                log "ERROR" "Unknown option: $1"
                show_help
                exit 1
                ;;
        esac
    done
}

# Main function
main() {
    # Get project root first
    get_project_root
    
    # Parse command line arguments
    parse_arguments "$@"
    
    # Show banner
    show_banner
    
    # Detect platform if not specified
    if [ -z "$PLATFORM" ]; then
        detect_platform
    fi
    
    # Check dependencies
    check_dependencies
    
    # Interactive mode if no options specified
    if [ "$CLEAN_BUILD" = false ] && [ "$RUN_TESTS" = false ] && [ "$CREATE_PACKAGE" = false ]; then
        log "DEBUG" "Entering interactive mode"
        show_build_options
        local build_method=$(get_user_choice "Choose build method (0-6):" 0 6)
        log "DEBUG" "User chose: '$build_method'"
        
        if [ "$build_method" = "0" ] || [ "$build_method" = 0 ]; then
            log "INFO" "Exiting build script"
            exit 0
        fi
        
        get_build_config
        
        log "DEBUG" "About to execute case statement with build_method='$build_method'"
        
        case $build_method in
            1) # Native build
                log "DEBUG" "Executing native build (option 1)"
                setup_vcpkg
                build_native
                ;;
            2) # Container build
                echo -e "${YELLOW}Choose container type:${NC}"
                echo "  1. Alpine (lightweight)"
                echo "  2. Ubuntu (full compatibility)"
                echo ""
                local container_choice=$(get_user_choice "Choose container (1-2):" 1 2)
                case $container_choice in
                    1) build_container "alpine" ;;
                    2) build_container "ubuntu" ;;
                esac
                ;;
            3) # Cross-platform build
                log "INFO" "Cross-platform build not implemented yet"
                exit 1
                ;;
            4) # Windows build
                build_windows
                ;;
            5) # Alpine container
                build_container "alpine"
                ;;
            6) # Ubuntu container
                build_container "ubuntu"
                ;;
        esac
        
        # Run tests
        run_tests
        
        # Create package
        create_package
        
        # Show summary
        show_build_summary
        
        log "SUCCESS" "Build process completed successfully!"
    else
        # Non-interactive mode
        log "DEBUG" "Entering non-interactive mode"
        setup_vcpkg
        build_native
        
        # Run tests
        run_tests
        
        # Create package
        create_package
        
        # Show summary
        show_build_summary
        
        log "SUCCESS" "Build process completed successfully!"
    fi
}

# Run main function with all arguments
main "$@"
