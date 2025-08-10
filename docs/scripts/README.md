# Scripts Documentation

This directory contains comprehensive documentation for all scripts in the DEXArb project.

## Overview

The scripts directory has been reorganized into logical subdirectories to improve maintainability and ease of use. Each category of scripts serves a specific purpose in the development and deployment workflow.

## Directory Structure

```
scripts/
├── build/           # Build and compilation scripts
├── deploy/          # Deployment and distribution scripts  
├── container/       # Docker and container management scripts
├── test/            # Testing and validation scripts
├── utils/           # Utility and helper scripts
└── README.md        # Main scripts overview
```

## Script Categories

### 🏗️ Build Scripts (`build/`)
Scripts responsible for compiling and building the project across different platforms and configurations.

**Key Scripts:**
- `build-modern.sh` - Modern build configuration with optimizations
- `build-multi-platform.sh` - Cross-platform compilation
- `build-apple-container.sh` - Apple Silicon container builds
- `build-alpine-arm64.sh` - Alpine Linux ARM64 builds

**Use Cases:**
- Development builds
- Release builds
- Cross-platform compilation
- Container image building

### 🚀 Deployment Scripts (`deploy/`)
Scripts for deploying and distributing compiled binaries to various environments.

**Key Scripts:**
- `DeployBins.sh` - Main deployment automation

**Use Cases:**
- Production deployments
- Package distribution
- Binary releases
- Environment management

### 🐳 Container Scripts (`container/`)
Scripts for managing Docker containers and containerized development environments.

**Key Scripts:**
- `run-alpine-simple.sh` - Recommended lightweight container runner
- `run-alpine-fast.sh` - Fast development container
- `run-alpine-container.sh` - Full-featured container with tests

**Use Cases:**
- Development environment isolation
- Cross-platform compatibility testing
- Consistent build environments
- Quick iteration cycles

### 🧪 Test Scripts (`test/`)
Scripts for testing and validating the project across different scenarios and platforms.

**Key Scripts:**
- `quick-test.sh` - Fast development testing
- `test-all-platforms.sh` - Comprehensive platform testing

**Use Cases:**
- Development testing
- Release validation
- Platform compatibility testing
- Performance benchmarking

### 🔧 Utility Scripts (`utils/`)
Helper scripts for various development and operational tasks.

**Key Scripts:**
- `scan-fantom.sh` - Fantom blockchain scanning utility

**Use Cases:**
- Blockchain data analysis
- Network monitoring
- Data processing
- Development automation

## Common Workflows

### Development Workflow
1. **Build**: Use `scripts/build/build-modern.sh`
2. **Test**: Use `scripts/test/quick-test.sh`
3. **Container**: Use `scripts/container/run-alpine-simple.sh`
4. **Iterate**: Repeat as needed

### Release Workflow
1. **Build All Platforms**: Use `scripts/build/build-multi-platform.sh`
2. **Test All Platforms**: Use `scripts/test/test-all-platforms.sh`
3. **Deploy**: Use `scripts/deploy/DeployBins.sh`

### Container Development
1. **Quick Start**: Use `scripts/container/run-alpine-simple.sh`
2. **Full Testing**: Use `scripts/container/run-alpine-container.sh`
3. **Custom Builds**: Use `scripts/build/build-apple-container.sh`

## Best Practices

### Script Usage
- Always run scripts from the project root directory
- Check script permissions before execution
- Review script dependencies and requirements
- Monitor script output for errors

### Development
- Use appropriate scripts for your use case
- Test scripts thoroughly before committing
- Maintain comprehensive documentation
- Follow consistent naming conventions

### Maintenance
- Keep scripts up to date with project changes
- Regular review and optimization
- Monitor script performance
- Update documentation as needed

## Troubleshooting

### Common Issues
- **Permission denied**: Run `chmod +x scripts/**/*.sh`
- **Script not found**: Ensure you're in the project root
- **Build failures**: Check dependencies and requirements
- **Container issues**: Verify Docker installation

### Getting Help
- Check individual script README files
- Review script output and error messages
- Consult project documentation
- Check system requirements and dependencies

## Contributing

When adding new scripts:
1. Place them in the appropriate subdirectory
2. Update relevant README files
3. Ensure proper permissions and documentation
4. Test thoroughly before committing
5. Follow established naming conventions

## Related Documentation

- [Getting Started Guide](../getting-started/README.md)
- [Build Documentation](../getting-started/BUILD_AND_USAGE.md)
- [Container Runner Guide](../getting-started/ALPINE_CONTAINER_RUNNER.md)
- [Project Structure](../../PROJECT_STRUCTURE.md)
