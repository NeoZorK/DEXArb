//
//  MeasureRunTime.cpp
//  NeoZorKDEXArb
//
//  Created by Rostyslav S. on 26.02.2025.
//
#include "main.h"

static auto start_time = std::chrono::high_resolution_clock::now();

void StartTimeMeasure() {
    start_time = std::chrono::high_resolution_clock::now();
}

void StopTimeMeasure(const ENUM_TIME_UNITS timeUnits) {
    auto end_time = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::nanoseconds>(end_time - start_time);
    
    // Print the duration in nanoseconds or microseconds
        switch (timeUnits) {
            case NANOSECONDS:
                std::cout << "Time Running: " << duration.count() << " nanoseconds" << std::endl;
                break;
                
            case MICROSECONDS:
                std::cout << "Time Running: " << duration.count() / 1000 << " microseconds" << std::endl;
                break;
                
            case MILLISECONDS:
                std::cout << "Time Running: " << duration.count() / 1000000 << " milliseconds" << std::endl;
                break;
                
            case SECONDS:
                std::cout << "Time Running: " << duration.count() / 1000000000 << " seconds" << std::endl;
                break;
                
            default:
                break;
        }
}

