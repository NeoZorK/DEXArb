# Container Scripts

This directory contains scripts for managing Docker containers and containerized environments for the DEXArb project.

## Scripts Overview

### Alpine Linux Container Scripts
- **`run-alpine-simple.sh`** - Recommended: Lightweight Alpine container runner
- **`run-alpine-fast.sh`** - Optimized for speed with minimal dependencies
- **`run-alpine-container.sh`** - Full-featured Alpine container with tests

## Alpine Container Runner

### Quick Start

```bash
# Make scripts executable
chmod +x scripts/container/*.sh

# Run application in Alpine container (recommended)
./scripts/container/run-alpine-simple.sh -- --help

# Run with fast configuration
./scripts/container/run-alpine-fast.sh -- --version

# Run full container with tests
./scripts/container/run-alpine-container.sh -- --showDEXES ethereum
```

### Features

✅ **Lightweight**: ~15MB runtime dependencies  
✅ **Fast**: ~10-15s container startup  
✅ **Secure**: Minimal attack surface  
✅ **Portable**: musl libc compatibility  
✅ **Apple Silicon**: ARM64 optimized  

### Script Differences

| Script | Use Case | Startup Time | Features |
|--------|----------|--------------|----------|
| `run-alpine-simple.sh` | Development, testing | ~10s | Core app only |
| `run-alpine-fast.sh` | Quick iterations | ~8s | Minimal deps |
| `run-alpine-container.sh` | Full testing | ~15s | Tests + validation |

### Examples

```bash
# Show help
./scripts/container/run-alpine-simple.sh

# Show version
./scripts/container/run-alpine-simple.sh -- -version

# Show Ethereum DEXes
./scripts/container/run-alpine-simple.sh -- -showDEXES ethereum

# Force rebuild
./scripts/container/run-alpine-simple.sh --build
```

### Requirements

- macOS with Apple Silicon
- Apple Container CLI (`brew install --cask docker`)
- Git repository
- Sufficient disk space for container images

### Container Management

```bash
# List running containers
docker ps

# Stop all DEXArb containers
docker stop $(docker ps -q --filter ancestor=dexarb:latest)

# Remove old containers
docker container prune

# View container logs
docker logs <container_id>
```

## Troubleshooting

- **Container won't start**: Check Docker daemon status
- **Permission denied**: Ensure scripts are executable
- **Build failures**: Verify Dockerfile and dependencies
- **Performance issues**: Use `run-alpine-fast.sh` for development

## Documentation

Full documentation: `docs/getting-started/ALPINE_CONTAINER_RUNNER.md`
