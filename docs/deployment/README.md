# Deployment Guide

Welcome to the DEXArb deployment guide! This section covers everything you need to know to deploy DEXArb in production environments, from basic setup to advanced containerization and monitoring.

## 🚀 Quick Deployment

### Prerequisites
- **Linux server** (Ubuntu 20.04+ recommended)
- **Docker** (optional, for containerized deployment)
- **4GB+ RAM** and **20GB+ storage**
- **Network access** to blockchain RPC endpoints

### Basic Deployment
```bash
# Clone and build
git clone <repository-url>
cd DEXArb
mkdir build && cd build
cmake .. -DCMAKE_BUILD_TYPE=Release
make -j$(nproc)

# Create configuration
mkdir -p /etc/dexarb
cp ../../config/production.json /etc/dexarb/config.json

# Install binary
sudo cp DEXArb /usr/local/bin/
sudo chmod +x /usr/local/bin/DEXArb

# Create service user
sudo useradd -r -s /bin/false dexarb
sudo chown -R dexarb:dexarb /etc/dexarb
```

## 📚 What You'll Find Here

This section covers:
- [**Deployment Guide**](deployment-guide.md) - Complete deployment instructions
- [**Container Usage**](containers/README.md) - Docker and container management
- [**Monitoring Setup**](monitoring.md) - Logging and monitoring configuration
- [**Production Checklist**](production-checklist.md) - Production readiness checklist

## 🏗️ Deployment Architectures

### Single Server Deployment
```
┌─────────────────────────────────────────────────────────┐
│                    Single Server                        │
├─────────────────────────────────────────────────────────┤
│  ┌─────────────┐  ┌─────────────┐  ┌─────────────┐    │
│  │   DEXArb    │  │   Logs      │  │   Config    │    │
│  │  Process    │  │   Directory │  │   Files     │    │
│  └─────────────┘  └─────────────┘  └─────────────┘    │
│  ┌─────────────┐  ┌─────────────┐  ┌─────────────┐    │
│  │   Data      │  │   Exports   │  │   Scripts   │    │
│  │   Directory│  │   Directory │  │   Directory │    │
│  └─────────────┘  └─────────────┘  └─────────────┘    │
└─────────────────────────────────────────────────────────┘
```

### Multi-Server Deployment
```
┌─────────────────┐    ┌─────────────────┐    ┌─────────────────┐
│   Load Balancer │    │   DEXArb Node 1 │    │   DEXArb Node 2 │
│                 │◄──►│                 │    │                 │
│ • Nginx/HAProxy │    │ • Core Process  │    │ • Core Process  │
│ • SSL Termination│   │ • Local Config  │    │ • Local Config  │
│ • Health Checks │    │ • Local Logs    │    │ • Local Logs    │
└─────────────────┘    └─────────────────┘    └─────────────────┘
         │                       │                       │
         ▼                       ▼                       ▼
┌─────────────────┐    ┌─────────────────┐    ┌─────────────────┐
│   Shared Storage│    │   Monitoring    │    │   Backup        │
│                 │    │                 │    │                 │
│ • NFS/S3        │    │ • Prometheus    │    │ • Automated     │
│ • Database      │    │ • Grafana       │    │ • Incremental   │
│ • Config Sync   │    │ • Alerting      │    │ • Disaster Rec. │
└─────────────────┘    └─────────────────┘    └─────────────────┘
```

### Containerized Deployment
```
┌─────────────────────────────────────────────────────────┐
│                 Docker Environment                      │
├─────────────────────────────────────────────────────────┤
│  ┌─────────────┐  ┌─────────────┐  ┌─────────────┐    │
│  │  DEXArb     │  │   Redis     │  │   Nginx     │    │
│  │  Container  │  │  Container  │  │  Container  │    │
│  └─────────────┘  └─────────────┘  └─────────────┘    │
│  ┌─────────────┐  ┌─────────────┐  ┌─────────────┐    │
│  │  Prometheus │  │   Grafana   │  │   Logs      │    │
│  │  Container  │  │  Container  │  │  Container  │    │
│  └─────────────┘  └─────────────┘  └─────────────┘    │
└─────────────────────────────────────────────────────────┘
```

## 🔧 System Requirements

### Minimum Requirements
| Component | Specification | Notes |
|-----------|---------------|-------|
| **CPU** | 2 cores, 2.0 GHz | x86_64 architecture |
| **RAM** | 4 GB | 8 GB recommended |
| **Storage** | 20 GB | SSD recommended |
| **Network** | 100 Mbps | Low latency to RPC endpoints |
| **OS** | Ubuntu 20.04+ | LTS versions preferred |

### Recommended Requirements
| Component | Specification | Notes |
|-----------|---------------|-------|
| **CPU** | 4+ cores, 3.0 GHz | For high-frequency scanning |
| **RAM** | 16 GB | For large pool datasets |
| **Storage** | 100 GB NVMe SSD | For high I/O operations |
| **Network** | 1 Gbps | For real-time arbitrage |
| **OS** | Ubuntu 22.04 LTS | Latest LTS with long support |

### Production Requirements
| Component | Specification | Notes |
|-----------|---------------|-------|
| **CPU** | 8+ cores, 3.5 GHz | For enterprise workloads |
| **RAM** | 32+ GB | For maximum performance |
| **Storage** | 500 GB+ NVMe RAID | For redundancy and speed |
| **Network** | 10 Gbps | For ultra-low latency |
| **OS** | Ubuntu 22.04 LTS | With security updates |

## 🐳 Container Deployment

### Docker Compose Setup
```yaml
# docker-compose.yml
version: '3.8'

services:
  dexarb:
    build: .
    container_name: dexarb
    restart: unless-stopped
    environment:
      - NETWORK=ethereum
      - LOG_LEVEL=info
      - CONFIG_PATH=/app/config/config.json
    volumes:
      - ./config:/app/config
      - ./data:/app/data
      - ./logs:/app/logs
      - ./exports:/app/exports
    ports:
      - "8080:8080"
    depends_on:
      - redis
    networks:
      - dexarb-network

  redis:
    image: redis:7-alpine
    container_name: dexarb-redis
    restart: unless-stopped
    volumes:
      - redis-data:/data
    networks:
      - dexarb-network

  nginx:
    image: nginx:alpine
    container_name: dexarb-nginx
    restart: unless-stopped
    ports:
      - "80:80"
      - "443:443"
    volumes:
      - ./nginx/nginx.conf:/etc/nginx/nginx.conf
      - ./nginx/ssl:/etc/nginx/ssl
    depends_on:
      - dexarb
    networks:
      - dexarb-network

  prometheus:
    image: prom/prometheus:latest
    container_name: dexarb-prometheus
    restart: unless-stopped
    ports:
      - "9090:9090"
    volumes:
      - ./monitoring/prometheus.yml:/etc/prometheus/prometheus.yml
      - prometheus-data:/prometheus
    networks:
      - dexarb-network

  grafana:
    image: grafana/grafana:latest
    container_name: dexarb-grafana
    restart: unless-stopped
    ports:
      - "3000:3000"
    environment:
      - GF_SECURITY_ADMIN_PASSWORD=admin
    volumes:
      - grafana-data:/var/lib/grafana
    networks:
      - dexarb-network

volumes:
  redis-data:
  prometheus-data:
  grafana-data:

networks:
  dexarb-network:
    driver: bridge
```

### Dockerfile
```dockerfile
# Dockerfile
FROM ubuntu:22.04

# Set environment variables
ENV DEBIAN_FRONTEND=noninteractive
ENV TZ=UTC

# Install system dependencies
RUN apt update && apt install -y \
    build-essential \
    cmake \
    git \
    curl \
    wget \
    libssl-dev \
    libcurl4-openssl-dev \
    nlohmann-json3-dev \
    pkg-config \
    && rm -rf /var/lib/apt/lists/*

# Set working directory
WORKDIR /app

# Copy source code
COPY . .

# Build the project
RUN mkdir build && cd build && \
    cmake .. -DCMAKE_BUILD_TYPE=Release && \
    make -j$(nproc)

# Create necessary directories
RUN mkdir -p /app/config /app/data /app/logs /app/exports

# Create non-root user
RUN useradd -r -s /bin/false dexarb && \
    chown -R dexarb:dexarb /app

# Switch to non-root user
USER dexarb

# Expose port
EXPOSE 8080

# Health check
HEALTHCHECK --interval=30s --timeout=10s --start-period=5s --retries=3 \
    CMD curl -f http://localhost:8080/health || exit 1

# Set entry point
CMD ["./build/DEXArb", "--config", "/app/config/config.json"]
```

## ⚙️ Configuration Management

### Production Configuration
```json
{
  "application": {
    "name": "DEXArb Production",
    "version": "2.0.0",
    "log_level": "info",
    "log_file": "/app/logs/dexarb.log",
    "pid_file": "/var/run/dexarb.pid"
  },
  "networks": {
    "ethereum": {
      "enabled": true,
      "rpc_url": "https://mainnet.infura.io/v3/YOUR_API_KEY",
      "chain_id": 1,
      "scan_interval": 30,
      "max_gas_price": 100,
      "timeout": 30,
      "retry_attempts": 3,
      "circuit_breaker": {
        "enabled": true,
        "failure_threshold": 5,
        "reset_timeout": 300
      }
    }
  },
  "scanning": {
    "enabled": true,
    "mode": "continuous",
    "scan_interval": 30,
    "min_profit_threshold": 0.5,
    "max_slippage": 1.0,
    "batch_size": 100,
    "max_concurrent": 10,
    "timeout": 30
  },
  "performance": {
    "max_memory": 8192,
    "cpu_limit": 0.8,
    "network_timeout": 30,
    "connection_pool_size": 50,
    "keep_alive": true
  },
  "monitoring": {
    "metrics_enabled": true,
    "metrics_port": 8080,
    "health_check_enabled": true,
    "health_check_interval": 30
  }
}
```

### Environment Variables
```bash
# Production environment variables
export DEXARB_ENVIRONMENT="production"
export DEXARB_LOG_LEVEL="info"
export DEXARB_CONFIG_PATH="/etc/dexarb/config.json"
export DEXARB_DATA_PATH="/var/lib/dexarb"
export DEXARB_LOG_PATH="/var/log/dexarb"

# Network-specific variables
export DEXARB_ETHEREUM_RPC_URL="https://mainnet.infura.io/v3/YOUR_KEY"
export DEXARB_ETHEREUM_SCAN_INTERVAL="30"
export DEXARB_ETHEREUM_MAX_GAS_PRICE="100"

# Performance tuning
export DEXARB_MAX_MEMORY="8192"
export DEXARB_CPU_LIMIT="0.8"
export DEXARB_NETWORK_TIMEOUT="30"
```

## 🔒 Security Configuration

### User and Permissions
```bash
# Create dedicated user
sudo useradd -r -s /bin/false dexarb

# Create necessary directories
sudo mkdir -p /etc/dexarb /var/lib/dexarb /var/log/dexarb /var/run/dexarb

# Set ownership
sudo chown -R dexarb:dexarb /etc/dexarb /var/lib/dexarb /var/log/dexarb /var/run/dexarb

# Set permissions
sudo chmod 750 /etc/dexarb
sudo chmod 755 /var/lib/dexarb /var/log/dexarb /var/run/dexarb
```

### Network Security
```bash
# Configure firewall
sudo ufw allow 22/tcp          # SSH
sudo ufw allow 80/tcp          # HTTP (if using web interface)
sudo ufw allow 443/tcp         # HTTPS
sudo ufw allow 8080/tcp        # DEXArb metrics port
sudo ufw enable

# Or using iptables
sudo iptables -A INPUT -p tcp --dport 22 -j ACCEPT
sudo iptables -A INPUT -p tcp --dport 8080 -j ACCEPT
sudo iptables -A INPUT -j DROP
```

### SSL/TLS Configuration
```bash
# Generate SSL certificate
sudo openssl req -x509 -nodes -days 365 -newkey rsa:2048 \
    -keyout /etc/ssl/private/dexarb.key \
    -out /etc/ssl/certs/dexarb.crt

# Set permissions
sudo chmod 600 /etc/ssl/private/dexarb.key
sudo chmod 644 /etc/ssl/certs/dexarb.crt
```

## 📊 Monitoring and Logging

### Systemd Service
```ini
# /etc/systemd/system/dexarb.service
[Unit]
Description=DEXArb Arbitrage Scanner
After=network.target
Wants=network.target

[Service]
Type=simple
User=dexarb
Group=dexarb
ExecStart=/usr/local/bin/DEXArb --config /etc/dexarb/config.json
ExecReload=/bin/kill -HUP $MAINPID
Restart=always
RestartSec=10
StandardOutput=journal
StandardError=journal
SyslogIdentifier=dexarb

# Security settings
NoNewPrivileges=true
PrivateTmp=true
ProtectSystem=strict
ProtectHome=true
ReadWritePaths=/var/lib/dexarb /var/log/dexarb /var/run/dexarb

[Install]
WantedBy=multi-user.target
```

### Log Rotation
```bash
# /etc/logrotate.d/dexarb
/var/log/dexarb/*.log {
    daily
    missingok
    rotate 52
    compress
    delaycompress
    notifempty
    create 644 dexarb dexarb
    postrotate
        systemctl reload dexarb > /dev/null 2>&1 || true
    endscript
}
```

### Prometheus Configuration
```yaml
# monitoring/prometheus.yml
global:
  scrape_interval: 15s
  evaluation_interval: 15s

scrape_configs:
  - job_name: 'dexarb'
    static_configs:
      - targets: ['dexarb:8080']
    metrics_path: '/metrics'
    scrape_interval: 10s

  - job_name: 'node-exporter'
    static_configs:
      - targets: ['node-exporter:9100']

alerting:
  alertmanagers:
    - static_configs:
        - targets:
          - alertmanager:9093

rule_files:
  - 'alerts.yml'
```

## 🚀 Deployment Scripts

### Automated Deployment
```bash
#!/bin/bash
# deploy.sh

set -e

echo "Starting DEXArb deployment..."

# Check prerequisites
if ! command -v docker &> /dev/null; then
    echo "Docker not found. Installing..."
    curl -fsSL https://get.docker.com -o get-docker.sh
    sudo sh get-docker.sh
    sudo usermod -aG docker $USER
fi

if ! command -v docker-compose &> /dev/null; then
    echo "Docker Compose not found. Installing..."
    sudo curl -L "https://github.com/docker/compose/releases/download/v2.20.0/docker-compose-$(uname -s)-$(uname -m)" -o /usr/local/bin/docker-compose
    sudo chmod +x /usr/local/bin/docker-compose
fi

# Create directories
mkdir -p {config,data,logs,exports,monitoring,nginx}

# Copy configuration files
cp config/production.json config/
cp monitoring/prometheus.yml monitoring/
cp nginx/nginx.conf nginx/

# Start services
docker-compose up -d

echo "Deployment completed successfully!"
echo "Access DEXArb at: http://localhost:8080"
echo "Access Grafana at: http://localhost:3000"
echo "Access Prometheus at: http://localhost:9090"
```

### Health Check Script
```bash
#!/bin/bash
# health-check.sh

# Check if DEXArb is running
if ! curl -f http://localhost:8080/health > /dev/null 2>&1; then
    echo "ERROR: DEXArb is not responding"
    exit 1
fi

# Check system resources
MEMORY_USAGE=$(free | grep Mem | awk '{printf "%.1f", $3/$2 * 100.0}')
CPU_USAGE=$(top -bn1 | grep "Cpu(s)" | awk '{print $2}' | awk -F'%' '{print $1}')

if (( $(echo "$MEMORY_USAGE > 90" | bc -l) )); then
    echo "WARNING: High memory usage: ${MEMORY_USAGE}%"
fi

if (( $(echo "$CPU_USAGE > 90" | bc -l) )); then
    echo "WARNING: High CPU usage: ${CPU_USAGE}%"
fi

echo "Health check passed"
echo "Memory usage: ${MEMORY_USAGE}%"
echo "CPU usage: ${CPU_USAGE}%"
```

## 🔄 Backup and Recovery

### Backup Strategy
```bash
#!/bin/bash
# backup.sh

BACKUP_DIR="/backup/dexarb"
DATE=$(date +%Y%m%d_%H%M%S)

# Create backup directory
mkdir -p "$BACKUP_DIR"

# Backup configuration
tar -czf "$BACKUP_DIR/config_$DATE.tar.gz" /etc/dexarb

# Backup data
tar -czf "$BACKUP_DIR/data_$DATE.tar.gz" /var/lib/dexarb

# Backup logs
tar -czf "$BACKUP_DIR/logs_$DATE.tar.gz" /var/log/dexarb

# Cleanup old backups (keep last 7 days)
find "$BACKUP_DIR" -name "*.tar.gz" -mtime +7 -delete

echo "Backup completed: $BACKUP_DIR"
```

### Recovery Procedure
```bash
#!/bin/bash
# recover.sh

BACKUP_FILE="$1"
RECOVERY_DIR="/tmp/recovery"

if [ -z "$BACKUP_FILE" ]; then
    echo "Usage: $0 <backup_file>"
    exit 1
fi

echo "Starting recovery from: $BACKUP_FILE"

# Stop services
sudo systemctl stop dexarb

# Extract backup
mkdir -p "$RECOVERY_DIR"
tar -xzf "$BACKUP_FILE" -C "$RECOVERY_DIR"

# Restore files
sudo cp -r "$RECOVERY_DIR/etc/dexarb" /etc/
sudo cp -r "$RECOVERY_DIR/var/lib/dexarb" /var/lib/
sudo cp -r "$RECOVERY_DIR/var/log/dexarb" /var/log/

# Set permissions
sudo chown -R dexarb:dexarb /etc/dexarb /var/lib/dexarb /var/log/dexarb

# Start services
sudo systemctl start dexarb

echo "Recovery completed successfully"
```

## 📚 Next Steps

After setting up deployment:
- [Configure monitoring](monitoring.md) for observability
- [Set up containers](containers/README.md) for scalability
- [Review production checklist](production-checklist.md) for readiness
- [Learn about maintenance](maintenance.md) for ongoing operations

---

*Ready to deploy? Let's get DEXArb running in production!*
