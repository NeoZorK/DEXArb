# DEXArb Test Suite

This directory contains comprehensive tests for the DEXArb application, ensuring code quality and reliability.

## 🧪 Test Structure

### Test Files
- **`test_main_functions.py`** - Tests for main application functions (help, version, CLI)
- **`test_config.py`** - Tests for configuration management
- **`test_blockchain.py`** - Tests for blockchain functionality and validation

### Test Categories
- **Unit Tests** - Test individual functions and components
- **Integration Tests** - Test complete workflows and component interactions
- **Mock Tests** - Test functionality using mocked C++ functions

## 🚀 Running Tests

### Prerequisites
Install testing dependencies:
```bash
uv pip install -r requirements.txt
```

### Run All Tests
```bash
uv run pytest tests -n auto
```

### Run Specific Test File
```bash
uv run pytest tests/test_main_functions.py -v
```

### Run Tests with Coverage
```bash
uv run pytest tests --cov=src --cov-report=html
```

### Run Tests in Parallel
```bash
uv run pytest tests -n auto --dist=loadfile
```

## 📊 Test Coverage

The test suite provides comprehensive coverage for:
- ✅ Command line interface functionality
- ✅ Help and version display functions
- ✅ Configuration management
- ✅ Blockchain type handling
- ✅ Input validation
- ✅ Error handling

## 🎯 Test Goals

1. **100% Code Coverage** - All new features must have complete test coverage
2. **Fast Execution** - Tests run in parallel for quick feedback
3. **Reliable Results** - Tests are deterministic and repeatable
4. **Clear Documentation** - Each test has clear purpose and description

## 🔧 Test Configuration

### pytest.ini
- Parallel execution with `-n auto`
- Coverage reporting enabled
- HTML and XML coverage reports
- Warning suppression for clean output

### Test Markers
- `@pytest.mark.slow` - For slow-running tests
- `@pytest.mark.integration` - For integration tests
- `@pytest.mark.unit` - For unit tests

## 📝 Adding New Tests

### Test Naming Convention
- Test files: `test_<module_name>.py`
- Test classes: `Test<ClassName>`
- Test methods: `test_<functionality>`

### Test Structure
```python
def test_functionality_description(self):
    """Test description explaining what is being tested"""
    # Arrange
    # Act
    # Assert
```

### Example Test
```python
def test_help_flag_behavior(self):
    """Test that help flags trigger help display"""
    # Test -h flag
    result = MockMainFunctions.process_arguments(["-h"])
    assert result == "help"
    
    # Test -help flag
    result = MockMainFunctions.process_arguments(["-help"])
    assert result == "help"
```

## 🚨 Troubleshooting

### Common Issues
1. **Import Errors** - Ensure `src/` directory is in Python path
2. **Test Failures** - Check test output for detailed error messages
3. **Coverage Issues** - Verify source files are in `src/` directory

### Debug Mode
Run tests with verbose output:
```bash
uv run pytest tests -v -s
```

### Test Isolation
Run single test method:
```bash
uv run pytest tests/test_main_functions.py::TestMockMainFunctions::test_show_help -v
```

## 📈 Continuous Integration

Tests are designed to run in CI/CD pipelines:
- Fast execution (< 30 seconds)
- Deterministic results
- Clear pass/fail status
- Comprehensive coverage reporting

## 🤝 Contributing

When adding new features:
1. Write tests first (TDD approach)
2. Ensure 100% coverage for new code
3. Follow existing test patterns
4. Update this README if needed
