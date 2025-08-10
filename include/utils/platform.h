//
//  platform.h
//  NeoZorKDEXArb
//
//  Universal platform detection and compatibility layer
//  Supports: macOS, Linux (Alpine musl), Linux (Ubuntu glibc), Windows
//

#ifndef PLATFORM_H
#define PLATFORM_H

// Platform detection
#ifdef _WIN32
    #define PLATFORM_WINDOWS
    #define PLATFORM_NAME "Windows"
#elif defined(__APPLE__)
    #define PLATFORM_MACOS
    #define PLATFORM_NAME "macOS"
#elif defined(__linux__)
    #define PLATFORM_LINUX
    #ifdef MUSL_LIBC
        #define PLATFORM_MUSL
        #define PLATFORM_NAME "Linux Alpine (musl)"
    #elif defined(PLATFORM_GLIBC)
        #define PLATFORM_NAME "Linux Ubuntu (glibc)"
    #else
        #define PLATFORM_NAME "Linux (generic)"
    #endif
#else
    #define PLATFORM_UNKNOWN
    #define PLATFORM_NAME "Unknown"
#endif

// Platform-specific includes
#ifdef PLATFORM_MACOS
    #include <CoreFoundation/CoreFoundation.h>
    #include <SystemConfiguration/SystemConfiguration.h>
    #include <Security/Security.h>
    #include <sys/stat.h>
    #include <unistd.h>
#elif defined(PLATFORM_LINUX)
    #include <sys/socket.h>
    #include <netinet/in.h>
    #include <arpa/inet.h>
    #include <sys/stat.h>
    #include <unistd.h>
    #include <fcntl.h>
    #include <sys/types.h>
#elif defined(PLATFORM_WINDOWS)
    #include <windows.h>
    #include <winsock2.h>
    #include <ws2tcpip.h>
    #include <io.h>
    #include <direct.h>
    #define stat _stat
    #define S_IFDIR _S_IFDIR
    #define S_IFREG _S_IFREG
#endif

// Platform-specific function wrappers
namespace Platform {
    
    // File operations
    inline bool fileExists(const char* path) {
        #ifdef PLATFORM_WINDOWS
            struct stat buffer;
            return (stat(path, &buffer) == 0);
        #else
            struct stat buffer;
            return (stat(path, &buffer) == 0);
        #endif
    }
    
    inline bool isDirectory(const char* path) {
        #ifdef PLATFORM_WINDOWS
            struct stat buffer;
            if (stat(path, &buffer) != 0) return false;
            return (buffer.st_mode & S_IFDIR) != 0;
        #else
            struct stat buffer;
            if (stat(path, &buffer) != 0) return false;
            return S_ISDIR(buffer.st_mode);
        #endif
    }
    
    // Network operations
    inline void initNetwork() {
        #ifdef PLATFORM_WINDOWS
            WSADATA wsaData;
            WSAStartup(MAKEWORD(2, 2), &wsaData);
        #endif
    }
    
    inline void cleanupNetwork() {
        #ifdef PLATFORM_WINDOWS
            WSACleanup();
        #endif
    }
    
    // Process information
    inline int getProcessId() {
        #ifdef PLATFORM_WINDOWS
            return static_cast<int>(GetCurrentProcessId());
        #else
            return static_cast<int>(getpid());
        #endif
    }
    
    inline int getUserId() {
        #ifdef PLATFORM_WINDOWS
            return static_cast<int>(GetCurrentProcessId()); // Simplified
        #else
            return static_cast<int>(getuid());
        #endif
    }
}

#endif // PLATFORM_H
