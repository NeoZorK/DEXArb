//
//  profit_analyzer.h
//  NeoZorKDEXArb
//
//  Created by Rostyslav S. on 18.03.2025.
//

#ifndef PROFIT_ANALYZER_H
#define PROFIT_ANALYZER_H

#include "main.h"       // For DexInfo, FunctionStats

// Function to measure transaction speed and estimate profit (stub)
void measure_tx_speed_and_profit(const std::vector<struct_dex_info>& dex_list, struct_function_stats& stats);

// Function to maximize profit with zero-loss guarantee (stub)
void maximize_profit_zero_loss(const std::vector<struct_dex_info>& dex_list, uint64_t& max_profit, struct_function_stats& stats);

#endif // PROFIT_ANALYZER_H
