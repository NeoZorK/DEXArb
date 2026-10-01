# Project Structure

Comprehensive overview of the DEXArb project structure, architecture, and organization.

## 🏗️ Project Overview

DEXArb is a decentralized exchange arbitrage scanner designed to identify profitable trading opportunities across multiple DEX platforms. The project follows a modular, interface-based architecture for maintainability and extensibility.

## 📁 Directory Structure

```
DEXArb/
├── src/                    # Source code
│   ├── cli/               # Command-line interface
│   ├── config/            # Configuration management
│   ├── core/              # Core arbitrage logic
│   ├── dex/               # DEX platform integrations
│   ├── di/                # Dependency injection
│   ├── main.cpp           # Application entry point
│   ├── network/           # Network communication
│   └── utils/             # Utility functions
├── include/                # Header files
│   ├── cli/               # CLI headers
│   ├── config/            # Configuration headers
│   ├── core/              # Core logic headers
│   ├── dex/               # DEX integration headers
│   ├── di/                # DI container headers
│   ├── interfaces/        # Abstract interfaces
│   ├── main.h             # Main application header
│   ├── network/           # Network headers
│   └── utils/             # Utility headers
├── tests/                  # Test suites
│   ├── cpp/               # C++ unit tests
│   ├── integration/       # Integration tests
│   └── performance/       # Performance tests
├── docs/                   # Documentation
├── scripts/                # Build and utility scripts
├── tools/                  # Development tools
├── vcpkg/                  # Package management
└── CMakeLists.txt          # Build configuration
```

## 🏛️ Architecture Overview

### Core Components
```
┌─────────────────────────────────────────────────────────┐
│                    DEXArb Architecture                  │
├─────────────────────────────────────────────────────────┤
│  ┌─────────────┐  ┌─────────────┐  ┌─────────────┐    │
│  │   CLI Layer │  │  Core Engine│  │  Network    │    │
│  │             │  │             │  │   Layer     │    │
│  │ • Parser    │◄─┤ • Arbitrage│◄─┤ • RPC       │    │
│  │ • Help      │  │ • Scanner  │  │ • HTTP      │    │
│  │ • Output    │  │ • Analyzer │  │ • WebSocket │    │
│  └─────────────┘  └─────────────┘  └─────────────┘    │
│           │               │               │            │
│           ▼               ▼               ▼            │
│  ┌─────────────┐  ┌─────────────┐  ┌─────────────┐    │
│  │  Config     │  │   DEX       │  │  Utilities  │    │
│  │ Management  │  │  Platforms  │  │             │    │
│  │             │  │             │  │ • Modern    │    │
│  │ • Loader    │  │ • Scanner   │  │ • Format    │    │
│  │ • Validator │  │ • Pools     │  │ • Platform  │    │
│  │ • Reload    │  │ • Tokens    │  │ • Results   │    │
│  └─────────────┘  └─────────────┘  └─────────────┘    │
└─────────────────────────────────────────────────────────┘
```

### Data Flow
1. **Configuration Loading** - Load and validate configuration
2. **Network Connection** - Establish RPC connections
3. **DEX Scanning** - Scan DEX platforms for pools
4. **Arbitrage Analysis** - Calculate profit opportunities
5. **Result Output** - Format and display results

## 🔧 Core Components

### CLI Layer (`src/cli/`)
- **Command Parser** - Parse command-line arguments
- **Help Display** - Show help information
- **Input/Output** - Handle user input and output

### Configuration Management (`src/config/`)
- **Config Manager** - Load and manage configuration
- **Environment Variables** - Handle environment overrides
- **Validation** - Validate configuration values
- **Hot Reload** - Reload configuration without restart

### Core Engine (`src/core/`)
- **Arbitrage Engine** - Core arbitrage logic
- **Profit Analyzer** - Calculate profit opportunities
- **Risk Manager** - Assess and manage risks
- **Pool Scanner** - Discover and analyze pools

### DEX Integrations (`src/dex/`)
- **DEX Scanner** - Platform-agnostic scanning
- **Pool Manager** - Manage pool information
- **Token Manager** - Handle token data
- **Stats Engine** - Calculate platform statistics

### Network Layer (`src/network/`)
- **RPC Client** - Blockchain RPC communication
- **HTTP Client** - REST API communication
- **WebSocket Client** - Real-time data streaming
- **Connection Pool** - Manage network connections

### Utilities (`src/utils/`)
- **Modern Utils** - Modern C++ utilities
- **Format Utils** - Data formatting utilities
- **Platform Utils** - Platform-specific utilities
- **Result Utils** - Error handling utilities

## 🔌 Interface Design

### Abstract Interfaces (`include/interfaces/`)
```cpp
// RPC Client Interface
class IRPCClient {
public:
    virtual ~IRPCClient() = default;
    virtual Result<std::string> call(const std::string& method, const json& params) = 0;
    virtual bool connect() = 0;
    virtual void disconnect() = 0;
};

// Configuration Manager Interface
class IConfigManager {
public:
    virtual ~IConfigManager() = default;
    virtual bool loadConfig(const std::string& path) = 0;
    virtual void reloadConfig() = 0;
    virtual json getConfig() const = 0;
};

// DEX Scanner Interface
class IDEXScanner {
public:
    virtual ~IDEXScanner() = default;
    virtual std::vector<Pool> scanPools(const Network& network) = 0;
    virtual std::vector<Pool> scanPoolsForTokens(const std::vector<Token>& tokens) = 0;
};
```

### Implementation Classes
- **HTTPClient** - Implements IRPCClient for HTTP
- **ConfigManager** - Implements IConfigManager
- **DEXScanner** - Implements IDEXScanner
- **ArbitrageEngine** - Core arbitrage implementation

## 🧪 Testing Structure

### Test Organization
```
tests/
├── cpp/                    # C++ unit tests
│   ├── core/              # Core component tests
│   ├── dex/               # DEX integration tests
│   ├── network/           # Network layer tests
│   ├── utils/             # Utility function tests
│   └── main.cpp           # Test runner
├── integration/            # Integration tests
│   ├── end_to_end/        # Complete workflow tests
│   ├── component/          # Component interaction tests
│   └── performance/        # Performance tests
└── data/                  # Test data and fixtures
```

### Testing Framework
- **Google Test** - Unit testing framework
- **Google Mock** - Mocking framework
- **CTest** - CMake test integration
- **Coverage** - Code coverage analysis

## 🚀 Build System

### CMake Configuration
```cmake
# Main CMakeLists.txt structure
cmake_minimum_required(VERSION 3.15)
project(DEXArb VERSION 2.0.0)

# Set C++ standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Options
option(ENABLE_TESTS "Enable tests" ON)
option(ENABLE_DOCS "Enable documentation" OFF)
option(ENABLE_EXAMPLES "Enable examples" OFF)

# Dependencies
find_package(PkgConfig REQUIRED)
find_package(OpenSSL REQUIRED)
find_package(CURL REQUIRED)
find_package(nlohmann_json REQUIRED)

# Subdirectories
add_subdirectory(src)
if(ENABLE_TESTS)
    add_subdirectory(tests)
endif()
```

### Build Variants
- **Debug** - Development and testing
- **Release** - Production deployment
- **RelWithDebInfo** - Release with debug information
- **MinSizeRel** - Minimal size release

## 📊 Data Models

### Core Data Structures
```cpp
// Token representation
struct Token {
    std::string symbol;
    std::string address;
    uint8_t decimals;
    std::string name;
    bool isStablecoin;
};

// Pool representation
struct Pool {
    std::string platform;
    TokenPair tokens;
    double liquidity;
    double price;
    double volume24h;
    double fee;
    std::string address;
};

// Arbitrage opportunity
struct ArbitrageOpportunity {
    Pool buyPool;
    Pool sellPool;
    double profit;
    double gasCost;
    double netProfit;
    double amount;
    RiskLevel risk;
    std::chrono::system_clock::time_point timestamp;
};
```

### Configuration Models
```cpp
// Network configuration
struct NetworkConfig {
    std::string name;
    std::string rpcUrl;
    uint32_t chainId;
    uint32_t scanInterval;
    double maxGasPrice;
    uint32_t timeout;
    bool enabled;
};

// Scanning configuration
struct ScanningConfig {
    bool enabled;
    std::string mode;
    uint32_t scanInterval;
    double minProfitThreshold;
    double maxSlippage;
    uint32_t batchSize;
    uint32_t maxConcurrent;
    uint32_t timeout;
};
```

## 🔒 Security Architecture

### Security Layers
1. **Input Validation** - Sanitize all external input
2. **Authentication** - API key and access control
3. **Encryption** - TLS/SSL for network communication
4. **Access Control** - Permission-based access
5. **Audit Logging** - Comprehensive operation logging

### Security Features
- **API Key Management** - Secure storage and rotation
- **Rate Limiting** - Prevent abuse and DoS
- **Input Sanitization** - Prevent injection attacks
- **Memory Safety** - Bounds checking and validation

## 📈 Performance Architecture

### Performance Features
- **Asynchronous I/O** - Non-blocking operations
- **Connection Pooling** - Reuse network connections
- **Caching** - Cache frequently accessed data
- **Batch Processing** - Process multiple items together
- **Parallel Processing** - Multi-threaded operations

### Optimization Strategies
- **Memory Pooling** - Reduce allocation overhead
- **SIMD Instructions** - Vectorized operations
- **Profile-Guided Optimization** - Compiler optimizations
- **Link-Time Optimization** - Cross-module optimization

## 🔄 Dependency Management

### External Dependencies
- **OpenSSL** - Cryptographic functions
- **CURL** - HTTP client library
- **nlohmann/json** - JSON parsing
- **CMake** - Build system
- **Google Test** - Testing framework

### Internal Dependencies
- **Modern Utils** - Core utility functions
- **Result Types** - Error handling utilities
- **Platform Utils** - Platform-specific utilities
- **Format Utils** - Data formatting utilities

## 🚀 Deployment Architecture

### Deployment Models
1. **Single Server** - Standalone deployment
2. **Multi-Server** - Load-balanced deployment
3. **Containerized** - Docker-based deployment
4. **Cloud-Native** - Kubernetes deployment

### Configuration Management
- **Environment Variables** - Runtime configuration
- **Configuration Files** - Persistent configuration
- **Hot Reload** - Dynamic configuration updates
- **Validation** - Configuration validation

## 📚 Documentation Structure

### Documentation Organization
```
docs/
├── README.md               # Main documentation
├── getting-started/        # User onboarding
├── development/            # Developer resources
├── reference/              # Technical reference
├── testing/                # Testing guides
├── deployment/             # Deployment guides
└── api/                    # API documentation
```

### Documentation Standards
- **Markdown Format** - Consistent formatting
- **Code Examples** - Practical examples
- **Cross-References** - Link between documents
- **Version Tracking** - Document versioning

## 🔍 Monitoring and Observability

### Monitoring Components
- **Metrics Collection** - Performance metrics
- **Health Checks** - Application health monitoring
- **Logging** - Structured logging system
- **Tracing** - Request tracing and debugging

### Observability Features
- **Prometheus Metrics** - Time-series metrics
- **Structured Logging** - JSON-formatted logs
- **Distributed Tracing** - Request flow tracking
- **Alerting** - Automated alerting system

## 🎯 Future Architecture

### Planned Enhancements
1. **Microservices** - Service decomposition
2. **Event-Driven Architecture** - Asynchronous processing
3. **GraphQL API** - Modern API interface
4. **Machine Learning** - ML-powered analysis
5. **Blockchain Integration** - Direct blockchain interaction

### Scalability Improvements
- **Horizontal Scaling** - Multi-instance deployment
- **Database Integration** - Persistent data storage
- **Message Queues** - Asynchronous processing
- **Caching Layer** - Distributed caching

---

**Architecture Status**: ✅ **STABLE**  
**Last Updated**: March 2025  
**Next Review**: Quarterly architecture review
