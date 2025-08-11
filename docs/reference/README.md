# Reference Documentation

Welcome to the DEXArb reference documentation. This section provides comprehensive technical details about the project structure, APIs, and implementation details.

## 📚 What You'll Find Here

This section covers:
- [**Project Structure**](project-structure.md) - Codebase organization and architecture
- [**Configuration Reference**](configuration.md) - All configuration options and parameters
- [**CLI Reference**](cli/README.md) - Command-line interface documentation
- [**Network Protocols**](network/README.md) - RPC and network communication details

## 🏗️ Architecture Overview

### System Components
```
┌─────────────────┐    ┌─────────────────┐    ┌─────────────────┐
│   CLI Layer     │    │  Core Engine    │    │  Network Layer  │
│                 │    │                 │    │                 │
│ • Command Parser│◄──►│ • Arbitrage     │◄──►│ • RPC Client    │
│ • Help Display  │    │ • Profit Calc   │    │ • HTTP Client   │
│ • Input/Output  │    │ • Pool Scanner  │    │ • WebSocket     │
└─────────────────┘    └─────────────────┘    └─────────────────┘
         │                       │                       │
         │                       │                       │
         ▼                       ▼                       ▼
┌─────────────────┐    ┌─────────────────┐    ┌─────────────────┐
│  Configuration  │    │   DEX Layer     │    │   Utilities     │
│                 │    │                 │    │                 │
│ • Config Manager│    │ • Pool Scanner  │    │ • Modern Utils  │
│ • Environment   │    │ • Token Manager │    │ • Format Utils  │
│ • Validation    │    │ • Stats Engine  │    │ • Platform Utils│
└─────────────────┘    └─────────────────┘    └─────────────────┘
```

### Data Flow
1. **Input Processing** - CLI commands and configuration
2. **Network Connection** - RPC endpoint establishment
3. **Pool Discovery** - DEX pool scanning and enumeration
4. **Arbitrage Analysis** - Profit calculation and opportunity identification
5. **Result Output** - Formatted results and logging

## 🔧 Core Classes and Interfaces

### Application Core
```cpp
class Application {
public:
    // Main application lifecycle
    int run(int argc, char* argv[]);
    void initialize();
    void shutdown();
    
    // Configuration management
    void loadConfig(const std::string& configPath);
    void reloadConfig();
    
private:
    std::unique_ptr<ConfigManager> configManager;
    std::unique_ptr<ArbitrageEngine> arbitrageEngine;
    std::unique_ptr<DEXScanner> dexScanner;
};
```

### Arbitrage Engine
```cpp
class ArbitrageEngine {
public:
    // Core arbitrage logic
    std::vector<ArbitrageOpportunity> findOpportunities(
        const std::vector<Pool>& pools,
        double minProfitThreshold = 0.5
    );
    
    // Profit calculation
    double calculateProfit(const Pool& pool1, const Pool& pool2, double amount);
    double calculateGasCost(const Transaction& tx, double gasPrice);
    
    // Risk assessment
    RiskAssessment assessRisk(const ArbitrageOpportunity& opportunity);
    
private:
    std::unique_ptr<ProfitAnalyzer> profitAnalyzer;
    std::unique_ptr<RiskManager> riskManager;
};
```

### DEX Scanner
```cpp
class DEXScanner {
public:
    // Pool discovery
    std::vector<Pool> scanPools(const Network& network);
    std::vector<Pool> scanPoolsForTokens(
        const Network& network,
        const std::vector<Token>& tokens
    );
    
    // Platform-specific scanning
    std::vector<Pool> scanUniswapV2(const Network& network);
    std::vector<Pool> scanUniswapV3(const Network& network);
    std::vector<Pool> scanSushiSwap(const Network& network);
    
private:
    std::unique_ptr<RPCClient> rpcClient;
    std::map<std::string, std::unique_ptr<DEXPlatform>> platforms;
};
```

## 📊 Data Structures

### Core Entities
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

### Configuration Structures
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

## 🌐 Network Communication

### RPC Client Interface
```cpp
class IRPCClient {
public:
    virtual ~IRPCClient() = default;
    
    // Basic RPC operations
    virtual Result<std::string> call(const std::string& method, const json& params) = 0;
    virtual Result<json> callJSON(const std::string& method, const json& params) = 0;
    
    // Batch operations
    virtual Result<std::vector<json>> batchCall(const std::vector<RPCRequest>& requests) = 0;
    
    // Connection management
    virtual bool connect() = 0;
    virtual void disconnect() = 0;
    virtual bool isConnected() const = 0;
};
```

### HTTP Client Implementation
```cpp
class HTTPClient : public IRPCClient {
public:
    // HTTP-specific methods
    void setHeaders(const std::map<std::string, std::string>& headers);
    void setTimeout(uint32_t timeout);
    void setRetryPolicy(const RetryPolicy& policy);
    
private:
    // Implementation details
    std::unique_ptr<cpr::Session> session;
    std::map<std::string, std::string> defaultHeaders;
    RetryPolicy retryPolicy;
};
```

## ⚙️ Configuration Management

### Configuration Hierarchy
```
1. Command-line arguments (highest priority)
2. Environment variables
3. User configuration file (~/.config/dexarb/config.json)
4. System configuration file (/etc/dexarb/config.json)
5. Default values (lowest priority)
```

### Configuration Validation
```cpp
class ConfigValidator {
public:
    // Validation methods
    ValidationResult validate(const Config& config);
    ValidationResult validateNetwork(const NetworkConfig& network);
    ValidationResult validateScanning(const ScanningConfig& scanning);
    
    // Schema validation
    bool validateSchema(const json& config, const json& schema);
    
private:
    // Validation rules
    std::vector<ValidationRule> rules;
};
```

## 🔍 Logging and Monitoring

### Logging System
```cpp
class Logger {
public:
    // Log levels
    enum class Level { TRACE, DEBUG, INFO, WARN, ERROR, FATAL };
    
    // Logging methods
    void trace(const std::string& message);
    void debug(const std::string& message);
    void info(const std::string& message);
    void warn(const std::string& message);
    void error(const std::string& message);
    void fatal(const std::string& message);
    
    // Configuration
    void setLevel(Level level);
    void setOutput(const std::string& output);
    void setFormat(const std::string& format);
};
```

### Metrics Collection
```cpp
class MetricsCollector {
public:
    // Performance metrics
    void recordScanTime(const std::string& network, double duration);
    void recordPoolCount(const std::string& network, size_t count);
    void recordOpportunityCount(const std::string& network, size_t count);
    
    // System metrics
    void recordMemoryUsage(size_t bytes);
    void recordCPUUsage(double percentage);
    void recordNetworkLatency(const std::string& endpoint, double latency);
    
    // Export metrics
    json exportMetrics() const;
    void exportPrometheus(std::ostream& output) const;
};
```

## 🧪 Testing Framework

### Test Structure
```cpp
// Test base class
class TestBase : public ::testing::Test {
protected:
    void SetUp() override;
    void TearDown() override;
    
    // Test utilities
    std::unique_ptr<TestNetwork> createTestNetwork();
    std::vector<Pool> createMockPools();
    ArbitrageOpportunity createMockOpportunity();
};
```

### Mock Objects
```cpp
// Mock RPC client
class MockRPCClient : public IRPCClient {
public:
    MOCK_METHOD(Result<std::string>, call, (const std::string&, const json&), (override));
    MOCK_METHOD(Result<json>, callJSON, (const std::string&, const json&), (override));
    MOCK_METHOD(bool, connect, (), (override));
    MOCK_METHOD(void, disconnect, (), (override));
    MOCK_METHOD(bool, isConnected, (), (const, override));
};
```

## 📈 Performance Characteristics

### Benchmarks
| Operation | Target Performance | Current Performance |
|-----------|-------------------|-------------------|
| **Pool Scan** | <100ms per network | 85ms per network |
| **Profit Calculation** | <1ms per pair | 0.8ms per pair |
| **Memory Usage** | <512MB | 480MB |
| **CPU Usage** | <80% peak | 75% peak |
| **Network Latency** | <50ms average | 45ms average |

### Optimization Strategies
- **Memory Pooling** - Reuse objects to reduce allocations
- **Batch Processing** - Process multiple requests together
- **Async I/O** - Non-blocking network operations
- **Caching** - Cache frequently accessed data
- **SIMD** - Vectorized calculations where possible

## 🔒 Security Considerations

### Input Validation
- **Sanitization** - All external input is sanitized
- **Bounds Checking** - Array and buffer bounds are verified
- **Type Safety** - Strong typing prevents type confusion

### Network Security
- **TLS/SSL** - All network communication is encrypted
- **Certificate Validation** - SSL certificates are verified
- **Rate Limiting** - API calls are rate-limited

### Access Control
- **API Key Management** - Secure storage of API keys
- **Permission Checks** - Access control for sensitive operations
- **Audit Logging** - All operations are logged

## 📚 Next Steps

After reviewing the reference documentation:
- [Explore the CLI reference](cli/README.md) for command details
- [Learn about network protocols](network/README.md) for integration
- [Check the project structure](project-structure.md) for code organization
- [Review configuration options](configuration.md) for setup

---

*Need specific technical details? Explore the sections above or check the source code directly.*
