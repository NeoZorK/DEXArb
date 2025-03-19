//
//  dex_stats.h
//  NeoZorKDEXArb
//
//  Created by Rostyslav S. on 18.03.2025.
//

#ifndef DEX_STATS_H
#define DEX_STATS_H

#include "main.h"       // Include main header for shared structures
#include <mutex>        // Include mutex for thread synchronization
#include <atomic>       // Include atomic for thread-safe counters

// Function to fetch swap statistics for a pool in a thread
void get_pool_swap_stats_thread(const std::string& rpc_url,
                                const std::string& pool_address,
                                uint64_t from_block,
                                uint64_t to_block,
                                int request_limit,
                                uint64_t& volume,
                                uint64_t& tx_count,
                                std::mutex& mtx,
                                std::atomic<int>& progress,
                                int total_pools);

#endif // DEX_STATS_H
