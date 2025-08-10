//
//  walllet.h
//  NeoZorKDEXArb
//
//  Created by Rostyslav S. on 18.03.2025.
//

#ifndef WALLET_H
#define WALLET_H

#include "main.h"       // For FunctionStats

// Function to connect a wallet (stub)
void connect_wallet(const std::string& private_key, FunctionStats& stats);

// Function to disconnect a wallet (stub)
void disconnect_wallet(FunctionStats& stats);

// Function to check wallet balance (stub)
uint64_t check_wallet_balance(const std::string& token_address, FunctionStats& stats);

#endif // WALLET_H
