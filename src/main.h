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

// Time units for Time Measure
enum ENUM_TIME_UNITS {NANOSECONDS, MICROSECONDS, MILLISECONDS, SECONDS};


// Function to find arbitrage opportunities
void findArbitrageOpportunity();

// Start Time Measure
void StartTimeMeasure();

// Stop Time Measure
void StopTimeMeasure(const ENUM_TIME_UNITS time_units);


#endif /* main_h */
