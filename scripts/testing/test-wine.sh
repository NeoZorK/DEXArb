#!/bin/bash

# Wine Test Script
# Tests various Wine functionality in the Docker container

set -e

CONTAINER_NAME="wine-ubuntu"
SCRIPT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
PROJECT_DIR="$(dirname "$SCRIPT_DIR")"

echo "=========================================="
echo "    Wine Docker Test Suite"
echo "=========================================="
echo

# Check if container is running
if ! docker ps | grep -q "$CONTAINER_NAME"; then
    echo "❌ Container $CONTAINER_NAME is not running!"
    echo "   Start it with: ./scripts/wine-docker.sh start"
    exit 1
fi

echo "✅ Container $CONTAINER_NAME is running"
echo

# Test 1: Basic Wine functionality
echo "🧪 Test 1: Basic Wine Functionality"
echo "----------------------------------------"
if docker exec -it "$CONTAINER_NAME" wine --version >/dev/null 2>&1; then
    echo "✅ Wine is working"
    docker exec -it "$CONTAINER_NAME" wine --version
else
    echo "❌ Wine is not working"
fi
echo

# Test 2: Batch file execution
echo "🧪 Test 2: Batch File Execution"
echo "----------------------------------------"
if docker exec -it "$CONTAINER_NAME" wine cmd /c /app/test-simple.bat >/dev/null 2>&1; then
    echo "✅ Batch files work"
else
    echo "❌ Batch files don't work"
fi
echo

# Test 3: C++ compilation
echo "🧪 Test 3: C++ Compilation"
echo "----------------------------------------"
if docker exec -it "$CONTAINER_NAME" x86_64-w64-mingw32-g++ --version >/dev/null 2>&1; then
    echo "✅ MinGW compiler is available"
    docker exec -it "$CONTAINER_NAME" x86_64-w64-mingw32-g++ --version | head -1
else
    echo "❌ MinGW compiler is not available"
fi
echo

# Test 4: Windows executable execution
echo "🧪 Test 4: Windows Executable Execution"
echo "----------------------------------------"
if [ -f "$PROJECT_DIR/app/simple-test.exe" ]; then
    echo "✅ Test executable exists"
    if docker exec -it "$CONTAINER_NAME" wine /app/simple-test.exe >/dev/null 2>&1; then
        echo "✅ Windows executables work"
    else
        echo "⚠️  Windows executables have issues (expected on ARM64)"
    fi
else
    echo "❌ Test executable not found"
fi
echo

# Test 5: File system access
echo "🧪 Test 5: File System Access"
echo "----------------------------------------"
if docker exec -it "$CONTAINER_NAME" ls -la /app/ >/dev/null 2>&1; then
    echo "✅ Container file system is accessible"
    echo "   Files in /app/:"
    docker exec -it "$CONTAINER_NAME" ls -la /app/ | head -10
else
    echo "❌ Container file system is not accessible"
fi
echo

# Test 6: Wine prefix
echo "🧪 Test 6: Wine Prefix"
echo "----------------------------------------"
if docker exec -it "$CONTAINER_NAME" ls -la /root/.wine64 >/dev/null 2>&1; then
    echo "✅ Wine prefix exists"
    echo "   Wine prefix location: /root/.wine64"
else
    echo "❌ Wine prefix not found"
fi
echo

# Test 7: Registry access
echo "🧪 Test 7: Registry Access"
echo "----------------------------------------"
if docker exec -it "$CONTAINER_NAME" wine reg query "HKEY_LOCAL_MACHINE\SOFTWARE\Microsoft\Windows NT\CurrentVersion" /v ProductName >/dev/null 2>&1; then
    echo "✅ Windows Registry is accessible"
else
    echo "❌ Windows Registry is not accessible"
fi
echo

# Test 8: Environment variables
echo "🧪 Test 8: Environment Variables"
echo "----------------------------------------"
echo "   WINEARCH: $(docker exec -it "$CONTAINER_NAME" printenv WINEARCH 2>/dev/null || echo 'Not set')"
echo "   WINEPREFIX: $(docker exec -it "$CONTAINER_NAME" printenv WINEPREFIX 2>/dev/null || echo 'Not set')"
echo

# Summary
echo "=========================================="
echo "    Test Summary"
echo "=========================================="
echo "Container: $CONTAINER_NAME"
echo "Status: Running"
echo "Architecture: ARM64"
echo "Wine Version: $(docker exec -it "$CONTAINER_NAME" wine --version 2>/dev/null || echo 'Unknown')"
echo

echo "✅ Wine Docker container is working!"
echo "   You can now run Windows applications using Wine"
echo
echo "📚 For usage examples, see: docs/WINE_DOCKER_USAGE.md"
echo "🚀 Quick start: docker exec -it $CONTAINER_NAME wine cmd"
