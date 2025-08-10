# Alpine Linux Container Runner

## Quick Start

```bash
# Make script executable
chmod +x scripts/run-alpine-simple.sh

# Run application in Alpine container
./scripts/run-alpine-simple.sh -- --help
```

## Scripts

- **`run-alpine-simple.sh`** - Recommended: Builds and runs main app only
- **`run-alpine-container.sh`** - Full build with tests (may have issues)

## Features

✅ **Lightweight**: ~15MB runtime dependencies  
✅ **Fast**: ~10-15s container startup  
✅ **Secure**: Minimal attack surface  
✅ **Portable**: musl libc compatibility  
✅ **Apple Silicon**: ARM64 optimized  

## Examples

```bash
# Show help
./scripts/run-alpine-simple.sh

# Show version
./scripts/run-alpine-simple.sh -- -version

# Show Ethereum DEXes
./scripts/run-alpine-simple.sh -- -showDEXES ethereum

# Force rebuild
./scripts/run-alpine-simple.sh --build
```

## Requirements

- macOS with Apple Silicon
- Apple Container CLI (`brew install --cask docker`)
- Git repository

## Documentation

Full documentation: `docs/getting-started/ALPINE_CONTAINER_RUNNER.md`
