# Deployment Scripts

This directory contains deployment and distribution scripts for the DEXArb project.

## Scripts

### `DeployBins.sh`
Main deployment script for distributing compiled binaries and artifacts.

## Usage

### Deploy Binaries
```bash
./scripts/deployment/DeployBins.sh
```

### Deploy with Options
```bash
./scripts/deployment/DeployBins.sh --platform linux --arch x64
```

## Deployment Features

- Cross-platform binary distribution
- Automated artifact packaging
- Version management
- Deployment validation

## Dependencies

- Compiled binaries available
- Target deployment environment access
- Required permissions for deployment

## Notes

- Run from project root directory
- Ensure build is complete before deployment
- Verify target environment compatibility
- Check deployment permissions
