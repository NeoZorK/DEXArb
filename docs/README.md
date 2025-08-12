# DEXArb Documentation

Welcome to the DEXArb project documentation. This project is a decentralized exchange arbitrage tool designed to identify and execute profitable trading opportunities across multiple DEX platforms.

## 📚 Documentation Structure

### 🚀 Getting Started
- **[Quick Start Guide](getting-started/README.md)** - Get up and running in minutes
- **[Universal Build Script](getting-started/UNIVERSAL_BUILD_SCRIPT.md)** - One script for all platforms
- **[Build Instructions](getting-started/build/README.md)** - Complete build guide for all platforms
- **[Platform Setup](getting-started/platforms/README.md)** - Setup for Alpine, Ubuntu, and Windows/Wine
- **[Windows/Wine Setup](getting-started/platforms/windows-wine.md)** - Windows development with Wine
- **[Wine Docker Usage](getting-started/platforms/wine-docker.md)** - Wine in Docker containers
- **[Wine Setup Guide](getting-started/platforms/wine-setup.md)** - Comprehensive Wine configuration
- **[Windows/Wine Summary](getting-started/platforms/windows-wine-summary.md)** - Implementation overview
- **[First Steps](getting-started/first-steps.md)** - Your first arbitrage scan
- **[Configuration](getting-started/configuration.md)** - Setting up networks and parameters

### 🏗️ Development
- **[Development Guide](development/README.md)** - How to contribute and develop
- **[Project Structure](development/project-structure.md)** - Codebase organization and architecture
- **[API Reference](development/api/README.md)** - Core API documentation
- **[Testing Guide](development/testing/README.md)** - Testing strategies and procedures

### 📖 Reference
- **[Reference Overview](reference/README.md)** - Technical documentation and APIs
- **[Wine Reference](reference/wine-reference.md)** - Comprehensive Wine usage guide
- **[Configuration Reference](reference/configuration.md)** - All configuration options
- **[CLI Reference](reference/cli/README.md)** - Command-line interface documentation
- **[Network Protocols](reference/network/README.md)** - RPC and network communication

### 🧪 Testing & Quality
- **[Testing Overview](testing/README.md)** - Testing framework and procedures
- **[Test Reports](testing/reports/README.md)** - Current test status and results
- **[Performance Testing](testing/performance/README.md)** - Performance benchmarks and analysis

### 🚢 Deployment & Operations
- **[Deployment Guide](deployment/README.md)** - Production deployment instructions
- **[Multi-Platform Plan](deployment/multi-platform-plan.md)** - Cross-platform implementation strategy
- **[Multi-Platform Build](deployment/multi-platform-build.md)** - Building for all platforms
- **[Container Usage](deployment/containers/README.md)** - Docker and container management
- **[Monitoring](deployment/monitoring.md)** - Logging and monitoring setup

## 🎯 Quick Navigation

| Section | Description | Key Files |
|---------|-------------|-----------|
| **Getting Started** | New user onboarding | `getting-started/README.md` |
| **Development** | Developer resources | `development/README.md` |
| **Reference** | Technical details | `reference/README.md` |
| **Testing** | Quality assurance | `testing/README.md` |
| **Deployment** | Production setup | `deployment/README.md` |

## 🔧 Project Overview

DEXArb is a C++ application that:
- Scans multiple DEX platforms for arbitrage opportunities
- Analyzes profit potential and transaction costs
- Provides real-time market data and statistics
- Supports multiple blockchain networks
- Offers both CLI and programmatic interfaces

## 🚀 Quick Start

1. **Clone the repository**
   ```bash
   git clone <repository-url>
   cd DEXArb
   ```

2. **Use Universal Build Script (Recommended)**
   ```bash
   # Make script executable
   chmod +x scripts/build/build-universal.sh
   
   # Interactive build
   ./scripts/build/build-universal.sh
   
   # Or quick build for your platform
   ./scripts/build/build-universal.sh --platform macos --build-type Release
   ```

3. **Alternative: Platform-specific setup**
   - [Alpine Linux](getting-started/platforms/README.md#alpine-linux-314)
   - [Ubuntu](getting-started/platforms/README.md#ubuntu-1804--debian-10)
   - [Windows/Wine](getting-started/platforms/windows-wine.md)

## 🌐 Supported Platforms

- **Linux**: Ubuntu 18.04+, Alpine 3.14+, CentOS 7+
- **macOS**: 10.15+ (Catalina), Apple Silicon support
- **Windows**: 10+ (via Wine and native builds)
- **Containers**: Docker-based deployment

## 📝 Contributing

We welcome contributions! Please see our [Development Guide](development/README.md) for details on:
- Setting up your development environment
- Code style and standards
- Testing requirements
- Pull request process

## 🆘 Support

- **Documentation Issues**: Submit a PR to fix documentation
- **Build Problems**: Check [Universal Build Script Guide](getting-started/UNIVERSAL_BUILD_SCRIPT.md#troubleshooting)
- **Runtime Errors**: See [Common Issues](getting-started/first-steps.md#troubleshooting)
- **Feature Requests**: Open a GitHub issue

## 📄 License

[Add your license information here]

---

*Last updated: March 2025*  
*Documentation version: 2.0*  
*Status: ✅ **REORGANIZATION COMPLETE***
