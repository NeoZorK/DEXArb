cmake_minimum_required(VERSION 3.28)
project(NeoZorKDEXArb LANGUAGES CXX)

# Standard C++
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# Source Files
set(SOURCES
    src/main.cpp
    src/arbitrage.cpp
    src/MeasureRunTime.cpp
    src/input.cpp
)

# Dynamic Version
add_executable(NeoZorKDEXArb ${SOURCES})
target_include_directories(NeoZorKDEXArb PRIVATE include)
set_target_properties(NeoZorKDEXArb PROPERTIES
    OUTPUT_NAME "dexarb2"  # Name of output file
    RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin"  # Save to bin
)
target_compile_options(NeoZorKDEXArb PRIVATE -O2)  # Optimisation

# Static Version
add_executable(NeoZorKDEXArb_static ${SOURCES})
target_include_directories(NeoZorKDEXArb_static PRIVATE include)
set_target_properties(NeoZorKDEXArb_static PROPERTIES
    OUTPUT_NAME "dexarb"  # Name of output file
    RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin"  # Save to bin
)
target_compile_options(NeoZorKDEXArb_static PRIVATE -O2)  # Optimisation
target_link_options(NeoZorKDEXArb_static PRIVATE -static)  # Static Linking


# Platform dependency
if(WIN32)
    message(STATUS "Configuring for Windows")
    # For Windows with MSVC static linking through /MT
    if(MSVC)
        target_compile_options(NeoZorKDEXArb_static PRIVATE /MT)  # Static runtime-lib
        target_compile_options(NeoZorKDEXArb PRIVATE /MD)        # Dynamic runtime-lib
        # Delete extension .exe in names, if necessary
        set_target_properties(NeoZorKDEXArb PROPERTIES SUFFIX "")
        set_target_properties(NeoZorKDEXArb_static PROPERTIES SUFFIX "")
    else()
        # For MinGW Static Linking
        target_link_options(NeoZorKDEXArb_static PRIVATE -static)
    endif()
elseif(APPLE)
    message(STATUS "Configuring for macOS")
elseif(UNIX AND NOT APPLE)
    message(STATUS "Configuring for Linux")
    target_link_options(NeoZorKDEXArb_static PRIVATE -static)  # Static Linking for Linux
endif()

# Windows: Visual Studio 17
#cd build
#cmake .. -G "Visual Studio 17 2022"  # Specify your version of VS
#cmake --build . --config Release

# Windows: Install MinGW (ex: through MSYS2: pacman -S mingw-w64-x86_64-gcc).
#cd build
#cmake .. -G "MinGW Makefiles" -DCMAKE_CXX_COMPILER=g++
#mingw32-make

# Unix: Execute this commands to build the project
#mkdir build
#cd build
#cmake .. -DCMAKE_CXX_COMPILER=clang++
#make


