"""
Test suite for RPC core functionality.

This module tests:
- RPC client operations
- Request handling
- Response parsing
- Error handling
"""

import pytest
import json
from unittest.mock import patch, MagicMock

class MockRpcClient:
    """Mock class for RPC client functionality"""
    
    def __init__(self, endpoint_url, request_limit):
        self.endpoint_url = endpoint_url
        self.request_limit = request_limit
        self.request_count = 0
        self.last_request = None
    
    def make_request(self, method, params=None):
        """Make RPC request"""
        if self.request_count >= self.request_limit:
            return None, "Rate limit exceeded"
        
        self.request_count += 1
        self.last_request = {"method": method, "params": params}
        
        # Mock responses for different methods
        responses = {
            "eth_blockNumber": {"result": "0x123456"},
            "eth_getBlockByNumber": {"result": {"number": "0x123456", "hash": "0xabc..."}},
            "eth_getBalance": {"result": "0x1000000000000000000"},
            "eth_call": {"result": "0x0000000000000000000000000000000000000000000000000000000000000000"}
        }
        
        if method in responses:
            return responses[method], "Success"
        else:
            return None, "Method not supported"
    
    def reset_rate_limit(self):
        """Reset rate limit counter"""
        self.request_count = 0
    
    def get_status(self):
        """Get client status"""
        return {
            "endpoint": self.endpoint_url,
            "requests_made": self.request_count,
            "request_limit": self.request_limit,
            "available_requests": self.request_limit - self.request_count
        }

class MockRpcManager:
    """Mock class for RPC manager functionality"""
    
    def __init__(self):
        self.clients = {}
        self.failover_clients = {}
    
    def add_client(self, blockchain, client):
        """Add RPC client for blockchain"""
        if blockchain not in self.clients:
            self.clients[blockchain] = []
        self.clients[blockchain].append(client)
    
    def add_failover_client(self, blockchain, client):
        """Add failover RPC client"""
        if blockchain not in self.failover_clients:
            self.failover_clients[blockchain] = []
        self.failover_clients[blockchain].append(client)
    
    def get_client(self, blockchain):
        """Get available RPC client for blockchain"""
        if blockchain in self.clients and self.clients[blockchain]:
            # Return first available client
            for client in self.clients[blockchain]:
                if client.request_count < client.request_limit:
                    return client
        return None
    
    def get_failover_client(self, blockchain):
        """Get failover RPC client for blockchain"""
        if blockchain in self.failover_clients and self.failover_clients[blockchain]:
            for client in self.failover_clients[blockchain]:
                if client.request_count < client.request_limit:
                    return client
        return None
    
    def make_request_with_failover(self, blockchain, method, params=None):
        """Make RPC request with failover support"""
        # Try primary client first
        client = self.get_client(blockchain)
        if client:
            result, status = client.make_request(method, params)
            if status == "Success":
                return result, status
        
        # Try failover client
        failover_client = self.get_failover_client(blockchain)
        if failover_client:
            result, status = failover_client.make_request(method, params)
            return result, status
        
        return None, "No available clients"
    
    def get_blockchain_status(self, blockchain):
        """Get status for all clients of blockchain"""
        status = {
            "primary_clients": [],
            "failover_clients": [],
            "total_requests": 0,
            "available_requests": 0
        }
        
        if blockchain in self.clients:
            for client in self.clients[blockchain]:
                client_status = client.get_status()
                status["primary_clients"].append(client_status)
                status["total_requests"] += client_status["requests_made"]
                status["available_requests"] += client_status["available_requests"]
        
        if blockchain in self.failover_clients:
            for client in self.failover_clients[blockchain]:
                client_status = client.get_status()
                status["failover_clients"].append(client_status)
                status["total_requests"] += client_status["requests_made"]
                status["available_requests"] += client_status["available_requests"]
        
        return status

class TestRpcClient:
    """Test suite for RPC client functionality"""
    
    def test_rpc_client_initialization(self):
        """Test RPC client initialization"""
        client = MockRpcClient("https://test.com", 100)
        
        assert client.endpoint_url == "https://test.com"
        assert client.request_limit == 100
        assert client.request_count == 0
        assert client.last_request is None
    
    def test_rpc_client_make_request_success(self):
        """Test successful RPC request"""
        client = MockRpcClient("https://test.com", 100)
        
        result, status = client.make_request("eth_blockNumber")
        
        assert status == "Success"
        assert result["result"] == "0x123456"
        assert client.request_count == 1
        assert client.last_request["method"] == "eth_blockNumber"
    
    def test_rpc_client_make_request_unsupported_method(self):
        """Test RPC request with unsupported method"""
        client = MockRpcClient("https://test.com", 100)
        
        result, status = client.make_request("unsupported_method")
        
        assert status == "Method not supported"
        assert result is None
        assert client.request_count == 1
    
    def test_rpc_client_rate_limit(self):
        """Test RPC client rate limiting"""
        client = MockRpcClient("https://test.com", 2)
        
        # Make 2 requests (at limit)
        client.make_request("eth_blockNumber")
        client.make_request("eth_getBalance")
        
        # Third request should fail
        result, status = client.make_request("eth_call")
        
        assert status == "Rate limit exceeded"
        assert result is None
        assert client.request_count == 2
    
    def test_rpc_client_reset_rate_limit(self):
        """Test RPC client rate limit reset"""
        client = MockRpcClient("https://test.com", 2)
        
        client.make_request("eth_blockNumber")
        assert client.request_count == 1
        
        client.reset_rate_limit()
        assert client.request_count == 0
        
        # Should be able to make requests again
        result, status = client.make_request("eth_getBalance")
        assert status == "Success"
        assert client.request_count == 1
    
    def test_rpc_client_get_status(self):
        """Test RPC client status retrieval"""
        client = MockRpcClient("https://test.com", 100)
        
        client.make_request("eth_blockNumber")
        status = client.get_status()
        
        assert status["endpoint"] == "https://test.com"
        assert status["requests_made"] == 1
        assert status["request_limit"] == 100
        assert status["available_requests"] == 99

class TestRpcManager:
    """Test suite for RPC manager functionality"""
    
    def test_rpc_manager_initialization(self):
        """Test RPC manager initialization"""
        manager = MockRpcManager()
        
        assert manager.clients == {}
        assert manager.failover_clients == {}
    
    def test_rpc_manager_add_client(self):
        """Test adding RPC client"""
        manager = MockRpcManager()
        client = MockRpcClient("https://test.com", 100)
        
        manager.add_client("ethereum", client)
        
        assert "ethereum" in manager.clients
        assert len(manager.clients["ethereum"]) == 1
        assert manager.clients["ethereum"][0] == client
    
    def test_rpc_manager_add_multiple_clients(self):
        """Test adding multiple RPC clients for same blockchain"""
        manager = MockRpcManager()
        client1 = MockRpcClient("https://test1.com", 100)
        client2 = MockRpcClient("https://test2.com", 100)
        
        manager.add_client("ethereum", client1)
        manager.add_client("ethereum", client2)
        
        assert len(manager.clients["ethereum"]) == 2
        assert client1 in manager.clients["ethereum"]
        assert client2 in manager.clients["ethereum"]
    
    def test_rpc_manager_get_available_client(self):
        """Test getting available RPC client"""
        manager = MockRpcManager()
        client = MockRpcClient("https://test.com", 100)
        
        manager.add_client("ethereum", client)
        
        retrieved_client = manager.get_client("ethereum")
        assert retrieved_client == client
    
    def test_rpc_manager_get_client_none_available(self):
        """Test getting client when none available"""
        manager = MockRpcManager()
        client = MockRpcClient("https://test.com", 1)
        
        manager.add_client("ethereum", client)
        
        # Use up the rate limit
        client.make_request("eth_blockNumber")
        
        # Should not be available
        retrieved_client = manager.get_client("ethereum")
        assert retrieved_client is None
    
    def test_rpc_manager_make_request_with_failover(self):
        """Test RPC request with failover support"""
        manager = MockRpcManager()
        
        # Add primary client (rate limited)
        primary_client = MockRpcClient("https://primary.com", 1)
        primary_client.make_request("eth_blockNumber")  # Use up limit
        manager.add_client("ethereum", primary_client)
        
        # Add failover client
        failover_client = MockRpcClient("https://failover.com", 100)
        manager.add_failover_client("ethereum", failover_client)
        
        # Request should use failover client
        result, status = manager.make_request_with_failover("ethereum", "eth_getBalance")
        
        assert status == "Success"
        assert result["result"] == "0x1000000000000000000"
    
    def test_rpc_manager_get_blockchain_status(self):
        """Test getting blockchain status"""
        manager = MockRpcManager()
        
        client1 = MockRpcClient("https://test1.com", 100)
        client2 = MockRpcClient("https://test2.com", 100)
        
        manager.add_client("ethereum", client1)
        manager.add_client("ethereum", client2)
        
        # Make some requests
        client1.make_request("eth_blockNumber")
        client2.make_request("eth_getBalance")
        
        status = manager.get_blockchain_status("ethereum")
        
        assert len(status["primary_clients"]) == 2
        assert status["total_requests"] == 2
        assert status["available_requests"] == 198  # 100+100-2

class TestRpcIntegration:
    """Test suite for RPC functionality integration"""
    
    def test_complete_rpc_workflow(self):
        """Test complete RPC workflow"""
        manager = MockRpcManager()
        
        # Add clients for Ethereum
        primary_client = MockRpcClient("https://primary.eth.com", 100)
        failover_client = MockRpcClient("https://failover.eth.com", 100)
        
        manager.add_client("ethereum", primary_client)
        manager.add_failover_client("ethereum", failover_client)
        
        # Make requests
        result1, status1 = manager.make_request_with_failover("ethereum", "eth_blockNumber")
        result2, status2 = manager.make_request_with_failover("ethereum", "eth_getBalance")
        
        assert status1 == "Success"
        assert status2 == "Success"
        assert result1["result"] == "0x123456"
        assert result2["result"] == "0x1000000000000000000"
        
        # Check status
        blockchain_status = manager.get_blockchain_status("ethereum")
        assert blockchain_status["total_requests"] == 2
    
    def test_error_handling_workflow(self):
        """Test error handling in RPC workflow"""
        manager = MockRpcManager()
        
        # Try to make request without clients
        result, status = manager.make_request_with_failover("nonexistent", "eth_blockNumber")
        
        assert status == "No available clients"
        assert result is None
        
        # Add client but use up rate limit
        client = MockRpcClient("https://test.com", 1)
        client.make_request("eth_blockNumber")
        manager.add_client("ethereum", client)
        
        # Try to make request
        result, status = manager.make_request_with_failover("ethereum", "eth_getBalance")
        
        assert status == "No available clients"
        assert result is None
    
    def test_performance_metrics(self):
        """Test performance metrics in RPC operations"""
        manager = MockRpcManager()
        
        # Add multiple clients
        for i in range(5):
            client = MockRpcClient(f"https://client{i}.com", 100)
            manager.add_client("ethereum", client)
        
        # Make multiple requests
        for i in range(10):
            result, status = manager.make_request_with_failover("ethereum", "eth_blockNumber")
            assert status == "Success"
        
        # Check status
        blockchain_status = manager.get_blockchain_status("ethereum")
        assert blockchain_status["total_requests"] == 10
        assert blockchain_status["available_requests"] == 490  # 5*100-10
