# Docker Scripts

This directory contains Docker-related files for the DEXArb project.

## Files

### `Dockerfile.wine`
Dockerfile for creating a Wine-enabled container for Windows application compatibility.

### `docker-compose.wine.yml`
Docker Compose configuration for running DEXArb with Wine support.

## Usage

### Building Wine Container
```bash
cd scripts/docker
docker build -f Dockerfile.wine -t dexarb-wine .
```

### Running with Docker Compose
```bash
cd scripts/docker
docker-compose -f docker-compose.wine.yml up
```

## Wine Container Features

- Ubuntu 24.04 base image
- Wine 64-bit and 32-bit support
- MinGW cross-compiler
- CMake build system
- Virtual display support (Xvfb)

## Dependencies

- Docker Engine
- Docker Compose
- Sufficient disk space for container images

## Notes

- Wine containers are resource-intensive
- Ensure adequate memory allocation
- Use for Windows compatibility testing only
- Consider using native containers for production builds
