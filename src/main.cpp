//
//  main.cpp
//  NeoZorKDEXArb
//
//  Created by Rostyslav S. on 26.02.2025.
//
#include "main.h"

// Global Project Version
const std::string VERSION = "100";



//
//          MAIN FUNCTION
//
int main() {
    // Start the timer
    StartTimeMeasure();
    
    std::cout << "Starting DEX Arbitrage, VERSION: "<<VERSION<<"\n";
    
    // Example of calling functions from another file arbitrage.cpp
    findArbitrageOpportunity();
    
    // Stop the timer
    StopTimeMeasure(MICROSECONDS);
    return 0;
}

