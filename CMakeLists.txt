cmake_minimum_required(VERSION 3.16)
project(DEXArb)

# Set C++ standard
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Set build type
if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Release)
endif()

# Build optimizations for faster compilation
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O1")
set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -O0")
set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -O2")

# Enable parallel compilation
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -pipe")

# Enable parallel builds
set(CMAKE_BUILD_PARALLEL_LEVEL $ENV{MAKEFLAGS})

# Enable testing
enable_testing()

# Find required packages
find_package(Threads REQUIRED)
find_package(CURL REQUIRED)

# Include directories
include_directories(${CMAKE_SOURCE_DIR}/include)
include_directories(${CMAKE_SOURCE_DIR}/src)

# Source files
file(GLOB_RECURSE SOURCES "src/*.cpp")

# Main executable
add_executable(dexarb ${SOURCES})
target_link_libraries(dexarb Threads::Threads CURL::libcurl)

# Add tests subdirectory
add_subdirectory(tests/cpp)

# Install target
install(TARGETS dexarb DESTINATION bin)
