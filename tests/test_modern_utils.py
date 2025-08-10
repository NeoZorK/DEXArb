"""
Test suite for modern utilities functionality.

This module tests:
- Logger functionality
- JSON parsing capabilities
- String utility functions
- Error handling
"""

import pytest
import json
from unittest.mock import patch, MagicMock

class MockLogger:
    """Mock class for logger functionality"""
    
    def __init__(self, level="INFO"):
        self.level = level
        self.messages = []
        self.levels = ["TRACE", "DEBUG", "INFO", "WARN", "ERROR", "FATAL"]
        self.colors = {
            "TRACE": "\033[36m",
            "DEBUG": "\033[34m", 
            "INFO": "\033[32m",
            "WARN": "\033[33m",
            "ERROR": "\033[31m",
            "FATAL": "\033[35m"
        }
    
    def log(self, level, message):
        """Log a message at specified level"""
        if level in self.levels:
            self.messages.append((level, message))
            return f"{self.colors[level]}[{level}]{message}\033[0m"
        return message
    
    def info(self, message):
        """Log info message"""
        return self.log("INFO", message)
    
    def error(self, message):
        """Log error message"""
        return self.log("ERROR", message)
    
    def warn(self, message):
        """Log warning message"""
        return self.log("WARN", message)
    
    def debug(self, message):
        """Log debug message"""
        return self.log("DEBUG", message)
    
    def trace(self, message):
        """Log trace message"""
        return self.log("TRACE", message)
    
    def fatal(self, message):
        """Log fatal message"""
        return self.log("FATAL", message)

class MockJsonParser:
    """Mock class for JSON parsing functionality"""
    
    @staticmethod
    def parse(json_str):
        """Parse JSON string"""
        try:
            return json.loads(json_str)
        except json.JSONDecodeError:
            return None
    
    @staticmethod
    def extract_field(json_str, field):
        """Extract field from JSON string"""
        try:
            data = json.loads(json_str)
            return data.get(field, "")
        except (json.JSONDecodeError, AttributeError):
            return ""
    
    @staticmethod
    def validate_json(json_str):
        """Validate JSON string"""
        try:
            json.loads(json_str)
            return True
        except json.JSONDecodeError:
            return False

class MockStringUtils:
    """Mock class for string utility functions"""
    
    @staticmethod
    def to_lower(text):
        """Convert string to lowercase"""
        return text.lower() if isinstance(text, str) else str(text).lower()
    
    @staticmethod
    def to_upper(text):
        """Convert string to uppercase"""
        return text.upper() if isinstance(text, str) else str(text).upper()
    
    @staticmethod
    def split(text, delimiter):
        """Split string by delimiter"""
        if not isinstance(text, str):
            return []
        return text.split(delimiter)
    
    @staticmethod
    def trim(text):
        """Trim whitespace from string"""
        if not isinstance(text, str):
            return str(text)
        return text.strip()
    
    @staticmethod
    def starts_with(text, prefix):
        """Check if string starts with prefix"""
        if not isinstance(text, str) or not isinstance(prefix, str):
            return False
        return text.startswith(prefix)
    
    @staticmethod
    def ends_with(text, suffix):
        """Check if string ends with suffix"""
        if not isinstance(text, str) or not isinstance(suffix, str):
            return False
        return text.endswith(suffix)

class TestLogger:
    """Test suite for logger functionality"""
    
    def test_logger_initialization(self):
        """Test logger initialization with different levels"""
        logger = MockLogger("INFO")
        assert logger.level == "INFO"
        assert len(logger.messages) == 0
    
    def test_logger_levels(self):
        """Test all logger levels"""
        logger = MockLogger()
        
        # Test all levels
        for level in logger.levels:
            result = logger.log(level, f"Test {level} message")
            assert level in result
            assert "Test" in result
            assert "message" in result
    
    def test_logger_colors(self):
        """Test logger color formatting"""
        logger = MockLogger()
        
        for level in logger.levels:
            result = logger.log(level, "Test message")
            assert logger.colors[level] in result
            assert "\033[0m" in result  # Reset color
    
    def test_logger_methods(self):
        """Test logger convenience methods"""
        logger = MockLogger()
        
        # Test all convenience methods
        logger.info("Info message")
        logger.error("Error message")
        logger.warn("Warning message")
        logger.debug("Debug message")
        logger.trace("Trace message")
        logger.fatal("Fatal message")
        
        assert len(logger.messages) == 6
        assert logger.messages[0][0] == "INFO"
        assert logger.messages[1][0] == "ERROR"
        assert logger.messages[2][0] == "WARN"
        assert logger.messages[3][0] == "DEBUG"
        assert logger.messages[4][0] == "TRACE"
        assert logger.messages[5][0] == "FATAL"
    
    def test_logger_invalid_level(self):
        """Test logger with invalid level"""
        logger = MockLogger()
        result = logger.log("INVALID", "Test message")
        assert result == "Test message"  # Should return message without formatting

class TestJsonParser:
    """Test suite for JSON parsing functionality"""
    
    def test_parse_valid_json(self):
        """Test parsing valid JSON"""
        parser = MockJsonParser()
        
        valid_json = '{"name": "test", "value": 42}'
        result = parser.parse(valid_json)
        
        assert result is not None
        assert result["name"] == "test"
        assert result["value"] == 42
    
    def test_parse_invalid_json(self):
        """Test parsing invalid JSON"""
        parser = MockJsonParser()
        
        invalid_json = '{"name": "test", "value": 42'  # Missing closing brace
        result = parser.parse(invalid_json)
        
        assert result is None
    
    def test_extract_field_valid(self):
        """Test extracting field from valid JSON"""
        parser = MockJsonParser()
        
        json_str = '{"name": "test", "value": 42, "nested": {"key": "value"}}'
        
        assert parser.extract_field(json_str, "name") == "test"
        assert parser.extract_field(json_str, "value") == 42
        assert parser.extract_field(json_str, "nested") == {"key": "value"}
    
    def test_extract_field_missing(self):
        """Test extracting missing field"""
        parser = MockJsonParser()
        
        json_str = '{"name": "test", "value": 42}'
        
        assert parser.extract_field(json_str, "missing") == ""
        assert parser.extract_field(json_str, "nonexistent") == ""
    
    def test_extract_field_invalid_json(self):
        """Test extracting field from invalid JSON"""
        parser = MockJsonParser()
        
        invalid_json = '{"name": "test", "value": 42'  # Missing closing brace
        
        assert parser.extract_field(invalid_json, "name") == ""
    
    def test_validate_json_valid(self):
        """Test JSON validation with valid JSON"""
        parser = MockJsonParser()
        
        valid_json = '{"name": "test", "value": 42}'
        assert parser.validate_json(valid_json) is True
    
    def test_validate_json_invalid(self):
        """Test JSON validation with invalid JSON"""
        parser = MockJsonParser()
        
        invalid_json = '{"name": "test", "value": 42'  # Missing closing brace
        assert parser.validate_json(invalid_json) is False
    
    def test_validate_json_empty(self):
        """Test JSON validation with empty string"""
        parser = MockJsonParser()
        
        assert parser.validate_json("") is False
        assert parser.validate_json("   ") is False

class TestStringUtils:
    """Test suite for string utility functions"""
    
    def test_to_lower(self):
        """Test string to lowercase conversion"""
        utils = MockStringUtils()
        
        assert utils.to_lower("Hello World") == "hello world"
        assert utils.to_lower("UPPERCASE") == "uppercase"
        assert utils.to_lower("lowercase") == "lowercase"
        assert utils.to_lower("MiXeD") == "mixed"
        assert utils.to_lower("123") == "123"
        assert utils.to_lower("") == ""
    
    def test_to_upper(self):
        """Test string to uppercase conversion"""
        utils = MockStringUtils()
        
        assert utils.to_upper("Hello World") == "HELLO WORLD"
        assert utils.to_upper("UPPERCASE") == "UPPERCASE"
        assert utils.to_upper("lowercase") == "LOWERCASE"
        assert utils.to_upper("MiXeD") == "MIXED"
        assert utils.to_upper("123") == "123"
        assert utils.to_upper("") == ""
    
    def test_split(self):
        """Test string splitting"""
        utils = MockStringUtils()
        
        assert utils.split("a,b,c", ",") == ["a", "b", "c"]
        assert utils.split("hello world", " ") == ["hello", "world"]
        assert utils.split("no-delimiter", "|") == ["no-delimiter"]
        assert utils.split("", ",") == [""]
        assert utils.split("single", ",") == ["single"]
    
    def test_trim(self):
        """Test string trimming"""
        utils = MockStringUtils()
        
        assert utils.trim("  hello world  ") == "hello world"
        assert utils.trim("hello world") == "hello world"
        assert utils.trim("   ") == ""
        assert utils.trim("") == ""
        assert utils.trim("  \t\n\r  ") == ""
    
    def test_starts_with(self):
        """Test string starts with check"""
        utils = MockStringUtils()
        
        assert utils.starts_with("hello world", "hello") is True
        assert utils.starts_with("hello world", "world") is False
        assert utils.starts_with("hello", "hello") is True
        assert utils.starts_with("", "hello") is False
        assert utils.starts_with("hello", "") is True
    
    def test_ends_with(self):
        """Test string ends with check"""
        utils = MockStringUtils()
        
        assert utils.ends_with("hello world", "world") is True
        assert utils.ends_with("hello world", "hello") is False
        assert utils.ends_with("hello", "hello") is True
        assert utils.ends_with("", "hello") is False
        assert utils.ends_with("hello", "") is True
    
    def test_string_utils_edge_cases(self):
        """Test string utilities with edge cases"""
        utils = MockStringUtils()
        
        # Test with non-string inputs
        assert utils.to_lower(123) == "123"
        assert utils.to_upper(456) == "456"
        assert utils.split(None, ",") == []
        assert utils.trim(789) == "789"
        assert utils.starts_with(None, "test") is False
        assert utils.ends_with(None, "test") is False

class TestModernUtilsIntegration:
    """Test suite for modern utilities integration"""
    
    def test_logger_with_json_parsing(self):
        """Test logger integration with JSON parsing"""
        logger = MockLogger()
        parser = MockJsonParser()
        
        json_data = '{"operation": "parse", "status": "success"}'
        parsed = parser.parse(json_data)
        
        if parsed:
            logger.info(f"JSON parsed successfully: {parsed['operation']}")
            assert len(logger.messages) == 1
            assert "JSON parsed successfully" in logger.messages[0][1]
        else:
            logger.error("JSON parsing failed")
            assert len(logger.messages) == 1
            assert "JSON parsing failed" in logger.messages[0][1]
    
    def test_string_utils_with_json(self):
        """Test string utilities with JSON data"""
        utils = MockStringUtils()
        parser = MockJsonParser()
        
        json_str = '{"name": "Test User", "email": "test@example.com"}'
        
        # Extract and process fields
        name = parser.extract_field(json_str, "name")
        email = parser.extract_field(json_str, "email")
        
        # Process with string utilities
        name_lower = utils.to_lower(name)
        email_upper = utils.to_upper(email)
        
        assert name_lower == "test user"
        assert email_upper == "TEST@EXAMPLE.COM"
    
    def test_error_handling_workflow(self):
        """Test error handling workflow"""
        logger = MockLogger()
        parser = MockJsonParser()
        utils = MockStringUtils()
        
        # Simulate error scenario
        invalid_json = '{"name": "test"'
        
        if not parser.validate_json(invalid_json):
            error_msg = utils.to_upper("json validation failed")
            logger.error(error_msg)
            
            assert len(logger.messages) == 1
            assert logger.messages[0][0] == "ERROR"
            assert "JSON VALIDATION FAILED" in logger.messages[0][1]
