#!/bin/bash

# Test script for Windows Wine Container Runner
# Demonstrates basic functionality
# Created by Rostyslav S. on 26.02.2025

set -euo pipefail

# Colors for output
GREEN='\033[0;32m'
YELLOW='\033[1;33m'
NC='\033[0m'

echo -e "${GREEN}=== Windows Wine Container Test ===${NC}"
echo "This script will test the Windows Wine container functionality"
echo ""

# Check if main script exists
if [ ! -f "run-windows-in-ubuntu-wine.sh" ]; then
    echo -e "${YELLOW}Warning: run-windows-in-ubuntu-wine.sh not found in current directory${NC}"
    echo "Please run this script from the container directory"
    exit 1
fi

# Make main script executable
chmod +x run-windows-in-ubuntu-wine.sh

echo -e "${GREEN}Step 1: Checking container status${NC}"
./run-windows-in-ubuntu-wine.sh --status

echo ""
echo -e "${GREEN}Step 2: Creating container with Wine${NC}"
echo "This will take several minutes..."
./run-windows-in-ubuntu-wine.sh --create

echo ""
echo -e "${GREEN}Step 3: Testing Wine installation${NC}"
./run-windows-in-ubuntu-wine.sh --wine-test

echo ""
echo -e "${GREEN}Step 4: Building Windows application${NC}"
echo "This will take several minutes..."
./run-windows-in-ubuntu-wine.sh --build

echo ""
echo -e "${GREEN}Step 5: Running Windows application${NC}"
./run-windows-in-ubuntu-wine.sh --run --help

echo ""
echo -e "${GREEN}Step 6: Container status${NC}"
./run-windows-in-ubuntu-wine.sh --status

echo ""
echo -e "${GREEN}=== Test completed successfully! ===${NC}"
echo ""
echo "Next steps:"
echo "  - Use --shell to open interactive shell"
echo "  - Use --run to execute the application"
echo "  - Use --clean to clean build artifacts"
echo "  - Use --delete to remove the container"
echo ""
echo "For more options: ./run-windows-in-ubuntu-wine.sh --help"
