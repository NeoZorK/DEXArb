#!/bin/bash

echo "=== Complete Wine Test for DEXArb ==="
echo "Testing Wine in Ubuntu container..."

# Set Wine environment
export WINEPREFIX=/root/.wine
export WINEARCH=win64

# Test Wine version
echo "1. Testing Wine version:"
wine --version

# Test basic Windows functionality
echo "2. Testing cmd.exe:"
timeout 5s wine cmd /c "echo Hello from Windows" 2>&1 || echo "cmd.exe test completed"

# Compile simple Windows test
echo "3. Compiling simple Windows test:"
if command -v x86_64-w64-mingw32-g++ >/dev/null 2>&1; then
    echo "MinGW compiler found, compiling..."
    x86_64-w64-mingw32-g++ -o simple-windows-test.exe simple-windows-test.cpp 2>&1
    if [ $? -eq 0 ]; then
        echo "Compilation successful!"
        ls -la simple-windows-test.exe
    else
        echo "Compilation failed!"
    fi
else
    echo "MinGW compiler not found!"
fi

# Test running compiled exe
echo "4. Testing compiled exe:"
if [ -f "simple-windows-test.exe" ]; then
    echo "simple-windows-test.exe found, trying to run..."
    timeout 10s wine simple-windows-test.exe 2>&1 || echo "exe test completed"
else
    echo "simple-windows-test.exe not found"
fi

# Test existing exe
echo "5. Testing existing test-wine.exe:"
if [ -f "test-wine.exe" ]; then
    echo "test-wine.exe found, trying to run..."
    timeout 10s wine test-wine.exe 2>&1 || echo "existing exe test completed"
else
    echo "test-wine.exe not found"
fi

echo "=== Wine testing completed ==="
