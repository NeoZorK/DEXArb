#!/bin/bash

# Simple script to scan Fantom blockchain
# Usage: ./scan-fantom.sh [block_count]

# Default block count if not specified
BLOCK_COUNT=${1:-5000}

# Check if block count is within valid range
if [ "$BLOCK_COUNT" -lt 1000 ] || [ "$BLOCK_COUNT" -gt 1000000 ]; then
    echo "Error: Block count must be between 1000 and 1000000"
    echo "Usage: ./scan-fantom.sh [block_count]"
    exit 1
fi

echo "Scanning Fantom blockchain for last $BLOCK_COUNT blocks..."
./build/bin/NeoZorKDEXArb -scan fantom $BLOCK_COUNT
