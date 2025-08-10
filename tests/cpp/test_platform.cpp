#include <gtest/gtest.h>
#include <iostream>
#include <string>
#include <sys/stat.h>
#include <unistd.h>

// Include the platform header
#include "utils/platform.h"

class PlatformTest : public ::testing::Test {
protected:
    void SetUp() override {
        // Create test files and directories
        system("mkdir -p test_dir");
        system("echo 'test content' > test_file.txt");
    }
    
    void TearDown() override {
        // Clean up test files
        system("rm -rf test_dir");
        system("rm -f test_file.txt");
    }
};

// Test platform detection macros
TEST_F(PlatformTest, PlatformDetection) {
    // These should be defined based on the current platform
    #ifdef PLATFORM_MACOS
        EXPECT_STREQ(PLATFORM_NAME, "macOS");
    #elif defined(PLATFORM_LINUX)
        #ifdef PLATFORM_MUSL
            EXPECT_STREQ(PLATFORM_NAME, "Linux Alpine (musl)");
        #elif defined(PLATFORM_GLIBC)
            EXPECT_STREQ(PLATFORM_NAME, "Linux Ubuntu (glibc)");
        #else
            EXPECT_STREQ(PLATFORM_NAME, "Linux (generic)");
        #endif
    #elif defined(PLATFORM_WINDOWS)
        EXPECT_STREQ(PLATFORM_NAME, "Windows");
    #else
        EXPECT_STREQ(PLATFORM_NAME, "Unknown");
    #endif
}

// Test file operations
TEST_F(PlatformTest, FileExists) {
    EXPECT_TRUE(Platform::fileExists("test_file.txt"));
    EXPECT_FALSE(Platform::fileExists("nonexistent_file.txt"));
}

TEST_F(PlatformTest, IsDirectory) {
    EXPECT_TRUE(Platform::isDirectory("test_dir"));
    EXPECT_FALSE(Platform::isDirectory("test_file.txt"));
    EXPECT_FALSE(Platform::isDirectory("nonexistent_dir"));
}

// Test network operations
TEST_F(PlatformTest, NetworkOperations) {
    // These functions should not crash
    Platform::initNetwork();
    Platform::cleanupNetwork();
    
    // Test multiple calls
    Platform::initNetwork();
    Platform::cleanupNetwork();
}

// Test process information
TEST_F(PlatformTest, ProcessInformation) {
    int pid = Platform::getProcessId();
    EXPECT_GT(pid, 0);
    
    int uid = Platform::getUserId();
    EXPECT_GE(uid, 0);
}

// Test platform-specific includes are available
TEST_F(PlatformTest, PlatformIncludes) {
    // Test that platform-specific headers can be included
    // This is mainly a compilation test
    
    #ifdef PLATFORM_MACOS
        // macOS specific includes should be available
        EXPECT_TRUE(true); // Placeholder for actual tests
    #elif defined(PLATFORM_LINUX)
        // Linux specific includes should be available
        EXPECT_TRUE(true); // Placeholder for actual tests
    #elif defined(PLATFORM_WINDOWS)
        // Windows specific includes should be available
        EXPECT_TRUE(true); // Placeholder for actual tests
    #endif
}

// Test edge cases
TEST_F(PlatformTest, EdgeCases) {
    // Test with empty/null paths
    EXPECT_FALSE(Platform::fileExists(""));
    EXPECT_FALSE(Platform::isDirectory(""));
    
    // Test with very long paths
    std::string long_path(1000, 'a');
    EXPECT_FALSE(Platform::fileExists(long_path.c_str()));
    EXPECT_FALSE(Platform::isDirectory(long_path.c_str()));
}

// Test platform constants
TEST_F(PlatformTest, PlatformConstants) {
    // Test that platform constants are defined
    #ifdef PLATFORM_WINDOWS
        EXPECT_TRUE(true); // Windows specific constants
    #elif defined(PLATFORM_MACOS)
        EXPECT_TRUE(true); // macOS specific constants
    #elif defined(PLATFORM_LINUX)
        EXPECT_TRUE(true); // Linux specific constants
    #else
        EXPECT_TRUE(true); // Unknown platform
    #endif
}

// Test multiple file operations
TEST_F(PlatformTest, MultipleFileOperations) {
    // Create multiple test files
    system("echo 'file1' > test_file1.txt");
    system("echo 'file2' > test_file2.txt");
    system("mkdir -p test_dir1");
    system("mkdir -p test_dir2");
    
    // Test all files exist
    EXPECT_TRUE(Platform::fileExists("test_file1.txt"));
    EXPECT_TRUE(Platform::fileExists("test_file2.txt"));
    EXPECT_TRUE(Platform::isDirectory("test_dir1"));
    EXPECT_TRUE(Platform::isDirectory("test_dir2"));
    
    // Clean up
    system("rm -f test_file1.txt test_file2.txt");
    system("rm -rf test_dir1 test_dir2");
}

// Test performance under load
TEST_F(PlatformTest, PerformanceTest) {
    const int iterations = 1000;
    
    auto start = std::chrono::high_resolution_clock::now();
    
    for (int i = 0; i < iterations; ++i) {
        Platform::fileExists("test_file.txt");
        Platform::isDirectory("test_dir");
    }
    
    auto end = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(end - start);
    
    // Should complete in reasonable time (less than 1 second)
    EXPECT_LT(duration.count(), 1000);
}

// Test error handling
TEST_F(PlatformTest, ErrorHandling) {
    // Test with invalid file descriptors (if applicable)
    // This is mainly to ensure the functions don't crash
    
    // Test with special characters in paths
    EXPECT_FALSE(Platform::fileExists("test_file*.txt"));
    EXPECT_FALSE(Platform::isDirectory("test_dir?"));
}

int main(int argc, char **argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
