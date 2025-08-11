# Getting Started with DEXArb

Welcome to DEXArb! This guide will help you get up and running quickly with our decentralized exchange arbitrage tool.

## 🚀 Quick Start (5 minutes)

### Prerequisites
- **C++17 compatible compiler** (GCC 7+, Clang 5+, MSVC 2017+)
- **CMake 3.15+**
- **Git**

### 1. Clone and Setup
```bash
git clone <repository-url>
cd DEXArb
```

### 2. Build the Project
```bash
mkdir build && cd build
cmake ..
make -j$(nproc)
```

### 3. Run Your First Scan
```bash
./DEXArb --help
./DEXArb --scan --network ethereum
```

## 📋 What You'll Learn

This section covers:
- [**Build Instructions**](build/README.md) - Complete build guide for all platforms
- [**Platform Setup**](platforms/README.md) - Alpine, Ubuntu, and Windows/Wine configurations
- [**First Steps**](first-steps.md) - Your first arbitrage scan
- [**Configuration**](configuration.md) - Setting up networks and parameters

## 🎯 Choose Your Path

### For New Users
1. **Start here** → Read this guide
2. **Choose your platform** → [Platform Setup](platforms/README.md)
3. **Build the project** → [Build Instructions](build/README.md)
4. **Run your first scan** → [First Steps](first-steps.md)

### For Developers
1. **Quick build** → [Build Instructions](build/README.md)
2. **Development setup** → [Development Guide](../development/development-guide.md)
3. **API reference** → [API Documentation](../development/api/README.md)

### For System Administrators
1. **Production deployment** → [Deployment Guide](../deployment/README.md)
2. **Container setup** → [Container Usage](../deployment/containers/README.md)
3. **Monitoring** → [Monitoring Setup](../deployment/monitoring.md)

## 🔧 System Requirements

| Component | Minimum | Recommended |
|-----------|---------|-------------|
| **RAM** | 4 GB | 8 GB+ |
| **Storage** | 10 GB | 20 GB+ |
| **CPU** | 2 cores | 4+ cores |
| **Network** | 10 Mbps | 100 Mbps+ |

## 🌐 Supported Platforms

- **Linux**: Ubuntu 18.04+, Alpine 3.14+
- **macOS**: 10.15+ (Catalina)
- **Windows**: 10+ (via Wine or native)

## 📚 Next Steps

After completing this guide:
- Explore [**Development**](../development/README.md) for contributing
- Check [**Reference**](../reference/README.md) for technical details
- Review [**Testing**](../testing/README.md) for quality assurance
- Learn about [**Deployment**](../deployment/README.md) for production use

## 🆘 Need Help?

- **Documentation Issues**: Submit a PR to fix documentation
- **Build Problems**: Check [Build Troubleshooting](build/troubleshooting.md)
- **Runtime Errors**: See [Common Issues](troubleshooting.md)
- **Feature Requests**: Open a GitHub issue

---

*Ready to start? Choose your platform setup below!*
