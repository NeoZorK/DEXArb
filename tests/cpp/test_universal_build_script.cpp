#include <gtest/gtest.h>
#include <string>
#include <vector>
#include <filesystem>
#include <fstream>
#include <sstream>

// Test fixture for universal build script
class UniversalBuildScriptTest : public ::testing::Test {
protected:
    void SetUp() override {
        // Create test directories
        std::filesystem::create_directories("test-build");
        std::filesystem::create_directories("test-dist");
    }
    
    void TearDown() override {
        // Clean up test directories
        std::filesystem::remove_all("test-build");
        std::filesystem::remove_all("test-dist");
    }
    
    // Helper function to check if file exists
    bool fileExists(const std::string& path) {
        return std::filesystem::exists(path);
    }
    
    // Helper function to check if directory exists
    bool directoryExists(const std::string& path) {
        return std::filesystem::is_directory(path);
    }
    
    // Helper function to get file content
    std::string getFileContent(const std::string& path) {
        std::ifstream file(path);
        if (!file.is_open()) {
            return "";
        }
        std::stringstream buffer;
        buffer << file.rdbuf();
        return buffer.str();
    }
};

// Test script file existence
TEST_F(UniversalBuildScriptTest, ScriptFileExists) {
    std::string scriptPath = "scripts/build/build-universal.sh";
    EXPECT_TRUE(fileExists(scriptPath)) << "Universal build script should exist";
}

// Test script is executable
TEST_F(UniversalBuildScriptTest, ScriptIsExecutable) {
    std::string scriptPath = "scripts/build/build-universal.sh";
    std::filesystem::path path(scriptPath);
    std::filesystem::perms perms = std::filesystem::status(path).permissions();
    
    bool isExecutable = (perms & std::filesystem::perms::owner_exec) != std::filesystem::perms::none ||
                       (perms & std::filesystem::perms::group_exec) != std::filesystem::perms::none ||
                       (perms & std::filesystem::perms::others_exec) != std::filesystem::perms::none;
    
    EXPECT_TRUE(isExecutable) << "Script should be executable";
}

// Test script content validation
TEST_F(UniversalBuildScriptTest, ScriptContentValidation) {
    std::string scriptPath = "scripts/build/build-universal.sh";
    std::string content = getFileContent(scriptPath);
    
    // Check for required components
    EXPECT_FALSE(content.empty()) << "Script should not be empty";
    EXPECT_NE(content.find("#!/bin/bash"), std::string::npos) << "Script should start with shebang";
    EXPECT_NE(content.find("Universal Interactive Build Script"), std::string::npos) << "Script should have correct header";
    EXPECT_NE(content.find("main()"), std::string::npos) << "Script should have main function";
    EXPECT_NE(content.find("set -euo pipefail"), std::string::npos) << "Script should have proper error handling";
}

// Test script structure
TEST_F(UniversalBuildScriptTest, ScriptStructure) {
    std::string scriptPath = "scripts/build/build-universal.sh";
    std::string content = getFileContent(scriptPath);
    
    // Check for required functions
    std::vector<std::string> requiredFunctions = {
        "detect_platform",
        "check_dependencies", 
        "build_native",
        "build_container",
        "build_windows",
        "run_tests",
        "create_package",
        "show_build_summary"
    };
    
    for (const auto& func : requiredFunctions) {
        EXPECT_NE(content.find(func), std::string::npos) 
            << "Script should contain function: " << func;
    }
}

// Test platform detection logic
TEST_F(UniversalBuildScriptTest, PlatformDetectionLogic) {
    std::string scriptPath = "scripts/build/build-universal.sh";
    std::string content = getFileContent(scriptPath);
    
    // Check platform detection patterns
    EXPECT_NE(content.find("darwin"), std::string::npos) << "Should detect macOS";
    EXPECT_NE(content.find("linux-gnu"), std::string::npos) << "Should detect Linux";
    EXPECT_NE(content.find("alpine-release"), std::string::npos) << "Should detect Alpine";
    EXPECT_NE(content.find("msys"), std::string::npos) << "Should detect Windows MSYS";
}

// Test build options
TEST_F(UniversalBuildScriptTest, BuildOptions) {
    std::string scriptPath = "scripts/build/build-universal.sh";
    std::string content = getFileContent(scriptPath);
    
    // Check build options
    std::vector<std::string> buildOptions = {
        "Native build",
        "Container build",
        "Cross-platform build", 
        "Windows build",
        "Alpine container build",
        "Ubuntu container build"
    };
    
    for (const auto& option : buildOptions) {
        EXPECT_NE(content.find(option), std::string::npos) 
            << "Script should offer build option: " << option;
    }
}

// Test build types
TEST_F(UniversalBuildScriptTest, BuildTypes) {
    std::string scriptPath = "scripts/build/build-universal.sh";
    std::string content = getFileContent(scriptPath);
    
    // Check build types
    std::vector<std::string> buildTypes = {
        "Release",
        "Debug", 
        "RelWithDebInfo"
    };
    
    for (const auto& type : buildTypes) {
        EXPECT_NE(content.find(type), std::string::npos) 
            << "Script should support build type: " << type;
    }
}

// Test dependency checking
TEST_F(UniversalBuildScriptTest, DependencyChecking) {
    std::string scriptPath = "scripts/build/build-universal.sh";
    std::string content = getFileContent(scriptPath);
    
    // Check required dependencies
    std::vector<std::string> dependencies = {
        "git",
        "cmake",
        "make",
        "c++-compiler"
    };
    
    for (const auto& dep : dependencies) {
        EXPECT_NE(content.find(dep), std::string::npos) 
            << "Script should check for dependency: " << dep;
    }
}

// Test vcpkg integration
TEST_F(UniversalBuildScriptTest, VcpkgIntegration) {
    std::string scriptPath = "scripts/build/build-universal.sh";
    std::string content = getFileContent(scriptPath);
    
    // Check vcpkg setup
    EXPECT_NE(content.find("vcpkg"), std::string::npos) << "Script should integrate with vcpkg";
    EXPECT_NE(content.find("bootstrap-vcpkg.sh"), std::string::npos) << "Script should bootstrap vcpkg";
    EXPECT_NE(content.find("curl nlohmann-json gtest"), std::string::npos) << "Script should install required packages";
}

// Test CMake integration
TEST_F(UniversalBuildScriptTest, CMakeIntegration) {
    std::string scriptPath = "scripts/build/build-universal.sh";
    std::string content = getFileContent(scriptPath);
    
    // Check CMake usage
    EXPECT_NE(content.find("cmake"), std::string::npos) << "Script should use CMake";
    EXPECT_NE(content.find("CMAKE_BUILD_TYPE"), std::string::npos) << "Script should set build type";
    EXPECT_NE(content.find("CMAKE_TOOLCHAIN_FILE"), std::string::npos) << "Script should use vcpkg toolchain";
}

// Test parallel build support
TEST_F(UniversalBuildScriptTest, ParallelBuildSupport) {
    std::string scriptPath = "scripts/build/build-universal.sh";
    std::string content = getFileContent(scriptPath);
    
    // Check parallel build support
    EXPECT_NE(content.find("nproc"), std::string::npos) << "Script should detect CPU cores (Linux)";
    EXPECT_NE(content.find("hw.ncpu"), std::string::npos) << "Script should detect CPU cores (macOS)";
    EXPECT_NE(content.find("-j"), std::string::npos) << "Script should use parallel make";
}

// Test error handling
TEST_F(UniversalBuildScriptTest, ErrorHandling) {
    std::string scriptPath = "scripts/build/build-universal.sh";
    std::string content = getFileContent(scriptPath);
    
    // Check error handling
    EXPECT_NE(content.find("set -euo pipefail"), std::string::npos) << "Script should have strict error handling";
    EXPECT_NE(content.find("exit 1"), std::string::npos) << "Script should exit on errors";
    EXPECT_NE(content.find("log \"ERROR\""), std::string::npos) << "Script should log errors";
}

// Test logging functionality
TEST_F(UniversalBuildScriptTest, LoggingFunctionality) {
    std::string scriptPath = "scripts/build/build-universal.sh";
    std::string content = getFileContent(scriptPath);
    
    // Check logging levels
    std::vector<std::string> logLevels = {
        "INFO",
        "WARN", 
        "ERROR",
        "DEBUG",
        "SUCCESS"
    };
    
    for (const auto& level : logLevels) {
        EXPECT_NE(content.find("log \"$level\""), std::string::npos) 
            << "Script should support logging level: " << level;
    }
}

// Test color output
TEST_F(UniversalBuildScriptTest, ColorOutput) {
    std::string scriptPath = "scripts/build/build-universal.sh";
    std::string content = getFileContent(scriptPath);
    
    // Check color definitions
    std::vector<std::string> colors = {
        "RED=",
        "GREEN=",
        "YELLOW=",
        "BLUE=",
        "CYAN=",
        "NC="
    };
    
    for (const auto& color : colors) {
        EXPECT_NE(content.find(color), std::string::npos) 
            << "Script should define color: " << color;
    }
}

// Test help functionality
TEST_F(UniversalBuildScriptTest, HelpFunctionality) {
    std::string scriptPath = "scripts/build/build-universal.sh";
    std::string content = getFileContent(scriptPath);
    
    // Check help options
    EXPECT_NE(content.find("--help"), std::string::npos) << "Script should support --help option";
    EXPECT_NE(content.find("-h"), std::string::npos) << "Script should support -h option";
    EXPECT_NE(content.find("show_help"), std::string::npos) << "Script should have help function";
}

// Test command line argument parsing
TEST_F(UniversalBuildScriptTest, ArgumentParsing) {
    std::string scriptPath = "scripts/build/build-universal.sh";
    std::string content = getFileContent(scriptPath);
    
    // Check argument parsing
    std::vector<std::string> arguments = {
        "--platform",
        "--build-type",
        "--clean",
        "--test",
        "--package",
        "--verbose"
    };
    
    for (const auto& arg : arguments) {
        EXPECT_NE(content.find(arg), std::string::npos) 
            << "Script should support argument: " << arg;
    }
}

// Test interactive mode
TEST_F(UniversalBuildScriptTest, InteractiveMode) {
    std::string scriptPath = "scripts/build/build-universal.sh";
    std::string content = getFileContent(scriptPath);
    
    // Check interactive functionality
    EXPECT_NE(content.find("read -r"), std::string::npos) << "Script should read user input";
    EXPECT_NE(content.find("get_user_choice"), std::string::npos) << "Script should have user choice function";
    EXPECT_NE(content.find("show_build_options"), std::string::npos) << "Script should show build options";
}

// Test package creation
TEST_F(UniversalBuildScriptTest, PackageCreation) {
    std::string scriptPath = "scripts/build/build-universal.sh";
    std::string content = getFileContent(scriptPath);
    
    // Check package creation
    EXPECT_NE(content.find("create_package"), std::string::npos) << "Script should create packages";
    EXPECT_NE(content.find("dist-"), std::string::npos) << "Script should create distribution directories";
    EXPECT_NE(content.find("README.txt"), std::string::npos) << "Script should create package README";
}

// Test test execution
TEST_F(UniversalBuildScriptTest, TestExecution) {
    std::string scriptPath = "scripts/build/build-universal.sh";
    std::string content = getFileContent(scriptPath);
    
    // Check test execution
    EXPECT_NE(content.find("run_tests"), std::string::npos) << "Script should run tests";
    EXPECT_NE(content.find("NeoZorKDEXArbTests"), std::string::npos) << "Script should find test executable";
    EXPECT_NE(content.find("Tests completed"), std::string::npos) << "Script should report test completion";
}

// Test build summary
TEST_F(UniversalBuildScriptTest, BuildSummary) {
    std::string scriptPath = "scripts/build/build-universal.sh";
    std::string content = getFileContent(scriptPath);
    
    // Check build summary
    EXPECT_NE(content.find("show_build_summary"), std::string::npos) << "Script should show build summary";
    EXPECT_NE(content.find("Build Summary"), std::string::npos) << "Script should display build summary";
    EXPECT_NE(content.find("Executable location"), std::string::npos) << "Script should show executable location";
}

// Test cross-platform support
TEST_F(UniversalBuildScriptTest, CrossPlatformSupport) {
    std::string scriptPath = "scripts/build/build-universal.sh";
    std::string content = getFileContent(scriptPath);
    
    // Check platform support
    std::vector<std::string> platforms = {
        "macos",
        "linux", 
        "alpine",
        "windows"
    };
    
    for (const auto& platform : platforms) {
        EXPECT_NE(content.find(platform), std::string::npos) 
            << "Script should support platform: " << platform;
    }
}

// Test container build support
TEST_F(UniversalBuildScriptTest, ContainerBuildSupport) {
    std::string scriptPath = "scripts/build/build-universal.sh";
    std::string content = getFileContent(scriptPath);
    
    // Check container support
    EXPECT_NE(content.find("docker"), std::string::npos) << "Script should support Docker";
    EXPECT_NE(content.find("alpine:latest"), std::string::npos) << "Script should support Alpine containers";
    EXPECT_NE(content.find("ubuntu:22.04"), std::string::npos) << "Script should support Ubuntu containers";
}

// Test Windows build support
TEST_F(UniversalBuildScriptTest, WindowsBuildSupport) {
    std::string scriptPath = "scripts/build/build-universal.sh";
    std::string content = getFileContent(scriptPath);
    
    // Check Windows build support
    EXPECT_NE(content.find("wine"), std::string::npos) << "Script should support Wine";
    EXPECT_NE(content.find("mingw32-make"), std::string::npos) << "Script should use MinGW make";
    EXPECT_NE(content.find("x86_64-w64-mingw32"), std::string::npos) << "Script should use MinGW toolchain";
}

// Test script completion
TEST_F(UniversalBuildScriptTest, ScriptCompletion) {
    std::string scriptPath = "scripts/build/build-universal.sh";
    std::string content = getFileContent(scriptPath);
    
    // Check script completion
    EXPECT_NE(content.find("Build process completed successfully"), std::string::npos) 
        << "Script should report successful completion";
    EXPECT_NE(content.find("main \"$@\""), std::string::npos) 
        << "Script should call main function with arguments";
}

int main(int argc, char** argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
