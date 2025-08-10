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

# Source files (all .cpp files from src directory)
set(SOURCES
    src/main.cpp
    src/blockchain.cpp
    src/rpc_core.cpp
    src/dex_scanner.cpp
    src/dex_pools.cpp
    src/dex_tokens.cpp
    src/dex_stats.cpp
    src/config_manager.cpp
    src/queries.cpp
    src/input.cpp
    src/measure.cpp
    src/arbitrage.cpp
    src/wallet.cpp
    src/profit_analyzer.cpp
    src/modern_utils.cpp
    # New modular source files (temporarily disabled for testing)
    # src/cli/command_parser.cpp
    # src/cli/help_display.cpp
    # src/core/application.cpp
    # src/di/service_container.cpp
)

# Create main executable
add_executable(NeoZorKDEXArb ${SOURCES})

# Include directories
target_include_directories(NeoZorKDEXArb PRIVATE 
    include
    ${CMAKE_CURRENT_SOURCE_DIR}/src
)



# Find and link dependencies via vcpkg
find_package(CURL REQUIRED)
find_package(nlohmann_json REQUIRED)
find_package(GTest REQUIRED)

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
elseif(UNIX AND NOT APPLE)
    message(STATUS "Configuring for Linux")
    # Linux-specific optimizations
    target_compile_options(NeoZorKDEXArb PRIVATE -D_GNU_SOURCE)
elseif(WIN32)
    message(STATUS "Configuring for Windows")
    target_link_libraries(NeoZorKDEXArb PRIVATE ws2_32)
endif()

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

# Include directories for test executables
target_include_directories(ModernResultTests PRIVATE 
    include
    ${CMAKE_CURRENT_SOURCE_DIR}/src
)

target_include_directories(ModernFormatTests PRIVATE 
    include
    ${CMAKE_CURRENT_SOURCE_DIR}/src
)

# Link Google Test to modern tests
target_link_libraries(ModernResultTests PRIVATE 
    GTest::gtest 
    GTest::gtest_main
)

target_link_libraries(ModernFormatTests PRIVATE 
    GTest::gtest 
    GTest::gtest_main
)

# Add tests
add_test(NAME NeoZorKDEXArbTests COMMAND NeoZorKDEXArbTests)
add_test(NAME ModernResultTests COMMAND ModernResultTests)
add_test(NAME ModernFormatTests COMMAND ModernFormatTests)

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

