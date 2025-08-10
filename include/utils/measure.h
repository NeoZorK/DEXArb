//
//  measure.h
//  NeoZorKDEXArb
//
//  Created by Rostyslav S. on 18.03.2025.
//

#ifndef MEASURE_H
#define MEASURE_H

#include "main.h"       // Include main header for shared structures
#include <chrono>       // For timing

// Function to update performance stats
void update_stats(FunctionStats& stats, const std::chrono::high_resolution_clock::time_point& start,
                  const std::chrono::high_resolution_clock::time_point& end, size_t outbound_size, size_t inbound_size);

#endif // MEASURE_H
