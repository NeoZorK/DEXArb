#!/bin/bash

echo "Testing Wine in Ubuntu container..."

# Set Wine environment
export WINEPREFIX=/root/.wine
export WINEARCH=win64

# Test Wine version
echo "Wine version:"
wine --version

# Test if we can run cmd.exe
echo "Testing cmd.exe:"
timeout 5s wine cmd /c "echo Hello from Windows" 2>&1 || echo "cmd.exe test completed"

# Test if we can run our compiled exe
echo "Testing compiled exe:"
if [ -f "test-wine.exe" ]; then
    echo "test-wine.exe found, trying to run..."
    timeout 5s wine test-wine.exe 2>&1 || echo "exe test completed"
else
    echo "test-wine.exe not found"
fi

echo "Wine testing completed"
