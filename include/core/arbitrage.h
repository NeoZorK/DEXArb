//
//  arbitrage.h
//  NeoZorKDEXArb
//
//  Created by Rostyslav S. on 18.03.2025.
//

#ifndef ARBITRAGE_H
#define ARBITRAGE_H

#include "main.h"       // For DexInfo, FunctionStats

// Function to analyze arbitrage opportunities (stub)
void analyze_arbitrage_opportunities(const std::vector<DexInfo>& dex_list, FunctionStats& stats);

// Function to execute arbitrage trade manually (stub)
void execute_manual_arbitrage(const std::string& dex_name, const std::string& pool_address, uint64_t amount, FunctionStats& stats);

// Function to execute arbitrage trade automatically (stub)
void execute_auto_arbitrage(const std::vector<DexInfo>& dex_list, FunctionStats& stats);

#endif // ARBITRAGE_H
