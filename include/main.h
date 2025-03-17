//
//  main.h
//  NeoZorKDEXArb
//
//  Created by Rostyslav S. on 26.02.2025.
//

#ifndef main_h
#define main_h

#include <iostream>
#include <chrono>
#include <vector>
#include <string>
#include <algorithm>

// Input Flags
struct STRUCT_Input_Flags {
    bool show = false;
    bool showSCAN = false;
    bool showDEXES = false;
    bool showPOOLS = false;
    bool showTOKENS = false;
    bool findTOKEN = false;
    bool findTOKENS = false;
};

// Time units for Time Measure
enum ENUM_TIME_UNITS {NANOSECONDS, MICROSECONDS, MILLISECONDS, SECONDS};

// Parse Input 
STRUCT_Input_Flags Parse_Arguments(int argc, char* argv[]);

// Function to find arbitrage opportunities
void findArbitrageOpportunity();

// Start Time Measure
void StartTimeMeasure();

// Stop Time Measure
void StopTimeMeasure(const ENUM_TIME_UNITS time_units);


#endif /* main_h */
