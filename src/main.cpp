//
//  main.cpp
//  NeoZorKDEXArb
//
//  Created by Rostyslav S. on 26.02.2025.
//
#include "main.h"


// Global Project Version
const std::string VERSION = "102";
//
//          MAIN FUNCTION
//
int main(int argc, char* argv[]) {
    
    // Start the timer
    StartTimeMeasure();
    
    std::cout << "Starting DEX Arbitrage, VERSION: "<<VERSION<<"\n";
    
    
    // Input Flags
    STRUCT_Input_Flags flags = Parse_Arguments(argc, argv);
    
    

    

    // Check Input
        if (flags.showDEXES) {
            std::cout << "showDEXES is enabled\n";
        }
        if (flags.showPOOLS) {
            std::cout << "showPOOLS is enabled\n";
        }
    
    // Example of calling functions from another file arbitrage.cpp
    findArbitrageOpportunity();
    
    // Stop the timer
    StopTimeMeasure(MICROSECONDS);
    return 0;
}

