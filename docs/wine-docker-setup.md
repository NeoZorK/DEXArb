# Wine Docker Container Setup

This guide explains how to set up and use a Docker container with Ubuntu and Wine to run Windows applications.

## Prerequisites

- Docker installed and running
- Docker Compose installed
- X11 forwarding support (for GUI applications)

## Quick Start

1. **Build and start the container:**
   ```bash
   chmod +x scripts/wine-docker.sh
   ./scripts/wine-docker.sh start
   ```

2. **Access the container shell:**
   ```bash
   ./scripts/wine-docker.sh shell
   ```

3. **Run a Windows executable:**
   ```bash
   ./scripts/wine-docker.sh run /app/program.exe
   ```

## Manual Setup

### 1. Build the Docker Image

```bash
docker build -f Dockerfile.wine -t wine-ubuntu .
```

### 2. Run the Container

```bash
docker run -it --rm \
  -v $(pwd)/app:/app \
  -v /tmp/.X11-unix:/tmp/.X11-unix:rw \
  -e DISPLAY=$DISPLAY \
  -e WINEARCH=win64 \
  -e WINEPREFIX=/root/.wine64 \
  wine-ubuntu
```

### 3. Using Docker Compose

```bash
docker-compose -f docker-compose.wine.yml up -d
```

## Container Management

The `wine-docker.sh` script provides convenient commands:

- `start` - Build and start the container
- `stop` - Stop the container
- `restart` - Restart the container
- `shell` - Access the container shell
- `run <exe>` - Run a Windows executable
- `status` - Show container status
- `logs` - Show container logs
- `cleanup` - Remove container and all data

## Wine Configuration

### Initial Setup

When you first access the container, Wine will automatically initialize:

```bash
./scripts/wine-docker.sh shell
# Wine will initialize automatically on first run
```

### Installing Windows Components

Use Winetricks to install additional Windows components:

```bash
# Inside the container
winetricks vcrun2019
winetricks dotnet48
winetricks corefonts
```

### Wine Configuration

Access Wine configuration:

```bash
# Inside the container
winecfg
```

## Running Windows Applications

### Command Line Applications

```bash
# From host
./scripts/wine-docker.sh run /app/command.exe

# From container shell
wine /app/command.exe
```

### GUI Applications

For GUI applications, ensure X11 forwarding is properly configured:

```bash
# Allow X11 connections
xhost +local:docker

# Run GUI application
./scripts/wine-docker.sh run /app/gui_app.exe
```

## Volume Mounts

- `./app:/app` - Application directory
- `wine-data:/root/.wine64` - Wine configuration and data
- `/tmp/.X11-unix:/tmp/.X11-unix:rw` - X11 display

## Troubleshooting

### Common Issues

1. **Wine not working:**
   ```bash
   # Check Wine version
   wine --version
   
   # Reinitialize Wine
   rm -rf /root/.wine64
   wineboot --init
   ```

2. **GUI applications not displaying:**
   ```bash
   # Check X11 forwarding
   echo $DISPLAY
   
   # Allow X11 connections
   xhost +local:docker
   ```

3. **Permission issues:**
   ```bash
   # Fix file permissions
   chmod -R 755 /app
   ```

### Debug Mode

Enable verbose Wine output:

```bash
export WINEDEBUG=+all
wine program.exe
```

## Performance Optimization

### Memory Settings

Configure Wine memory settings in `winecfg`:
- Graphics tab: Adjust video memory
- Staging tab: Enable CSMT for better performance

### Graphics Drivers

For better graphics performance:

```bash
# Install additional graphics libraries
apt-get update && apt-get install -y \
    libgl1-mesa-glx \
    libgl1-mesa-dri \
    xvfb
```

## Security Considerations

- The container runs with root privileges inside
- Wine applications have access to the host filesystem through volume mounts
- Consider using a non-root user for production environments

## Examples

### Running a Simple Windows Application

```bash
# Start container
./scripts/wine-docker.sh start

# Copy your .exe file to the app directory
cp myapp.exe app/

# Run the application
./scripts/wine-docker.sh run /app/myapp.exe
```

### Installing and Running Software

```bash
# Access container shell
./scripts/wine-docker.sh shell

# Download and install software
wget https://example.com/software.exe
wine software.exe

# Run installed software
wine "C:\Program Files\Software\software.exe"
```

## Support

For issues related to:
- Docker: Check Docker documentation
- Wine: Visit WineHQ.org
- Container setup: Review this documentation

## Notes

- Wine performance may vary depending on the Windows application
- Some applications may require specific Wine versions or configurations
- GUI applications work best with proper X11 forwarding setup
- Consider using Wine staging for better compatibility with newer applications
