//
//  profit_analyzer.h
//  NeoZorKDEXArb
//
//  Created by Rostyslav S. on 18.03.2025.
//

#ifndef PROFIT_ANALYZER_H
#define PROFIT_ANALYZER_H

#include "main.h"       // Include main header for shared structures

// Function to measure transaction speed and estimate profit (stub)
void measure_tx_speed_and_profit(const std::vector<DexInfo>& dex_list, FunctionStats& stats);

// Function to maximize profit with zero-loss guarantee (stub)
void maximize_profit_zero_loss(const std::vector<DexInfo>& dex_list, uint64_t& max_profit, FunctionStats& stats);

#endif // PROFIT_ANALYZER_H
