# Scripts Directory

This directory contains all scripts organized by functionality for the DEXArb project.

## Directory Structure

### `containers/`
Contains scripts for running different container environments:
- Alpine Linux containers
- Ubuntu containers  
- Windows containers
- Wine containers for Windows compatibility

### `docker/`
Contains Docker-related files:
- Dockerfiles
- Docker Compose configurations
- Docker build scripts

### `build/`
Contains build-related scripts and configurations:
- CMake scripts
- Build configurations for different platforms
- Compilation scripts

### `testing/`
Contains testing scripts and executables:
- Platform-specific test scripts
- Test executables
- PowerShell and batch test scripts
- C++ test files

### `deployment/`
Contains deployment and distribution scripts:
- Binary deployment scripts
- Installation scripts
- Distribution utilities

### `utilities/`
Contains utility scripts and tools:
- Neozork configuration tools
- Scan statistics tools
- Wine-Docker utilities
- General utility scripts

## Usage

Each subdirectory contains scripts specific to its purpose. Refer to individual README files in each subdirectory for detailed usage instructions.

## Organization Principles

- **Separation of Concerns**: Each directory has a specific purpose
- **Platform Independence**: Scripts are organized by functionality, not platform
- **Maintainability**: Clear structure makes it easier to find and maintain scripts
- **Documentation**: Each directory has its own README with usage examples
