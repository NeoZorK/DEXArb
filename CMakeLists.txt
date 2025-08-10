# Modern CMake configuration for NeoZorKDEXArb
# Minimum required CMake version
cmake_minimum_required(VERSION 3.28)

# Project name and language
project(NeoZorKDEXArb 
    VERSION 1.0.7
    DESCRIPTION "High-performance DEX Arbitrage Scanner"
    LANGUAGES CXX
)

# Set C++ standard to 23 (latest available)
set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# Enable testing
enable_testing()

# Set build type if not specified
if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Release)
endif()

# Set global compiler flags
if(CMAKE_CXX_COMPILER_ID MATCHES "GNU|Clang")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra -Wpedantic -O3 -fPIC -std=c++23 -march=native -fstack-protector-strong -D_FORTIFY_SOURCE=2")
    if(CMAKE_CXX_COMPILER_ID MATCHES "Clang")
        set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Weverything -Wno-c++98-compat -Wno-c++98-compat-pedantic")
    endif()
elseif(CMAKE_CXX_COMPILER_ID MATCHES "MSVC")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /W4 /O2 /std:c++23")
endif()

# Source files organized by functionality
set(SOURCES
    src/main.cpp
    src/core/arbitrage.cpp
    src/core/blockchain.cpp
    src/core/profit_analyzer.cpp
    src/dex/dex_pools.cpp
    src/dex/dex_scanner.cpp
    src/dex/dex_stats.cpp
    src/dex/dex_tokens.cpp
    src/network/rpc_core.cpp
    src/network/queries.cpp
    src/utils/input.cpp
    src/utils/measure.cpp
    src/utils/wallet.cpp
    src/config/config_manager.cpp
    src/cli/command_parser.cpp
    src/cli/help_display.cpp
    src/core/application.cpp
    src/di/service_container.cpp
)

# Create main executable
add_executable(NeoZorKDEXArb ${SOURCES})

# Include directories
target_include_directories(NeoZorKDEXArb PRIVATE 
    include
    ${CMAKE_CURRENT_SOURCE_DIR}/src
    ${CMAKE_CURRENT_SOURCE_DIR}
)



# Find and link dependencies via vcpkg
find_package(CURL REQUIRED)
find_package(nlohmann_json REQUIRED)

# Only find GTest if testing is enabled
if(BUILD_TESTING)
    find_package(GTest REQUIRED)
endif()

# Link libraries
target_link_libraries(NeoZorKDEXArb PRIVATE
    CURL::libcurl
    nlohmann_json::nlohmann_json
)

# Enable threading support
find_package(Threads REQUIRED)
target_link_libraries(NeoZorKDEXArb PRIVATE Threads::Threads)

# Cross-platform settings
if(APPLE)
    message(STATUS "Configuring for macOS")
    target_link_libraries(NeoZorKDEXArb PRIVATE 
        "-framework CoreFoundation" 
        "-framework SystemConfiguration"
        "-framework Security"
    )
    # Enable Apple-specific optimizations
    if(CMAKE_CXX_COMPILER_ID MATCHES "Clang")
        target_compile_options(NeoZorKDEXArb PRIVATE -mcpu=apple-m1)
    endif()
    target_compile_definitions(NeoZorKDEXArb PRIVATE PLATFORM_MACOS)
elseif(UNIX AND NOT APPLE)
    message(STATUS "Configuring for Linux")
    # Linux-specific optimizations
    target_compile_options(NeoZorKDEXArb PRIVATE -D_GNU_SOURCE)
    
    # Check for musl vs glibc
    include(CheckCXXSourceCompiles)
    check_cxx_source_compiles(
        "#include <gnu/libc-version.h>
         int main() { return 0; }"
        HAS_GLIBC
    )
    if(NOT HAS_GLIBC)
        message(STATUS "Detected musl libc")
        target_compile_definitions(NeoZorKDEXArb PRIVATE PLATFORM_LINUX PLATFORM_MUSL)
    else()
        message(STATUS "Detected glibc")
        target_compile_definitions(NeoZorKDEXArb PRIVATE PLATFORM_LINUX PLATFORM_GLIBC)
    endif()
elseif(WIN32)
    message(STATUS "Configuring for Windows")
    target_link_libraries(NeoZorKDEXArb PRIVATE ws2_32)
    target_compile_definitions(NeoZorKDEXArb PRIVATE PLATFORM_WINDOWS)
endif()

# Include platform header
target_include_directories(NeoZorKDEXArb PRIVATE 
    include
    ${CMAKE_CURRENT_SOURCE_DIR}/src
    ${CMAKE_CURRENT_SOURCE_DIR}
)

# Set compile definitions
target_compile_definitions(NeoZorKDEXArb PRIVATE
    $<$<CONFIG:Debug>:_DEBUG>
    $<$<CONFIG:Release>:NDEBUG>
    VERSION="${PROJECT_VERSION}"
    BUILD_DATE="${CMAKE_BUILD_DATE}"
)

# Installation target
install(TARGETS NeoZorKDEXArb 
    RUNTIME DESTINATION bin
    LIBRARY DESTINATION lib
    ARCHIVE DESTINATION lib
)

# Install headers
install(DIRECTORY include/ DESTINATION include)

# Install documentation
install(DIRECTORY docs/ DESTINATION share/doc/NeoZorKDEXArb)

# Create package
include(CPack)
set(CPACK_PACKAGE_NAME "NeoZorKDEXArb")
set(CPACK_PACKAGE_VERSION "${PROJECT_VERSION}")
set(CPACK_PACKAGE_DESCRIPTION_SUMMARY "DEX Arbitrage Scanner")
set(CPACK_PACKAGE_VENDOR "NeoZork")
set(CPACK_PACKAGE_CONTACT "rostyslav@neozork.com")

# Testing configuration
enable_testing()

# Create test executables
add_executable(NeoZorKDEXArbTests
    tests/test_main.cpp
)

# Modern test executables
add_executable(ModernResultTests
    tests/cpp/test_modern_result.cpp
)

add_executable(ModernFormatTests
    tests/cpp/test_modern_format.cpp
)

# New comprehensive test executables
add_executable(AllFlagsAndResultsTests
    tests/cpp/test_all_flags_and_results.cpp
)

add_executable(CommandLineFlagsTests
    tests/cpp/test_command_line_flags.cpp
)

add_executable(CliCommandsTests
    tests/cpp/test_cli_commands.cpp
    src/cli/command_parser.cpp
)

add_executable(HelpDisplayTests
    tests/cpp/test_help_display.cpp
    src/cli/help_display.cpp
)

add_executable(ModernResultExtendedTests
    tests/cpp/test_modern_result_extended.cpp
)

add_executable(PlatformCompatibilityTests
    tests/cpp/test_platform_compatibility.cpp
)

add_executable(ProfitAnalyzerTests
    tests/cpp/test_profit_analyzer.cpp
    src/core/profit_analyzer.cpp
)

add_executable(DexScannerTests
    tests/cpp/test_dex_scanner.cpp
    src/dex/dex_scanner.cpp
    src/network/rpc_core.cpp
    src/utils/measure.cpp
)

add_executable(DexPoolsTests
    tests/cpp/test_dex_pools.cpp
    src/dex/dex_pools.cpp
    src/network/rpc_core.cpp
    src/utils/measure.cpp
)

add_executable(DexTokensTests
    tests/cpp/test_dex_tokens.cpp
    src/dex/dex_tokens.cpp
    src/network/rpc_core.cpp
    src/utils/measure.cpp
)

add_executable(ArbitrageTests
    tests/cpp/test_arbitrage.cpp
    src/core/arbitrage.cpp
    src/core/profit_analyzer.cpp
    src/dex/dex_pools.cpp
    src/dex/dex_tokens.cpp
    src/network/rpc_core.cpp
    src/utils/measure.cpp
)

add_executable(BlockchainTests
    tests/cpp/test_blockchain.cpp
    src/core/blockchain.cpp
    src/network/rpc_core.cpp
    src/utils/measure.cpp
)

# Include directories for test executables
target_include_directories(ModernResultTests PRIVATE 
    include
    ${CMAKE_CURRENT_SOURCE_DIR}/src
)

target_include_directories(ModernFormatTests PRIVATE 
    include
    ${CMAKE_CURRENT_SOURCE_DIR}/src
)

target_include_directories(AllFlagsAndResultsTests PRIVATE 
    include
    ${CMAKE_CURRENT_SOURCE_DIR}/src
)

target_include_directories(CommandLineFlagsTests PRIVATE 
    include
    ${CMAKE_CURRENT_SOURCE_DIR}/src
)

target_include_directories(CliCommandsTests PRIVATE 
    include
    ${CMAKE_CURRENT_SOURCE_DIR}/src
)

target_include_directories(HelpDisplayTests PRIVATE 
    include
    ${CMAKE_CURRENT_SOURCE_DIR}/src
)

target_include_directories(ModernResultExtendedTests PRIVATE 
    include
    ${CMAKE_CURRENT_SOURCE_DIR}/src
)

target_include_directories(PlatformCompatibilityTests PRIVATE 
    include
    ${CMAKE_CURRENT_SOURCE_DIR}/src
)

target_include_directories(ProfitAnalyzerTests PRIVATE 
    include
    ${CMAKE_CURRENT_SOURCE_DIR}/src
)

target_include_directories(DexScannerTests PRIVATE 
    include
    ${CMAKE_CURRENT_SOURCE_DIR}/src
)

target_include_directories(DexPoolsTests PRIVATE 
    include
    ${CMAKE_CURRENT_SOURCE_DIR}/src
)

target_include_directories(DexTokensTests PRIVATE 
    include
    ${CMAKE_CURRENT_SOURCE_DIR}/src
)

target_include_directories(ArbitrageTests PRIVATE 
    include
    ${CMAKE_CURRENT_SOURCE_DIR}/src
)

target_include_directories(BlockchainTests PRIVATE 
    include
    ${CMAKE_CURRENT_SOURCE_DIR}/src
)

# Link libraries to test executables
target_link_libraries(ModernResultTests PRIVATE 
    GTest::gtest 
    GTest::gtest_main
)

target_link_libraries(ModernFormatTests PRIVATE 
    GTest::gtest 
    GTest::gtest_main
)

# For tests that need the actual implementations, we need to include source files
target_link_libraries(AllFlagsAndResultsTests PRIVATE 
    GTest::gtest 
    GTest::gtest_main
    Threads::Threads
)

target_link_libraries(CommandLineFlagsTests PRIVATE 
    GTest::gtest 
    GTest::gtest_main
    Threads::Threads
)

target_link_libraries(CliCommandsTests PRIVATE 
    GTest::gtest 
    GTest::gtest_main
    Threads::Threads
)

target_link_libraries(HelpDisplayTests PRIVATE 
    GTest::gtest 
    GTest::gtest_main
    Threads::Threads
)

target_link_libraries(ModernResultExtendedTests PRIVATE 
    GTest::gtest 
    GTest::gtest_main
    Threads::Threads
)

target_link_libraries(PlatformCompatibilityTests PRIVATE 
    GTest::gtest 
    GTest::gtest_main
    Threads::Threads
)

target_link_libraries(ProfitAnalyzerTests PRIVATE 
    GTest::gtest 
    GTest::gtest_main
    Threads::Threads
)

target_link_libraries(DexScannerTests PRIVATE 
    GTest::gtest 
    GTest::gtest_main
    Threads::Threads
    CURL::libcurl
)

# Add tests
add_test(NAME NeoZorKDEXArbTests COMMAND NeoZorKDEXArbTests)
add_test(NAME ModernResultTests COMMAND ModernResultTests)
add_test(NAME ModernFormatTests COMMAND ModernFormatTests)
add_test(NAME AllFlagsAndResultsTests COMMAND AllFlagsAndResultsTests)
add_test(NAME CommandLineFlagsTests COMMAND CommandLineFlagsTests)
add_test(NAME CliCommandsTests COMMAND CliCommandsTests)
add_test(NAME HelpDisplayTests COMMAND HelpDisplayTests)
add_test(NAME ModernResultExtendedTests COMMAND ModernResultExtendedTests)
add_test(NAME PlatformCompatibilityTests COMMAND PlatformCompatibilityTests)
add_test(NAME ProfitAnalyzerTests COMMAND ProfitAnalyzerTests)
add_test(NAME DexScannerTests COMMAND DexScannerTests)

# Set test properties
set_tests_properties(AllFlagsAndResultsTests PROPERTIES TIMEOUT 300)
set_tests_properties(CommandLineFlagsTests PROPERTIES TIMEOUT 300)
set_tests_properties(CliCommandsTests PROPERTIES TIMEOUT 300)
set_tests_properties(HelpDisplayTests PROPERTIES TIMEOUT 300)
set_tests_properties(ModernResultExtendedTests PROPERTIES TIMEOUT 300)
set_tests_properties(ProfitAnalyzerTests PROPERTIES TIMEOUT 300)
set_tests_properties(DexScannerTests PROPERTIES TIMEOUT 300)

# Set output directories
set_target_properties(NeoZorKDEXArb PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin
    LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib
    ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib
)

# Print configuration summary
message(STATUS "Configuration Summary:")
message(STATUS "  Project: ${PROJECT_NAME}")
message(STATUS "  Version: ${PROJECT_VERSION}")
message(STATUS "  C++ Standard: ${CMAKE_CXX_STANDARD}")
message(STATUS "  Build Type: ${CMAKE_BUILD_TYPE}")
message(STATUS "  Install Prefix: ${CMAKE_INSTALL_PREFIX}")
message(STATUS "  CURL Version: ${CURL_VERSION}")
message(STATUS "  nlohmann-json Version: ${nlohmann_json_VERSION}")

