#!/bin/bash

# Example Usage Script for Windows Wine Container
# Shows common workflows and commands
# Created by Rostyslav S. on 26.02.2025

set -euo pipefail

# Colors for output
GREEN='\033[0;32m'
BLUE='\033[0;34m'
YELLOW='\033[1;33m'
NC='\033[0m'

echo -e "${GREEN}=== Windows Wine Container - Example Usage ===${NC}"
echo ""

# Check if main script exists
SCRIPT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
MAIN_SCRIPT="$SCRIPT_DIR/run-windows-in-ubuntu-wine.sh"

if [ ! -f "$MAIN_SCRIPT" ]; then
    echo -e "${YELLOW}Error: run-windows-in-ubuntu-wine.sh not found${NC}"
    exit 1
fi

echo -e "${BLUE}Example 1: Complete Setup and Test${NC}"
echo "This workflow creates container, builds app, and runs it:"
echo ""
echo "  # Create container with Wine"
echo "  $MAIN_SCRIPT --create"
echo ""
echo "  # Test Wine installation"
echo "  $MAIN_SCRIPT --wine-test"
echo ""
echo "  # Build Windows application"
echo "  $MAIN_SCRIPT --build"
echo ""
echo "  # Run application"
echo "  $MAIN_SCRIPT --run --help"
echo ""

echo -e "${BLUE}Example 2: Development Workflow${NC}"
echo "For ongoing development work:"
echo ""
echo "  # Open interactive shell"
echo "  $MAIN_SCRIPT --shell"
echo ""
echo "  # Inside container:"
echo "  cd /workspace"
echo "  mkdir -p build-windows-wine"
echo "  cd build-windows-wine"
echo "  cmake .. -DCMAKE_SYSTEM_NAME=Windows \\"
echo "    -DCMAKE_C_COMPILER=x86_64-w64-mingw32-gcc"
echo "  make"
echo "  wine NeoZorKDEXArb.exe --help"
echo ""

echo -e "${BLUE}Example 3: Testing and Debugging${NC}"
echo "For troubleshooting and testing:"
echo ""
echo "  # Check container status"
echo "  $MAIN_SCRIPT --status"
echo ""
echo "  # Test Wine configuration"
echo "  $MAIN_SCRIPT --wine-test"
echo ""
echo "  # Configure Wine settings"
echo "  $MAIN_SCRIPT --wine-config"
echo ""
echo "  # Clean build artifacts"
echo "  $MAIN_SCRIPT --clean"
echo ""

echo -e "${BLUE}Example 4: Container Management${NC}"
echo "For container lifecycle management:"
echo ""
echo "  # Stop container"
echo "  $MAIN_SCRIPT --stop"
echo ""
echo "  # Start container (auto-starts when needed)"
echo "  $MAIN_SCRIPT --status"
echo ""
echo "  # Delete container completely"
echo "  $MAIN_SCRIPT --delete"
echo ""
echo "  # Force rebuild everything"
echo "  $MAIN_SCRIPT --rebuild"
echo ""

echo -e "${BLUE}Example 5: Running with Arguments${NC}"
echo "Pass arguments to the Windows application:"
echo ""
echo "  # Show help"
echo "  $MAIN_SCRIPT --run --help"
echo ""
echo "  # Show version"
echo "  $MAIN_SCRIPT --run --version"
echo ""
echo "  # Scan specific blockchain"
echo "  $MAIN_SCRIPT --run --scan ethereum 1000"
echo ""

echo -e "${YELLOW}Note:${NC} All commands automatically check prerequisites and provide helpful error messages."
echo ""
echo -e "${GREEN}For full help:${NC} $MAIN_SCRIPT --help"
echo -e "${GREEN}For version:${NC} $MAIN_SCRIPT --version"
