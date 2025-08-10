# Quick Start: Windows in Ubuntu Wine Container

## 🚀 Get Started in 5 Minutes

### 1. Make Scripts Executable
```bash
chmod +x scripts/container/*.sh
```

### 2. Create Container with Wine
```bash
./scripts/container/run-windows-in-ubuntu-wine.sh --create
```
*This takes 5-10 minutes for initial setup*

### 3. Test Wine Installation
```bash
./scripts/container/run-windows-in-ubuntu-wine.sh --wine-test
```

### 4. Build Windows Application
```bash
./scripts/container/run-windows-in-ubuntu-wine.sh --build
```

### 5. Run Windows Application
```bash
./scripts/container/run-windows-in-ubuntu-wine.sh --run --help
```

## 🎯 What You Get

- ✅ **Ubuntu 24.04** container with Wine support
- ✅ **MinGW cross-compiler** for Windows targets
- ✅ **CMake integration** with Windows toolchain
- ✅ **Virtual display** for GUI applications
- ✅ **Full container management** (create, start, stop, delete)

## 🔧 Common Commands

```bash
# Check status
./scripts/container/run-windows-in-ubuntu-wine.sh --status

# Open shell for development
./scripts/container/run-windows-in-ubuntu-wine.sh --shell

# Force rebuild everything
./scripts/container/run-windows-in-ubuntu-wine.sh --rebuild

# Clean build artifacts
./scripts/container/run-windows-in-ubuntu-wine.sh --clean
```

## 📚 More Information

- **Full Documentation**: `docs/getting-started/WINDOWS_IN_UBUNTU_WINE.md`
- **Examples**: `scripts/container/example-usage.sh`
- **Testing**: `scripts/container/test-windows-wine.sh`
- **Help**: `./scripts/container/run-windows-in-ubuntu-wine.sh --help`

## 🚨 Prerequisites

- Apple Container CLI (`brew install --cask docker`)
- macOS with Docker support
- Internet connection for container images
- Sufficient disk space (~2-3 GB)

## 💡 Pro Tips

1. **First Run**: Use `--create` only once, then reuse container
2. **Development**: Use `--shell` for interactive development
3. **Troubleshooting**: Use `--wine-test` to diagnose issues
4. **Clean Slate**: Use `--rebuild` to start completely fresh
5. **Resource Management**: Stop container when not in use

## 🆘 Need Help?

```bash
# Show all options
./scripts/container/run-windows-in-ubuntu-wine.sh --help

# Check container status
./scripts/container/run-windows-in-ubuntu-wine.sh --status

# Test Wine installation
./scripts/container/run-windows-in-ubuntu-wine.sh --wine-test
```
