# Container Scripts

This directory contains scripts for running DEXArb in different container environments.

## Alpine Linux Containers

### `run-alpine-container.sh`
Main Alpine Linux container runner with full configuration options.

### `run-alpine-fast.sh`
Fast Alpine container runner with minimal configuration for quick testing.

### `run-alpine-simple.sh`
Simple Alpine container runner with basic configuration.

## Ubuntu Containers

### `run-ubuntu-container.sh`
Ubuntu container runner with full development environment.

## Windows Containers

### `run-windows-container.sh`
Windows container runner for native Windows builds.

### `run-windows-wine-container.sh`
Windows container with Wine for cross-platform compatibility.

### `run-windows-in-ubuntu-wine.sh`
Windows application running in Ubuntu container with Wine.

## Usage Examples

### Basic Alpine Container
```bash
./scripts/containers/run-alpine-simple.sh
```

### Windows with Wine
```bash
./scripts/containers/run-windows-in-ubuntu-wine.sh
```

### Full Ubuntu Environment
```bash
./scripts/containers/run-ubuntu-container.sh
```

## Dependencies

- Docker installed and running
- Appropriate permissions for Docker commands
- Platform-specific requirements (see individual script documentation)

## Documentation

- `README_ALPINE.md` - Alpine-specific documentation
- `README_WINDOWS_WINE.md` - Windows Wine setup
- `QUICK_START_WINDOWS_WINE.md` - Quick start guide
- `README_WINDOWS_WINE_FINAL.md` - Complete Windows Wine guide

## Testing

Use `test-windows-wine.sh` to test Windows Wine functionality.

## Notes

- All scripts should be run from the project root directory
- Ensure Docker daemon is running before executing scripts
- Check individual script documentation for specific requirements
