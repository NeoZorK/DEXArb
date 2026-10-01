# Quick Start Guide - 5 Minutes to DEXArb

Get NeoZorKDEXArb up and running in just 5 minutes using our universal build script!

## 🚀 Super Quick Start (2 minutes)

### 1. Clone and Navigate
```bash
git clone <repository-url>
cd DEXArb
```

### 2. Run Universal Build Script
```bash
# Make script executable
chmod +x scripts/build/build-universal.sh

# Interactive build (recommended for first time)
./scripts/build/build-universal.sh
```

**That's it!** The script will:
- ✅ Detect your platform automatically
- ✅ Check dependencies
- ✅ Guide you through build options
- ✅ Build the project
- ✅ Run tests (optional)
- ✅ Create packages (optional)

## 🎯 Interactive Build Experience

When you run the universal build script, you'll see:

```
🚀 Universal Build Script v1.0.7
                    NeoZorKDEXArb - DEX Arbitrage Scanner

🎯 Build Options:

1. Native build (recommended)
2. Container build (Docker)
3. Cross-platform build
4. Windows build (Wine)
5. Alpine container build
6. Ubuntu container build

Choose build method (1-6):
```

**Choose option 1** for fastest results on your system!

## ⚡ Quick Commands

### For macOS Users
```bash
./scripts/build/build-universal.sh --platform macos --build-type Release
```

### For Linux Users
```bash
./scripts/build/build-universal.sh --platform linux --clean --test
```

### For Windows Users (Wine)
```bash
./scripts/build/build-universal.sh --platform windows --verbose
```

### For Container Users
```bash
./scripts/build/build-universal.sh --platform alpine --package
```

## 🔧 What Happens During Build

1. **Platform Detection** - Automatically detects your OS
2. **Dependency Check** - Verifies required tools
3. **vcpkg Setup** - Downloads and configures dependencies
4. **CMake Configuration** - Sets up build environment
5. **Compilation** - Builds with all CPU cores
6. **Testing** - Runs C++ tests (optional)
7. **Packaging** - Creates distribution package (optional)

## 📁 Where to Find Your Build

After successful build, your executable will be in:

```
DEXArb/
├── build-macos/bin/NeoZorKDEXArb          # macOS
├── build-linux/bin/NeoZorKDEXArb          # Linux
├── build-alpine/NeoZorKDEXArb             # Alpine container
├── build-ubuntu/NeoZorKDEXArb             # Ubuntu container
└── build-windows/NeoZorKDEXArb.exe        # Windows
```

## 🚀 First Run

```bash
# Show help
./build-macos/bin/NeoZorKDEXArb -h

# Show version
./build-macos/bin/NeoZorKDEXArb -v

# Scan Ethereum (last 5000 blocks)
./build-macos/bin/NeoZorKDEXArb -scan ethereum 5000
```

## 🆘 Need Help?

### Common Issues

#### Script Not Executable
```bash
chmod +x scripts/build/build-universal.sh
```

#### Dependencies Missing
```bash
# macOS
brew install cmake git
xcode-select --install

# Ubuntu
sudo apt install build-essential cmake git

# Alpine
apk add --no-cache build-base cmake git
```

#### Build Fails
```bash
# Clean build
./scripts/build/build-universal.sh --clean

# Verbose output
./scripts/build/build-universal.sh --verbose

# Check help
./scripts/build/build-universal.sh --help
```

### Get More Help

- **Full Documentation**: [Universal Build Script Guide](UNIVERSAL_BUILD_SCRIPT.md)
- **Build Issues**: [Build and Usage Guide](BUILD_AND_USAGE.md)
- **Platform Setup**: [Platform Setup Guide](platforms/README.md)

## 🎉 You're Ready!

Congratulations! You now have:
- ✅ **Built DEXArb** for your platform
- ✅ **Learned the basics** of the universal build script
- ✅ **Ready to scan** blockchain for arbitrage opportunities

**Next steps:**
1. **Configure networks** - Set up RPC endpoints
2. **Run your first scan** - Discover DEX opportunities
3. **Explore features** - Learn advanced usage

---

**Questions? Run `./scripts/build/build-universal.sh --help` for all options!** 🚀
