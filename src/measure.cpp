//
//  MeasureRunTime.cpp
//  NeoZorKDEXArb
//
//  Created by Rostyslav S. on 26.02.2025.
//
#include "main.h"
#include "measure.h"        // Include measure header

void update_stats(FunctionStats& stats, const std::chrono::high_resolution_clock::time_point& start,
                  const std::chrono::high_resolution_clock::time_point& end, size_t outbound_size, size_t inbound_size) {
    // Calculate execution time in milliseconds
    stats.execution_time_ms = std::chrono::duration_cast<std::chrono::microseconds>(end - start).count() / 1000.0;
    
    // Update network traffic stats
    stats.outbound_traffic = outbound_size; // Set outbound traffic
    stats.inbound_traffic = inbound_size;   // Set inbound traffic
    
    // Placeholder values for future implementation
    stats.cpu_usage_percent = 0.0; // CPU usage not implemented yet
    stats.virtual_memory_kb = 0;   // Virtual memory not implemented yet
}

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

