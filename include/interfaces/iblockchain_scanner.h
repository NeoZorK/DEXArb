//
//  iblockchain_scanner.h
//  NeoZorKDEXArb
//
//  Created by Rostyslav S. on 10.08.2025.
//
#ifndef IBLOCKCHAIN_SCANNER_H
#define IBLOCKCHAIN_SCANNER_H

#include <string>
#include <vector>
#include <string_view>

// Forward declarations
struct DexInfo;
struct FunctionStats;

namespace interfaces {

// Interface for blockchain scanning operations
class IBlockchainScanner {
public:
    virtual ~IBlockchainScanner() = default;
    
    // Scan blockchain for DEX factory contracts
    virtual std::vector<DexInfo> scan_blockchain(
        std::string_view blockchain, 
        uint64_t start_block, 
        uint64_t end_block,
        FunctionStats& stats) = 0;
    
    // Get supported blockchains
    virtual std::vector<std::string> get_supported_blockchains() const = 0;
    
    // Validate blockchain name
    virtual bool is_blockchain_supported(std::string_view blockchain) const = 0;
    
    // Get scan statistics
    virtual FunctionStats get_scan_stats(std::string_view blockchain) const = 0;
};

} // namespace interfaces

#endif // IBLOCKCHAIN_SCANNER_H
