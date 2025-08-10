# Test Scripts

This directory contains testing and validation scripts for the DEXArb project.

## Scripts Overview

- **`quick-test.sh`** - Fast testing script for development iterations
- **`test-all-platforms.sh`** - Comprehensive cross-platform testing

## Quick Test Script

### Purpose
Provides fast testing capabilities for development workflows and quick validation of changes.

### Features
- Rapid test execution
- Minimal setup requirements
- Development-friendly output
- Quick feedback loop

### Usage

```bash
# Make script executable
chmod +x scripts/test/quick-test.sh

# Run quick tests
./scripts/test/quick-test.sh

# Run with specific test suite
./scripts/test/quick-test.sh --suite core

# Run with verbose output
./scripts/test/quick-test.sh --verbose
```

### Test Coverage
- Core functionality tests
- Basic integration tests
- Performance benchmarks
- Configuration validation

## All Platforms Test Script

### Purpose
Comprehensive testing across multiple platforms and architectures to ensure compatibility.

### Features
- Multi-platform testing
- Architecture validation
- Cross-compilation testing
- Platform-specific test cases

### Usage

```bash
# Make script executable
chmod +x scripts/test/test-all-platforms.sh

# Run all platform tests
./scripts/test/test-all-platforms.sh

# Test specific platform
./scripts/test/test-all-platforms.sh --platform linux

# Run with parallel execution
./scripts/test/test-all-platforms.sh --parallel
```

### Supported Platforms
- Linux (x86_64, ARM64)
- macOS (Intel, Apple Silicon)
- Windows (x86_64)
- Alpine Linux (ARM64)

### Test Categories
- **Unit Tests**: Individual component testing
- **Integration Tests**: Component interaction testing
- **Performance Tests**: Benchmarking and profiling
- **Compatibility Tests**: Platform-specific validation

## Test Execution

### Prerequisites
- All dependencies installed
- Build artifacts available
- Test data configured
- Sufficient system resources

### Test Output
- Test results summary
- Performance metrics
- Error reports
- Coverage statistics

### Continuous Integration
- Automated test execution
- Result reporting
- Failure notifications
- Historical test data

## Troubleshooting

- **Tests fail**: Check dependencies and build status
- **Performance issues**: Verify system resources
- **Platform errors**: Ensure platform-specific tools installed
- **Timeout issues**: Adjust test timeouts for slower systems

## Best Practices

- Run quick tests during development
- Use all-platform tests before releases
- Monitor test execution times
- Review test coverage regularly
- Maintain test data quality
