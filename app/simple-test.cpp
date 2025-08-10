#include <iostream>
#include <windows.h>

int main() {
    std::cout << "Hello from Windows application!" << std::endl;
    std::cout << "Windows version: " << GetVersion() << std::endl;
    std::cout << "Current directory: " << GetCurrentDirectoryA(0, nullptr) << std::endl;
    return 0;
}
