# DEXArb Project Structure

## Overview
DEXArb is a high-performance DEX Arbitrage Scanner organized with a modular, maintainable architecture.

## Directory Structure

```
DEXArb/
├── include/                    # Header files organized by functionality
│   ├── core/                  # Core arbitrage and blockchain logic
│   ├── dex/                   # DEX-related functionality
│   ├── cli/                   # Command-line interface
│   ├── network/               # Network and RPC functionality
│   ├── utils/                 # Utility functions and helpers
│   ├── config/                # Configuration management
│   └── di/                    # Dependency injection
├── src/                       # Source files matching include structure
│   ├── core/                  # Core implementation
│   ├── dex/                   # DEX implementation
│   ├── cli/                   # CLI implementation
│   ├── network/               # Network implementation
│   ├── utils/                 # Utils implementation
│   ├── config/                # Config implementation
│   └── di/                    # DI implementation
├── tests/                     # Test files organized by type
│   ├── cpp/                   # C++ unit tests
│   ├── integration/           # Integration tests
│   └── performance/           # Performance tests
├── docs/                      # Project documentation
├── scripts/                   # Build and deployment scripts
├── build/                     # Build configuration files
├── tools/                     # Development and utility tools
└── vcpkg/                     # Package management
```

## Module Organization

### Core Module (`core/`)
- **arbitrage.h/cpp** - Core arbitrage algorithms and logic
- **blockchain.h/cpp** - Blockchain interaction and data structures
- **profit_analyzer.h/cpp** - Profit calculation and analysis
- **application.h/cpp** - Main application class

### DEX Module (`dex/`)
- **dex_pools.h/cpp** - DEX pool management and operations
- **dex_scanner.h/cpp** - DEX scanning and discovery
- **dex_stats.h/cpp** - DEX statistics and metrics
- **dex_tokens.h/cpp** - Token management and operations

### CLI Module (`cli/`)
- **command_parser.h/cpp** - Command line argument parsing
- **help_display.h/cpp** - Help and usage information

### Network Module (`network/`)
- **rpc_core.h/cpp** - RPC client implementation
- **queries.h/cpp** - Network query definitions

### Utils Module (`utils/`)
- **measure.h/cpp** - Performance measurement utilities
- **modern_format.h** - Modern data formatting (header-only)
- **modern_result.h** - Result handling and formatting (header-only)
- **input.h/cpp** - Input processing utilities
- **wallet.h/cpp** - Wallet management

### Config Module (`config/`)
- **config_manager.h/cpp** - Configuration management

### DI Module (`di/`)
- **service_container.h/cpp** - Dependency injection container

## Benefits of This Structure

1. **Logical Grouping**: Related functionality is grouped together
2. **Maintainability**: Easier to find and modify specific features
3. **Scalability**: New modules can be added without affecting existing code
4. **Testing**: Each module can be tested independently
5. **Documentation**: Clear organization makes code easier to understand
6. **Build System**: CMake configuration is cleaner and more organized

## File Naming Conventions

- **Headers**: `module_name.h` (snake_case)
- **Sources**: `module_name.cpp` (snake_case)
- **Directories**: `module_name/` (snake_case)
- **Tests**: `test_module_name.cpp` (snake_case)

## Build Configuration

- **CMakeLists.txt** - Main build configuration
- **build/CMakePresets.json** - Build presets for different configurations
- **build/vcpkg.json** - Package dependencies
- **build/Containerfile** - Container build definition

## Development Workflow

1. **New Features**: Add to appropriate module in `include/` and `src/`
2. **Testing**: Create tests in `tests/cpp/` following naming convention
3. **Documentation**: Update relevant README files
4. **Build**: Use scripts in `scripts/` directory
5. **Deploy**: Use deployment scripts for distribution

## Testing Strategy

- **Unit Tests**: Each module has corresponding test files
- **Integration Tests**: Test module interactions
- **Performance Tests**: Benchmark critical paths
- **Coverage**: Aim for 100% test coverage on new features

## Dependencies

- **C++23**: Modern C++ standard for best performance
- **vcpkg**: Package management for external libraries
- **CMake**: Build system configuration
- **Testing**: Google Test framework for unit testing
