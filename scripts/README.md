# Scripts Directory

This directory contains all build, deployment, and utility scripts:

## Build Scripts
- **build-and-deploy.sh** - Main build and deployment script
- **build-apple-container.sh** - Apple-specific container build script
- **build-modern.sh** - Modern build configuration script

## Deployment Scripts
- **DeployBins.sh** - Binary deployment script
- **quick-test.sh** - Quick testing script

## Usage
All scripts should be run from the project root directory. Make sure to:
1. Set appropriate permissions: `chmod +x scripts/*.sh`
2. Check script dependencies before running
3. Review script output for any errors

## Script Categories
- **Build**: Compilation and linking
- **Deploy**: Distribution and installation
- **Test**: Testing and validation
- **Container**: Docker/container management
