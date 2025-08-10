# Testing Scripts

This directory contains testing scripts and executables for the DEXArb project.

## Scripts

### Shell Scripts
- `test-wine.sh` - Wine compatibility testing
- `test-wine-complete.sh` - Complete Wine testing suite
- `quick-test.sh` - Quick functionality tests
- `test-all-platforms.sh` - Cross-platform testing

### Windows Scripts
- `test-wine.bat` - Windows batch file for Wine testing
- `test-simple.bat` - Simple Windows test script
- `wine-test.bat` - Wine-specific Windows testing

### PowerShell Scripts
- `test-powershell.ps1` - PowerShell testing script

### C++ Test Files
- `test-wine.cpp` - C++ Wine test implementation
- `simple-windows-test.cpp` - Simple Windows C++ test

## Executables

- `test-wine.exe` - Windows Wine test executable
- `simple-test.exe` - Simple test executable

## Usage

### Run All Tests
```bash
./scripts/testing/test-all-platforms.sh
```

### Quick Test
```bash
./scripts/testing/quick-test.sh
```

### Wine Testing
```bash
./scripts/testing/test-wine.sh
```

## Testing Features

- Cross-platform compatibility testing
- Wine integration testing
- Platform-specific validation
- Automated test execution

## Dependencies

- Platform-specific test environments
- Wine (for Wine testing)
- Required libraries and headers

## Notes

- Run from project root directory
- Ensure test environment is properly configured
- Check platform-specific requirements
- Review test output for failures
