cmake_minimum_required(VERSION 3.28)
project(NeoZorKDEXArb LANGUAGES CXX)

# Install Standard C++
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# Source Files
add_executable(NeoZorKDEXArb
    src/main.cpp
    src/arbitrage.cpp
)

# Header Folders
target_include_directories(NeoZorKDEXArb PRIVATE include)

# Crossplatform dependant settings
if(APPLE)
    # Settings for macOS (specific libraries)
    message(STATUS "Configuring for macOS")
elseif(UNIX AND NOT APPLE)
    # Settings for Linux
    message(STATUS "Configuring for Linux")
endif()

# Optional: add external libraries (ex: for HTTP-query or JSON)
# find_package(CURL REQUIRED)
# target_link_libraries(NeoZorKDEXArb PRIVATE CURL::libcurl)