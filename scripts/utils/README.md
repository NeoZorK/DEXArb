# Utility Scripts

This directory contains utility and helper scripts for the DEXArb project.

## Scripts Overview

- **`scan-fantom.sh`** - Fantom blockchain scanning utility

## Fantom Scanner

### Purpose
Utility script for scanning and analyzing the Fantom blockchain network.

### Features
- Blockchain data extraction
- Network monitoring
- Transaction analysis
- Performance metrics

### Usage

```bash
# Make script executable
chmod +x scripts/utils/scan-fantom.sh

# Run Fantom scan
./scripts/utils/scan-fantom.sh

# Scan with specific parameters
./scripts/utils/scan-fantom.sh --network mainnet

# Scan with custom output
./scripts/utils/scan-fantom.sh --output json
```

### Capabilities
- **Network Analysis**: Monitor Fantom network status
- **Transaction Scanning**: Analyze transaction patterns
- **Performance Metrics**: Collect network performance data
- **Data Export**: Export results in various formats

### Output Formats
- JSON for programmatic processing
- CSV for spreadsheet analysis
- Text for human reading
- Log files for debugging

## Utility Functions

### Common Operations
- Data validation and sanitization
- Format conversion
- Performance measurement
- Error handling and logging

### Integration Points
- Blockchain RPC endpoints
- Database connections
- File system operations
- Network communications

## Usage Examples

```bash
# Basic network scan
./scripts/utils/scan-fantom.sh

# Detailed analysis with logging
./scripts/utils/scan-fantom.sh --verbose --log-level debug

# Custom output directory
./scripts/utils/scan-fantom.sh --output-dir ./data/fantom

# Filter by time range
./scripts/utils/scan-fantom.sh --start-time "2024-01-01" --end-time "2024-01-31"
```

## Configuration

### Environment Variables
- `FANTOM_RPC_URL`: Fantom RPC endpoint
- `SCAN_TIMEOUT`: Request timeout in seconds
- `MAX_RETRIES`: Maximum retry attempts
- `LOG_LEVEL`: Logging verbosity

### Configuration Files
- Network endpoints configuration
- Scan parameters
- Output format settings
- Performance tuning options

## Error Handling

### Common Issues
- Network connectivity problems
- RPC endpoint failures
- Data format errors
- Resource limitations

### Recovery Strategies
- Automatic retry mechanisms
- Fallback endpoints
- Graceful degradation
- Error reporting and logging

## Performance Considerations

### Optimization Tips
- Use appropriate batch sizes
- Implement connection pooling
- Cache frequently accessed data
- Monitor resource usage

### Monitoring
- Execution time tracking
- Memory usage monitoring
- Network latency measurement
- Error rate tracking

## Troubleshooting

- **Connection failures**: Check network and RPC endpoints
- **Performance issues**: Monitor system resources
- **Data errors**: Verify input format and validation
- **Timeout errors**: Adjust timeout settings

## Best Practices

- Run utilities during low-traffic periods
- Monitor resource consumption
- Implement proper error handling
- Maintain comprehensive logging
- Regular performance optimization
