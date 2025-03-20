# Minimum required CMake version
cmake_minimum_required(VERSION 3.28)

# Project name and language
project(NeoZorKDEXArb LANGUAGES CXX)

# Set C++ standard to 17
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# Source files (all .cpp files from src directory)
add_executable(NeoZorKDEXArb
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
    Src/preliminary_check_rpc_endpoints.cpp
)

# Include directories (header files in include folder)
target_include_directories(NeoZorKDEXArb PRIVATE include)

# Find and link CURL library (required for HTTP requests)
find_package(CURL REQUIRED)
if(CURL_FOUND)
    target_link_libraries(NeoZorKDEXArb PRIVATE CURL::libcurl)
else()
    message(FATAL_ERROR "CURL not found. Please install libcurl development package.")
endif()

# Enable threading support
find_package(Threads REQUIRED)
target_link_libraries(NeoZorKDEXArb PRIVATE Threads::Threads)

# Cross-platform settings
if(APPLE)
    # macOS-specific settings
    message(STATUS "Configuring for macOS")
    # Ensure libcurl is linked correctly on macOS
    target_link_libraries(NeoZorKDEXArb PRIVATE "-framework CoreFoundation" "-framework SystemConfiguration")
elseif(UNIX AND NOT APPLE)
    # Linux-specific settings
    message(STATUS "Configuring for Linux")
    # No additional libraries needed beyond CURL and Threads
elseif(WIN32)
    # Windows-specific settings
    message(STATUS "Configuring for Windows")
    # Ensure proper linking of Winsock for CURL on Windows
    target_link_libraries(NeoZorKDEXArb PRIVATE ws2_32)
endif()

# Compiler-specific flags for warnings and optimization
if(CMAKE_CXX_COMPILER_ID MATCHES "GNU|Clang")
    target_compile_options(NeoZorKDEXArb PRIVATE -Wall -Wextra -O2)
elseif(CMAKE_CXX_COMPILER_ID MATCHES "MSVC")
    target_compile_options(NeoZorKDEXArb PRIVATE /W4 /O2)
endif()

# Optional: Installation target (for packaging)
install(TARGETS NeoZorKDEXArb DESTINATION bin)

