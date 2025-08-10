#include <iostream>
#include <windows.h>

int main() {
    std::cout << "Hello from Windows application!" << std::endl;
    std::cout << "Windows version: " << GetVersion() << std::endl;
    return 0;
}
