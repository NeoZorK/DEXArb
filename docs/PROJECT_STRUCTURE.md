# Project Structure

This document describes the organized structure of the DEXArb project after reorganization.

## Root Directory Structure

```
DEXArb/
├── docs/                    # Project documentation
├── include/                 # Header files
├── src/                     # Source code
├── tests/                   # Unit tests
├── scripts/                 # Organized scripts by functionality
├── tools/                   # Development tools
├── vcpkg/                   # Package manager
├── build-*/                 # Platform-specific build directories
├── CMakeLists.txt           # Main CMake configuration
├── README.md                # Main project documentation
└── .gitignore              # Git ignore rules
```

## Scripts Directory Organization

The `scripts/` directory has been reorganized by functionality:

### `scripts/containers/`
Container management scripts for different environments:
- Alpine Linux containers
- Ubuntu containers
- Windows containers
- Wine containers for Windows compatibility

### `scripts/docker/`
Docker-related files:
- Dockerfiles
- Docker Compose configurations
- Docker build scripts

### `scripts/build/`
Build-related scripts and configurations:
- CMake scripts
- Build configurations for different platforms
- Compilation scripts

### `scripts/testing/`
Testing scripts and executables:
- Platform-specific test scripts
- Test executables
- PowerShell and batch test scripts
- C++ test files

### `scripts/deployment/`
Deployment and distribution scripts:
- Binary deployment scripts
- Installation scripts
- Distribution utilities

### `scripts/utilities/`
Utility scripts and tools:
- Neozork configuration tools
- Scan statistics tools
- Wine-Docker utilities
- General utility scripts

## Source Code Organization

### `include/`
Header files organized by module:
- `cli/` - Command line interface
- `config/` - Configuration management
- `core/` - Core application logic
- `dex/` - DEX-related functionality
- `di/` - Dependency injection
- `interfaces/` - Abstract interfaces
- `network/` - Network communication
- `utils/` - Utility functions

### `src/`
Source code files corresponding to headers:
- Organized by module with matching structure
- Each file contains implementation for corresponding header

### `tests/`
Unit tests organized by functionality:
- `cpp/` - C++ unit tests
- `integration/` - Integration tests
- `performance/` - Performance tests

## Build System

### CMake Configuration
- `CMakeLists.txt` - Main build configuration
- `scripts/build/CMakeLists-windows.txt` - Windows-specific configuration
- `scripts/build/cmake.sh` - Main build script

### Platform Support
- Linux (x86_64, ARM64)
- macOS (Intel, Apple Silicon)
- Windows (x86_64)

## Container Support

### Container Types
- **Alpine Linux**: Lightweight, fast startup
- **Ubuntu**: Full compatibility, development environment
- **Windows**: Native Windows builds
- **Wine**: Windows compatibility in Linux

### Container Scripts
- Located in `scripts/containers/`
- Platform-specific runners
- Wine integration scripts

## Documentation

### Documentation Structure
- `docs/` - Main documentation directory
- `docs/getting-started/` - Getting started guides
- `docs/development/` - Development documentation
- `docs/reference/` - Reference materials
- `docs/reports/` - Project reports

## Tools and Utilities

### Development Tools
- `tools/` - Development and utility tools
- Neozork integration tools
- Blockchain scanning utilities

### Scripts
- All scripts organized by functionality
- Clear separation of concerns
- Comprehensive documentation for each category

## Key Benefits of Reorganization

1. **Logical Organization**: Scripts grouped by purpose, not platform
2. **Easier Navigation**: Clear structure makes finding scripts simple
3. **Better Maintenance**: Related functionality grouped together
4. **Improved Documentation**: Each category has its own README
5. **Platform Independence**: Scripts organized by function, not platform
6. **Clear Dependencies**: Each category has clear responsibilities

## Usage Guidelines

1. **Script Execution**: Always run from project root directory
2. **Permissions**: Ensure scripts have appropriate execute permissions
3. **Dependencies**: Check individual script documentation for requirements
4. **Platform**: Use appropriate scripts for your target platform
5. **Documentation**: Refer to category-specific README files for details

## Maintenance

- Keep scripts in appropriate categories
- Update documentation when adding new scripts
- Maintain consistent naming conventions
- Ensure all scripts have proper documentation
