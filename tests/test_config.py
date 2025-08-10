"""
Test suite for configuration management functionality.

This module tests:
- Configuration file loading
- Configuration validation
- Default configuration creation
- RPC endpoint management
"""

import pytest
import json
import tempfile
import os
from unittest.mock import patch, mock_open, MagicMock

class MockConfigManager:
    """Mock class for configuration management functions"""
    
    @staticmethod
    def create_default_config():
        """Create default configuration"""
        return {
            "threads": 3,
            "ethereum": {
                "rpc_endpoints": [
                    {"url": "https://rpc.ankr.com/eth", "request_limit": 20},
                    {"url": "https://eth.llamarpc.com", "request_limit": 25}
                ]
            },
            "bsc": {
                "rpc_endpoints": [
                    {"url": "https://bsc-dataseed.binance.org", "request_limit": 50},
                    {"url": "https://rpc.ankr.com/bsc", "request_limit": 30}
                ]
            },
            "polygon": {
                "rpc_endpoints": [
                    {"url": "https://polygon-rpc.com", "request_limit": 30}
                ]
            }
        }
    
    @staticmethod
    def load_config(file_path):
        """Load configuration from file"""
        try:
            with open(file_path, 'r') as f:
                return json.load(f)
        except FileNotFoundError:
            return MockConfigManager.create_default_config()
        except json.JSONDecodeError:
            return MockConfigManager.create_default_config()
    
    @staticmethod
    def save_config(config, file_path):
        """Save configuration to file"""
        with open(file_path, 'w') as f:
            json.dump(config, f, indent=2)
    
    @staticmethod
    def validate_config(config):
        """Validate configuration structure"""
        required_keys = ["threads"]
        required_blockchain_keys = ["rpc_endpoints"]
        
        # Check required top-level keys
        for key in required_keys:
            if key not in config:
                return False, f"Missing required key: {key}"
        
        # Check blockchain configurations
        for blockchain, blockchain_config in config.items():
            if blockchain == "threads":
                continue
            
            if not isinstance(blockchain_config, dict):
                return False, f"Invalid blockchain config for {blockchain}"
            
            if "rpc_endpoints" not in blockchain_config:
                return False, f"Missing rpc_endpoints for {blockchain}"
            
            if not isinstance(blockchain_config["rpc_endpoints"], list):
                return False, f"Invalid rpc_endpoints format for {blockchain}"
            
            for endpoint in blockchain_config["rpc_endpoints"]:
                if not isinstance(endpoint, dict):
                    return False, f"Invalid endpoint format in {blockchain}"
                
                if "url" not in endpoint or "request_limit" not in endpoint:
                    return False, f"Missing required endpoint fields in {blockchain}"
        
        return True, "Configuration is valid"
    
    @staticmethod
    def get_rpc_endpoints(config, blockchain):
        """Get RPC endpoints for specific blockchain"""
        if blockchain not in config:
            return []
        
        return config[blockchain].get("rpc_endpoints", [])
    
    @staticmethod
    def get_thread_count(config):
        """Get thread count from configuration"""
        return config.get("threads", 3)

class TestConfigManager:
    """Test suite for configuration management"""
    
    def test_create_default_config(self):
        """Test default configuration creation"""
        config = MockConfigManager.create_default_config()
        
        # Check required keys
        assert "threads" in config
        assert "ethereum" in config
        assert "bsc" in config
        assert "polygon" in config
        
        # Check thread count
        assert config["threads"] == 3
        
        # Check blockchain configurations
        for blockchain in ["ethereum", "bsc", "polygon"]:
            assert "rpc_endpoints" in config[blockchain]
            assert isinstance(config[blockchain]["rpc_endpoints"], list)
            assert len(config[blockchain]["rpc_endpoints"]) > 0
    
    def test_load_config_existing_file(self):
        """Test loading configuration from existing file"""
        with tempfile.NamedTemporaryFile(mode='w', delete=False) as f:
            test_config = {
                "threads": 5,
                "ethereum": {
                    "rpc_endpoints": [{"url": "https://test.com", "request_limit": 10}]
                }
            }
            json.dump(test_config, f)
            temp_file = f.name
        
        try:
            config = MockConfigManager.load_config(temp_file)
            assert config["threads"] == 5
            assert len(config["ethereum"]["rpc_endpoints"]) == 1
            assert config["ethereum"]["rpc_endpoints"][0]["url"] == "https://test.com"
        finally:
            os.unlink(temp_file)
    
    def test_load_config_missing_file(self):
        """Test loading configuration when file doesn't exist"""
        config = MockConfigManager.load_config("nonexistent_file.json")
        
        # Should return default config
        assert "threads" in config
        assert "ethereum" in config
        assert config["threads"] == 3
    
    def test_save_config(self):
        """Test saving configuration to file"""
        test_config = {"threads": 7, "test": "value"}
        
        with tempfile.NamedTemporaryFile(mode='w', delete=False) as f:
            temp_file = f.name
        
        try:
            MockConfigManager.save_config(test_config, temp_file)
            
            # Load and verify
            loaded_config = MockConfigManager.load_config(temp_file)
            assert loaded_config["threads"] == 7
            assert loaded_config["test"] == "value"
        finally:
            os.unlink(temp_file)
    
    def test_validate_config_valid(self):
        """Test configuration validation with valid config"""
        valid_config = MockConfigManager.create_default_config()
        is_valid, message = MockConfigManager.validate_config(valid_config)
        
        assert is_valid
        assert message == "Configuration is valid"
    
    def test_validate_config_missing_threads(self):
        """Test configuration validation with missing threads"""
        invalid_config = {"ethereum": {"rpc_endpoints": []}}
        is_valid, message = MockConfigManager.validate_config(invalid_config)
        
        assert not is_valid
        assert "Missing required key: threads" in message
    
    def test_validate_config_missing_rpc_endpoints(self):
        """Test configuration validation with missing RPC endpoints"""
        invalid_config = {
            "threads": 3,
            "ethereum": {}
        }
        is_valid, message = MockConfigManager.validate_config(invalid_config)
        
        assert not is_valid
        assert "Missing rpc_endpoints for ethereum" in message
    
    def test_validate_config_invalid_endpoint_format(self):
        """Test configuration validation with invalid endpoint format"""
        invalid_config = {
            "threads": 3,
            "ethereum": {
                "rpc_endpoints": ["invalid_endpoint"]
            }
        }
        is_valid, message = MockConfigManager.validate_config(invalid_config)
        
        assert not is_valid
        assert "Invalid endpoint format in ethereum" in message
    
    def test_get_rpc_endpoints(self):
        """Test getting RPC endpoints for specific blockchain"""
        config = MockConfigManager.create_default_config()
        
        # Test existing blockchain
        ethereum_endpoints = MockConfigManager.get_rpc_endpoints(config, "ethereum")
        assert len(ethereum_endpoints) == 2
        assert ethereum_endpoints[0]["url"] == "https://rpc.ankr.com/eth"
        
        # Test non-existing blockchain
        non_existing_endpoints = MockConfigManager.get_rpc_endpoints(config, "nonexistent")
        assert non_existing_endpoints == []
    
    def test_get_thread_count(self):
        """Test getting thread count from configuration"""
        config = {"threads": 8}
        thread_count = MockConfigManager.get_thread_count(config)
        assert thread_count == 8
        
        # Test default value
        config_no_threads = {}
        thread_count = MockConfigManager.get_thread_count(config_no_threads)
        assert thread_count == 3

class TestConfigurationIntegration:
    """Integration tests for configuration management"""
    
    def test_full_config_workflow(self):
        """Test complete configuration workflow"""
        # Create default config
        config = MockConfigManager.create_default_config()
        
        # Validate config
        is_valid, message = MockConfigManager.validate_config(config)
        assert is_valid
        
        # Save config
        with tempfile.NamedTemporaryFile(mode='w', delete=False) as f:
            temp_file = f.name
        
        try:
            MockConfigManager.save_config(config, temp_file)
            
            # Load config
            loaded_config = MockConfigManager.load_config(temp_file)
            
            # Verify loaded config
            assert loaded_config["threads"] == config["threads"]
            assert "ethereum" in loaded_config
            assert "bsc" in loaded_config
            
            # Get specific values
            ethereum_endpoints = MockConfigManager.get_rpc_endpoints(loaded_config, "ethereum")
            thread_count = MockConfigManager.get_thread_count(loaded_config)
            
            assert len(ethereum_endpoints) > 0
            assert thread_count == 3
        finally:
            os.unlink(temp_file)
    
    def test_config_error_handling(self):
        """Test configuration error handling"""
        # Test invalid JSON
        with tempfile.NamedTemporaryFile(mode='w', delete=False) as f:
            f.write("invalid json content")
            temp_file = f.name
        
        try:
            config = MockConfigManager.load_config(temp_file)
            # Should return default config on error
            assert "threads" in config
            assert config["threads"] == 3
        finally:
            os.unlink(temp_file)

if __name__ == "__main__":
    # Run tests if file is executed directly
    pytest.main([__file__, "-v"])
