# Scripts Directory

This directory contains all build, deployment, testing, and utility scripts for the DEXArb project, organized into logical subdirectories for better maintainability.

## Directory Structure

```
scripts/
├── build/           # Build and compilation scripts
├── deploy/          # Deployment and distribution scripts
├── container/       # Docker and container management scripts
├── test/            # Testing and validation scripts
├── utils/           # Utility and helper scripts
└── README.md        # This file
```

## Quick Navigation

### 🏗️ Build Scripts
- **`build/`** - All build-related scripts including cross-platform builds and container builds
- See [build/README.md](build/README.md) for detailed information

### 🚀 Deployment Scripts
- **`deploy/`** - Binary deployment and distribution automation
- See [deploy/README.md](deploy/README.md) for detailed information

### 🐳 Container Scripts
- **`container/`** - Alpine Linux container runners and management
- See [container/README.md](container/README.md) for detailed information

### 🧪 Test Scripts
- **`test/`** - Quick testing and comprehensive platform testing
- See [test/README.md](test/README.md) for detailed information

### 🔧 Utility Scripts
- **`utils/`** - Helper scripts for blockchain scanning and data processing
- See [utils/README.md](utils/README.md) for detailed information

## Common Operations

### Setup Permissions
```bash
# Make all scripts executable
chmod +x scripts/**/*.sh

# Make specific category executable
chmod +x scripts/build/*.sh
chmod +x scripts/container/*.sh
```

### Quick Start Examples
```bash
# Build the project
./scripts/build/build-modern.sh

# Run in Alpine container
./scripts/container/run-alpine-simple.sh -- --help

# Run quick tests
./scripts/test/quick-test.sh

# Deploy binaries
./scripts/deploy/DeployBins.sh
```

## Script Categories

| Category | Purpose | Key Scripts |
|----------|---------|-------------|
| **Build** | Compilation and linking | `build-modern.sh`, `build-multi-platform.sh` |
| **Deploy** | Distribution and installation | `DeployBins.sh` |
| **Container** | Docker/container management | `run-alpine-simple.sh` |
| **Test** | Testing and validation | `quick-test.sh`, `test-all-platforms.sh` |
| **Utils** | Helper and utility functions | `scan-fantom.sh` |

## Best Practices

1. **Always run scripts from project root directory**
2. **Check script permissions before execution**
3. **Review script dependencies and requirements**
4. **Monitor script output for errors and warnings**
5. **Use appropriate script for your use case**

## Troubleshooting

- **Permission denied**: Run `chmod +x scripts/**/*.sh`
- **Script not found**: Ensure you're in the project root directory
- **Build failures**: Check dependencies and system requirements
- **Container issues**: Verify Docker installation and permissions

## Documentation

Each subdirectory contains detailed README files with:
- Script descriptions and usage examples
- Dependencies and requirements
- Troubleshooting guides
- Best practices and tips

## Contributing

When adding new scripts:
1. Place them in the appropriate subdirectory
2. Update the relevant README.md file
3. Ensure proper permissions and documentation
4. Test the script thoroughly before committing
