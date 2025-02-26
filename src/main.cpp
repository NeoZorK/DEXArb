#include "main.h"

int main() {
    // Start the timer
    StartTimeMeasure();
    
    std::cout << "Starting DEX Arbitrage...\n";
    
    // Example of calling functions from another file arbitrage.cpp
    findArbitrageOpportunity();
    
    // Stop the timer
    StopTimeMeasure(MICROSECONDS);
    return 0;
}

