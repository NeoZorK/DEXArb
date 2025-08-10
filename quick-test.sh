#!/bin/bash

# Quick test script for modern build system
# This script tests the basic functionality without full container build

set -euo pipefail

echo "🧪 Quick Test for Modern Build System"
echo "====================================="

# Check if we're on macOS
if [[ "$OSTYPE" != "darwin"* ]]; then
    echo "❌ This script is designed for macOS with Apple Container CLI"
    exit 1
fi

# Check Apple Container CLI
if ! command -v container &> /dev/null; then
    echo "❌ Apple Container CLI not found"
    echo "   Install Command Line Tools: xcode-select --install"
    exit 1
fi

echo "✅ Apple Container CLI found: $(container version | head -n1)"

# Check CMake
if ! command -v cmake &> /dev/null; then
    echo "❌ CMake not found"
    echo "   Install via Homebrew: brew install cmake"
    exit 1
fi

echo "✅ CMake found: $(cmake --version | head -n1)"

# Check C++ compiler
if ! command -v clang++ &> /dev/null; then
    echo "❌ clang++ not found"
    echo "   Install Command Line Tools: xcode-select --install"
    exit 1
fi

echo "✅ C++ compiler found: $(clang++ --version | head -n1)"

# Check Git
if ! command -v git &> /dev/null; then
    echo "❌ Git not found"
    exit 1
fi

echo "✅ Git found: $(git --version)"

# Test vcpkg setup
echo ""
echo "📦 Testing vcpkg setup..."

if [ ! -d "vcpkg" ]; then
    echo "   Cloning vcpkg..."
    git clone https://github.com/Microsoft/vcpkg.git
fi

cd vcpkg

if [ ! -f "vcpkg" ]; then
    echo "   Bootstrapping vcpkg..."
    ./bootstrap-vcpkg.sh
fi

echo "✅ vcpkg setup completed"

# Test CMake configuration
echo ""
echo "🔧 Testing CMake configuration..."

cd ..

if [ -d "build" ]; then
    rm -rf build
fi

mkdir -p build
cd build

if cmake .. -DCMAKE_TOOLCHAIN_FILE=../vcpkg/scripts/buildsystems/vcpkg.cmake; then
    echo "✅ CMake configuration successful"
else
    echo "❌ CMake configuration failed"
    exit 1
fi

cd ..

echo ""
echo "🎉 Quick test completed successfully!"
echo ""
echo "Next steps:"
echo "  1. Run full build: ./build-apple-container.sh"
echo "  2. Run local build: ./build-apple-container.sh --local"
echo "  3. Check documentation: docs/BUILD_MODERN.md"
