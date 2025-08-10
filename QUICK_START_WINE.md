# Quick Start: Wine Docker Container

## Prerequisites
- Docker installed and running
- Docker Compose installed

## Quick Commands

1. **Start the container:**
   ```bash
   ./scripts/wine-docker.sh start
   ```

2. **Access container shell:**
   ```bash
   ./scripts/wine-docker.sh shell
   ```

3. **Run Windows executable:**
   ```bash
   ./scripts/wine-docker.sh run /app/program.exe
   ```

4. **Stop container:**
   ```bash
   ./scripts/wine-docker.sh stop
   ```

## What's Included

- **Dockerfile.wine** - Ubuntu 22.04 with Wine 64-bit
- **docker-compose.wine.yml** - Container orchestration
- **scripts/wine-docker.sh** - Management script
- **app/** - Directory for Windows applications
- **docs/wine-docker-setup.md** - Full documentation

## First Run

The first time you start the container, Wine will automatically initialize. This may take a few minutes.

## Testing

A test batch file is included in the `app/` directory:
```bash
./scripts/wine-docker.sh run /app/test-wine.bat
```

## GUI Applications

For GUI applications, ensure X11 forwarding:
```bash
xhost +local:docker
```

## More Information

See `docs/wine-docker-setup.md` for detailed instructions and troubleshooting.
