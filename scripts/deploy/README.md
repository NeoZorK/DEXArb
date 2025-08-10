# Deployment Scripts

This directory contains deployment and distribution scripts for the DEXArb project.

## Scripts Overview

- **`DeployBins.sh`** - Main binary deployment script for production releases

## DeployBins.sh

### Purpose
Automates the deployment of compiled binaries to target environments and distribution channels.

### Features
- Binary validation and integrity checks
- Multi-platform deployment support
- Version management and rollback capabilities
- Automated distribution to package managers

### Usage

```bash
# Make script executable
chmod +x scripts/deploy/DeployBins.sh

# Deploy current build
./scripts/deploy/DeployBins.sh

# Deploy with specific version
./scripts/deploy/DeployBins.sh --version 1.0.0

# Dry run (no actual deployment)
./scripts/deploy/DeployBins.sh --dry-run
```

### Prerequisites
- Successful build completion
- Valid binary artifacts in `build/` directory
- Deployment credentials configured
- Target environment access

### Deployment Targets
- Package repositories
- Container registries
- Binary distribution servers
- Development environments

## Security Considerations

- All binaries are signed before deployment
- Checksums are verified during deployment
- Rollback procedures are available for failed deployments
- Audit logs are maintained for all deployment activities

## Troubleshooting

- Verify build artifacts exist and are valid
- Check deployment credentials and permissions
- Review deployment logs for specific errors
- Ensure target environments are accessible
