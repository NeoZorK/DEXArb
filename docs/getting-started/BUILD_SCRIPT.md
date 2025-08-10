# Build and Deploy Script

The `build-and-deploy.sh` script automates the complete build, test, commit, and push process for NeoZorKDEXArb.

## 🚀 Quick Start

```bash
# Basic usage - build, test, commit and push
./build-and-deploy.sh

# Build in debug mode
./build-and-deploy.sh -d

# Build and commit with custom message
./build-and-deploy.sh "Update version 1.0.5"
```

## 📋 Features

### ✅ Automated Workflow
- **Dependency checking** - Verifies all required tools are installed
- **Git status management** - Handles uncommitted changes and pulls latest updates
- **CMake build** - Configures and builds the project automatically
- **Binary deployment** - Copies built binary to `bin/` directory
- **Git operations** - Commits changes and pushes to remote repository

### 🛡️ Safety Features
- **Error handling** - Exits on any error with clear messages
- **Dependency validation** - Checks for required tools before starting
- **Git repository validation** - Ensures you're in a git repository
- **Interactive prompts** - Asks before committing uncommitted changes

### 🎨 User Experience
- **Colored output** - Clear visual feedback with color-coded messages
- **Progress indicators** - Shows current step in the process
- **Help system** - Comprehensive help with examples
- **Cross-platform** - Works on macOS, Linux, and Windows

## 📖 Usage

### Basic Commands

```bash
# Full workflow (default)
./build-and-deploy.sh

# Build only (no commit/push)
./build-and-deploy.sh --build-only

# Debug build
./build-and-deploy.sh -d

# Custom commit message
./build-and-deploy.sh "Fix critical bug in DEX scanner"

# Show help
./build-and-deploy.sh -h
```

### Command Line Options

| Option | Description | Example |
|--------|-------------|---------|
| `-h, --help` | Show help message | `./build-and-deploy.sh -h` |
| `-d, --debug` | Build in debug mode | `./build-and-deploy.sh -d` |
| `-t, --test` | Run tests after build | `./build-and-deploy.sh -t` |
| `-c, --commit` | Commit and push changes | `./build-and-deploy.sh -c` |
| `-a, --all` | Build, test, commit and push | `./build-and-deploy.sh -a` |

### Examples

#### Development Workflow
```bash
# 1. Make changes to code
# 2. Run build and deploy
./build-and-deploy.sh "Add new DEX scanner feature"

# 3. Script will:
#    - Check dependencies
#    - Pull latest changes
#    - Build project
#    - Copy binary to bin/
#    - Commit changes
#    - Push to repository
```

#### Debug Development
```bash
# Build in debug mode for development
./build-and-deploy.sh -d "Debug build for testing"

# This will:
#    - Build with debug symbols
#    - Skip tests (tests not implemented yet)
#    - Commit and push
```

#### Testing Only
```bash
# Build and run tests (when implemented)
./build-and-deploy.sh -t

# This will:
#    - Build project
#    - Run tests
#    - Skip commit/push
```

## 🔧 Dependencies

The script automatically checks for required dependencies:

### Required Tools
- **CMake** (3.28+) - Build system
- **Git** - Version control
- **C++ Compiler** - g++, clang++, or cl (MSVC)

### Installation by OS

#### macOS
```bash
brew install cmake curl git
```

#### Ubuntu/Debian
```bash
sudo apt install build-essential cmake libcurl4-openssl-dev git
```

#### CentOS/RHEL
```bash
sudo yum install gcc-c++ cmake libcurl-devel git
```

#### Windows
```bash
# Install Visual Studio with C++ support
# Install CMake from https://cmake.org/download/
```

## 📁 Output

### Generated Files
- **`build/`** - CMake build directory
- **`bin/NeoZorKDEXArb`** - Compiled binary (macOS/Linux)
- **`bin/NeoZorKDEXArb.exe`** - Compiled binary (Windows)

### Console Output
```
[INFO] Starting NeoZorKDEXArb build and deploy process...
[INFO] Build type: release
[INFO] Run tests: false
[INFO] Commit and push: true
[INFO] Checking dependencies...
[SUCCESS] All dependencies found
[INFO] Checking git status...
[INFO] Current branch: v1.0.4
[INFO] Pulling latest changes...
[INFO] Building project...
[INFO] Configuring with CMake...
[INFO] Building project...
[SUCCESS] Binary copied to bin/ directory
[INFO] Preparing to commit and push...
[SUCCESS] Changes committed
[INFO] Pushing to remote repository...
[SUCCESS] Successfully pushed to v1.0.4
[SUCCESS] Build and deploy process completed successfully!
```

## ⚠️ Troubleshooting

### Common Issues

#### Missing Dependencies
```bash
[ERROR] Missing dependencies: cmake
[INFO] Please install missing dependencies:
[INFO] macOS: brew install cmake curl
```

**Solution**: Install the missing dependency as shown in the output.

#### Not a Git Repository
```bash
[ERROR] Not a git repository
```

**Solution**: Run the script from the project root directory.

#### Build Failures
```bash
[ERROR] CMake configuration failed
[ERROR] Build failed
```

**Solution**: 
1. Check that all source files are present
2. Verify CMakeLists.txt is correct
3. Check for compilation errors in source code

#### Git Push Failures
```bash
[ERROR] Push failed
```

**Solution**:
1. Check internet connection
2. Verify git credentials
3. Ensure you have push permissions to the repository

### Debug Mode

For troubleshooting, use debug mode:
```bash
./build-and-deploy.sh -d
```

This will:
- Build with debug symbols
- Provide more detailed error messages
- Skip tests to isolate build issues

## 🔄 Integration with CI/CD

The script can be integrated into CI/CD pipelines:

```yaml
# Example GitHub Actions workflow
- name: Build and Deploy
  run: |
    chmod +x build-and-deploy.sh
    ./build-and-deploy.sh "CI/CD build: ${{ github.sha }}"
```

## 📝 Customization

### Environment Variables
You can customize the script behavior with environment variables:

```bash
# Skip dependency checks
export SKIP_DEPENDENCY_CHECK=1

# Custom build directory
export BUILD_DIR=custom_build

# Custom binary name
export BINARY_NAME=my_dex_scanner
```

### Script Modifications
The script is modular and can be easily modified:
- Add new build targets
- Customize commit messages
- Add additional validation steps
- Integrate with external tools

## 🤝 Contributing

When modifying the script:
1. Maintain backward compatibility
2. Add appropriate error handling
3. Update this documentation
4. Test on multiple platforms
5. Follow the existing code style

---

**Script Version**: 1.0.4  
**Last Updated**: March 2025  
**Maintained by**: Project Contributors
