#include <iostream>
#include <windows.h>
#include <string>

int main() {
    std::cout << "=== DEXArb Windows Test Application ===" << std::endl;
    std::cout << "Hello from Windows application!" << std::endl;
    
    // Get Windows version info
    OSVERSIONINFO osvi;
    ZeroMemory(&osvi, sizeof(OSVERSIONINFO));
    osvi.dwOSVersionInfoSize = sizeof(OSVERSIONINFO);
    
    if (GetVersionEx(&osvi)) {
        std::cout << "Windows Version: " << osvi.dwMajorVersion << "." << osvi.dwMinorVersion << std::endl;
        std::cout << "Build Number: " << osvi.dwBuildNumber << std::endl;
    }
    
    // Get system info
    SYSTEM_INFO sysInfo;
    GetSystemInfo(&sysInfo);
    std::cout << "Processor Architecture: " << sysInfo.wProcessorArchitecture << std::endl;
    std::cout << "Number of Processors: " << sysInfo.dwNumberOfProcessors << std::endl;
    
    // Get current directory
    char currentDir[MAX_PATH];
    GetCurrentDirectory(MAX_PATH, currentDir);
    std::cout << "Current Directory: " << currentDir << std::endl;
    
    // Get environment variable
    char* path = getenv("PATH");
    if (path) {
        std::cout << "PATH environment variable found" << std::endl;
    } else {
        std::cout << "PATH environment variable not found" << std::endl;
    }
    
    std::cout << "=== Test completed successfully ===" << std::endl;
    
    // Wait for user input
    std::cout << "Press Enter to continue...";
    std::cin.get();
    
    return 0;
}
