# Wine Docker Usage Guide

## Overview
This guide explains how to use the Wine Docker container to run Windows applications on macOS/Linux.

## Container Status

### Check Container Status
```bash
./scripts/wine-docker.sh status
```

### Start Container
```bash
./scripts/wine-docker.sh start
```

### Stop Container
```bash
./scripts/wine-docker.sh stop
```

### Access Container Shell
```bash
./scripts/wine-docker.sh shell
```

## Running Windows Applications

### Batch Files
```bash
docker exec -it wine-ubuntu wine cmd /c /app/your-script.bat
```

### Executable Files
```bash
docker exec -it wine-ubuntu wine /app/your-app.exe
```

### Interactive Commands
```bash
docker exec -it wine-ubuntu wine cmd
```

## Available Test Files

### 1. Simple Batch Test
- File: `app/test-simple.bat`
- Purpose: Basic Windows batch functionality test
- Run: `docker exec -it wine-ubuntu wine cmd /c /app/test-simple.bat`

### 2. Wine Test Suite
- File: `app/wine-test.bat`
- Purpose: Comprehensive Wine functionality test
- Run: `docker exec -it wine-ubuntu wine cmd /c /app/wine-test.bat`

### 3. C++ Test Application
- File: `app/simple-test.cpp` (source)
- File: `app/simple-test.exe` (compiled)
- Purpose: Test Wine with compiled Windows executables
- Run: `docker exec -it wine-ubuntu wine /app/simple-test.exe`

## Compiling Windows Applications

### Using MinGW (already installed)
```bash
# Compile C++ to Windows executable
docker exec -it wine-ubuntu x86_64-w64-mingw32-g++ -o /app/output.exe /app/source.cpp

# Compile C to Windows executable
docker exec -it wine-ubuntu x86_64-w64-mingw32-gcc -o /app/output.exe /app/source.c
```

## Wine Configuration

### Environment Variables
- `WINEARCH=win64` - Use 64-bit Wine
- `WINEPREFIX=/root/.wine64` - Wine prefix location
- `WINEDEBUG=-all` - Disable debug output

### Wine Prefix
The Wine prefix is located at `/root/.wine64` inside the container.

## Limitations

### Known Issues
1. **wine32 missing**: The container runs on ARM64, so wine32 (32-bit) is not available
2. **X11 display**: GUI applications may not work due to display server limitations
3. **File system**: Some Windows drives may not be accessible

### Workarounds
1. Use 64-bit applications when possible
2. Run applications in console mode
3. Mount necessary directories to `/app` in the container

## Troubleshooting

### Common Problems

#### Wine32 Missing Error
- **Cause**: Running on ARM64 architecture
- **Solution**: Use 64-bit applications or ignore the warning (it's just informational)

#### Display Errors
- **Cause**: No X11 server available
- **Solution**: Run console applications only

#### File Access Issues
- **Cause**: Windows drive mapping
- **Solution**: Use the `/app` directory for file access

### Debug Mode
To enable Wine debug output:
```bash
docker exec -it -e WINEDEBUG=+all wine-ubuntu wine your-app.exe
```

## Performance Notes

- Wine adds some overhead compared to native Windows
- Console applications perform better than GUI applications
- Large applications may take longer to start
- Memory usage is higher than native Windows

## Best Practices

1. **Use console applications** when possible
2. **Mount only necessary directories** to reduce overhead
3. **Compile applications** using MinGW for better compatibility
4. **Test thoroughly** before deploying to production
5. **Monitor resource usage** during execution

## Examples

### Running a Custom Application
```bash
# Copy your application to the container
docker cp my-app.exe wine-ubuntu:/app/

# Run the application
docker exec -it wine-ubuntu wine /app/my-app.exe
```

### Batch Processing
```bash
# Create a batch file for your workflow
echo "echo Processing files..." > process.bat
echo "dir /app" >> process.bat

# Copy and run
docker cp process.bat wine-ubuntu:/app/
docker exec -it wine-ubuntu wine cmd /c /app/process.bat
```

### Compiling and Running
```bash
# Compile source code
docker exec -it wine-ubuntu x86_64-w64-mingw32-g++ -o /app/myapp.exe /app/mysource.cpp

# Run the compiled application
docker exec -it wine-ubuntu wine /app/myapp.exe
```
