//
//  help_display.cpp
//  NeoZorKDEXArb
//
//  Created by Rostyslav S. on 10.08.2025.
//
#include "../../include/cli/help_display.h"
#include "../../include/main.h"
#include <iomanip>
#include <sstream>

namespace cli {

void HelpDisplay::show_help(std::ostream& output) {
    show_banner(output);
    
    output << "\n";
    output << format_text("USAGE", YELLOW) << '\n';
    output << "   " << format_text("dexarb", GREEN) << " [flag] <blockchain> [value]\n\n";
    
    show_supported_blockchains(output);
    show_command_flags(output);
    show_usage_examples(output);
    show_performance_tips(output);
    
    output << format_text("Built for blockchain arbitrage opportunities", GREEN) << '\n';
    output << "\n";
}

void HelpDisplay::show_version(std::ostream& output) {
    show_banner(output);
    
    output << "\n";
    output << format_text("BUILD INFORMATION", YELLOW) << '\n';
    output << "   " << format_text(">", BLUE) << " Version: " << format_text(get_project_version(), GREEN) << '\n';
    output << "   " << format_text(">", BLUE) << " Purpose: Blockchain arbitrage opportunities detection\n";
    output << "   " << format_text(">", BLUE) << " Language: C++23 (Modern C++)\n";
    output << "   " << format_text(">", BLUE) << " Platform: Cross-platform\n";
    output << "   " << format_text(">", BLUE) << " Features: Modern logging, JSON parsing, Error handling\n\n";
    
    output << format_text("Built for blockchain arbitrage opportunities", GREEN) << '\n';
    output << "\n";
}

void HelpDisplay::show_examples(std::ostream& output) {
    show_banner(output);
    
    output << "\n";
    output << format_text("DETAILED EXAMPLES", YELLOW) << '\n';
    output << "   Modern, aligned examples for all commands\n\n";
    
    output << format_text("Help & Information", GREEN) << '\n';
    output << "   " << format_text("*", BLUE) << " dexarb -h                    " << format_text("Show help", CYAN) << '\n';
    output << "   " << format_text("*", BLUE) << " dexarb -help                 " << format_text("Show help (alternative)", CYAN) << '\n';
    output << "   " << format_text("*", BLUE) << " dexarb -v                    " << format_text("Show version", CYAN) << '\n';
    output << "   " << format_text("*", BLUE) << " dexarb -version              " << format_text("Show version (alternative)", CYAN) << '\n';
    output << "   " << format_text("*", BLUE) << " dexarb -examples             " << format_text("Show detailed examples", CYAN) << '\n\n';
    
    output << format_text("Scanning & Discovery", GREEN) << '\n';
    output << "   " << format_text("*", BLUE) << " dexarb -scan fantom         " << format_text("Scan Fantom (default 1000 blocks)", CYAN) << '\n';
    output << "   " << format_text("*", BLUE) << " dexarb -scan 250 5000       " << format_text("Scan Fantom by network ID", CYAN) << '\n';
    output << "   " << format_text("*", BLUE) << " dexarb -scan ethereum 10000 " << format_text("Scan Ethereum (10k blocks)", CYAN) << '\n';
    output << "   " << format_text("*", BLUE) << " dexarb -scan 1 50000        " << format_text("Scan Ethereum by network ID", CYAN) << '\n';
    output << "   " << format_text("*", BLUE) << " dexarb -scan bsc 25000      " << format_text("Scan BSC (25k blocks)", CYAN) << '\n';
    output << "   " << format_text("*", BLUE) << " dexarb -scan 56 100000      " << format_text("Scan BSC by network ID", CYAN) << '\n';
    output << "   " << format_text("*", BLUE) << " dexarb -scan polygon 15000  " << format_text("Scan Polygon (15k blocks)", CYAN) << '\n';
    output << "   " << format_text("*", BLUE) << " dexarb -scan 137 75000      " << format_text("Scan Polygon by network ID", CYAN) << '\n';
    output << "   " << format_text("*", BLUE) << " dexarb -scan avalanche 30000" << format_text("Scan Avalanche (30k blocks)", CYAN) << '\n';
    output << "   " << format_text("*", BLUE) << " dexarb -scan 43114 50000   " << format_text("Scan Avalanche by network ID", CYAN) << '\n\n';
    
    output << format_text("DEX Analysis", GREEN) << '\n';
    output << "   " << format_text("*", BLUE) << " dexarb -dexes               " << format_text("Show all known DEXes by blockchain", CYAN) << '\n';
    output << "   " << format_text("*", BLUE) << " dexarb -showDEXES fantom    " << format_text("Show all Fantom DEXes", CYAN) << '\n';
    output << "   " << format_text("*", BLUE) << " dexarb -showDEXES 250       " << format_text("Show Fantom DEXes by network ID", CYAN) << '\n';
    output << "   " << format_text("*", BLUE) << " dexarb -showPOOLS ethereum  " << format_text("Show all Ethereum pools", CYAN) << '\n';
    output << "   " << format_text("*", BLUE) << " dexarb -showPOOLS ethereum 0x5C69bEe701ef814a2B6a3EDD4B1652CB9cc5aA6f" << format_text("Show Uniswap V2 pools", CYAN) << '\n';
    output << "   " << format_text("*", BLUE) << " dexarb -showTOKENS bsc      " << format_text("Show all BSC tokens", CYAN) << '\n';
    output << "   " << format_text("*", BLUE) << " dexarb -showTOKENS bsc 0x10ED43C718714eb63d5aA57B78B54704E256024E" << format_text("Show PancakeSwap tokens", CYAN) << '\n\n';
    
    output << format_text("Token Search", GREEN) << '\n';
    output << "   " << format_text("*", BLUE) << " dexarb -findTOKEN ethereum 0x5C69bEe701ef814a2B6a3EDD4B1652CB9cc5aA6f 0xA0b86a33E6441b8C4C8C8C8C8C8C8C8C8C8C8C8C8" << format_text("Find token in specific DEX", CYAN) << '\n';
    output << "   " << format_text("*", BLUE) << " dexarb -findTOKENS fantom 0x04068DA6C83AFCFA0e13ba15A6696662335D5B75" << format_text("Find USDC across all Fantom DEXes", CYAN) << '\n';
    output << "   " << format_text("*", BLUE) << " dexarb -findTOKENS 250 0x04068DA6C83AFCFA0e13ba15A6696662335D5B75" << format_text("Find USDC by network ID", CYAN) << '\n\n';
    
    output << format_text("Configuration & Statistics", GREEN) << '\n';
    output << "   " << format_text("*", BLUE) << " dexarb -showSCAN-CONFIG fantom" << format_text("Show Fantom scan configuration", CYAN) << '\n';
    output << "   " << format_text("*", BLUE) << " dexarb -showSCAN fantom      " << format_text("Show Fantom scan results", CYAN) << '\n';
    output << "   " << format_text("*", BLUE) << " dexarb -showSCAN-STAT fantom " << format_text("Show Fantom scan statistics", CYAN) << '\n\n';
    
    output << format_text("Supported Network IDs", GREEN) << '\n';
    output << "   " << format_text("*", BLUE) << " 1        " << format_text("Ethereum Mainnet", CYAN) << '\n';
    output << "   " << format_text("*", BLUE) << " 56       " << format_text("Binance Smart Chain", CYAN) << '\n';
    output << "   " << format_text("*", BLUE) << " 137      " << format_text("Polygon (Matic)", CYAN) << '\n';
    output << "   " << format_text("*", BLUE) << " 250      " << format_text("Fantom Opera", CYAN) << '\n';
    output << "   " << format_text("*", BLUE) << " 43114    " << format_text("Avalanche C-Chain", CYAN) << '\n';
    output << "   " << format_text("*", BLUE) << " 101      " << format_text("Solana (limited support)", CYAN) << '\n\n';
    
    output << format_text("Pro Tips", YELLOW) << '\n';
    output << "   " << format_text("*", BLUE) << " Use network IDs for faster typing and automation\n";
    output << "   " << format_text("*", BLUE) << " Start with small block ranges (1000-5000) for testing\n";
    output << "   " << format_text("*", BLUE) << " Use 10,000-50,000 blocks for discovery\n";
    output << "   " << format_text("*", BLUE) << " 100,000+ blocks for deep analysis\n";
    output << "   " << format_text("*", BLUE) << " Default blockchain is Fantom (250) for -scan command\n\n";
    
    output << format_text("Built for blockchain arbitrage opportunities", GREEN) << '\n';
    output << "\n";
}

void HelpDisplay::show_error(const std::string& message, std::ostream& output) {
    output << RED << "ERROR: " << message << RESET << '\n';
}

void HelpDisplay::show_info(const std::string& message, std::ostream& output) {
    output << BLUE << "INFO: " << message << RESET << '\n';
}

void HelpDisplay::show_usage_examples(std::ostream& output) {
    output << format_text("QUICK EXAMPLES", YELLOW) << '\n';
    output << "   " << format_text("*", BLUE) << " dexarb -h                              " << format_text("Show this help (includes examples)", CYAN) << '\n';
    output << "   " << format_text("*", BLUE) << " dexarb -v                              " << format_text("Show version", CYAN) << '\n';
    output << "   " << format_text("*", BLUE) << " dexarb -examples                       " << format_text("Show detailed examples", CYAN) << '\n';
    output << "   " << format_text("*", BLUE) << " dexarb -dexes                          " << format_text("Show all known DEXes", CYAN) << '\n';
    output << "   " << format_text("*", BLUE) << " dexarb -scan fantom                    " << format_text("Scan Fantom (default 1000)", CYAN) << '\n';
    output << "   " << format_text("*", BLUE) << " dexarb -scan ethereum 5000            " << format_text("Scan Ethereum", CYAN) << '\n';
    output << "   " << format_text("*", BLUE) << " dexarb -showDEXES bsc                 " << format_text("Show BSC DEXes", CYAN) << '\n';
    output << "   " << format_text("*", BLUE) << " dexarb -scan 250 10000                " << format_text("Scan Fantom by network ID", CYAN) << '\n\n';
}

void HelpDisplay::show_command_flags(std::ostream& output) {
    output << format_text("COMMAND FLAGS", YELLOW) << '\n';
    
    output << "   " << format_text("Help & Info", GREEN) << '\n';
    output << "      " << format_text("-h, -help", BLUE) << "                    " << format_text("Show this help message (includes examples)", CYAN) << '\n';
    output << "      " << format_text("-v, -version", BLUE) << "                 " << format_text("Show application version", CYAN) << '\n';
    output << "      " << format_text("-examples", BLUE) << "                    " << format_text("Show detailed examples", CYAN) << '\n';
    output << "      " << format_text("--verbose", BLUE) << "                    " << format_text("Enable debug output", CYAN) << '\n\n';
    
    output << "   " << format_text("Scanning & Discovery", GREEN) << '\n';
    output << "      " << format_text("-scan", BLUE) << "                        " << format_text("<blockchain> <blocks>     Scan last <blocks> (1000-1000000)", CYAN) << '\n';
    output << "      " << format_text("-showSCAN-CONFIG", BLUE) << "             " << format_text("<blockchain>           Show config settings", CYAN) << '\n';
    output << "      " << format_text("-showSCAN", BLUE) << "                     " << format_text("<blockchain>              Show scan results", CYAN) << '\n';
    output << "      " << format_text("-showSCAN-STAT", BLUE) << "                " << format_text("<blockchain>              Show scan stats", CYAN) << '\n\n';
    
    output << "   " << format_text("DEX Analysis", GREEN) << '\n';
    output << "      " << format_text("-dexes", BLUE) << "                        " << format_text("                              Show all known DEXes by blockchain", CYAN) << '\n';
    output << "      " << format_text("-showDEXES", BLUE) << "                    " << format_text("<blockchain>              Show discovered DEXes", CYAN) << '\n';
    output << "      " << format_text("-showPOOLS", BLUE) << "                    " << format_text("<blockchain> [DEX]        Show pools (all or specific DEX)", CYAN) << '\n';
    output << "      " << format_text("-showTOKENS", BLUE) << "                   " << format_text("<blockchain> [DEX]        Show tokens (all or specific DEX)", CYAN) << '\n\n';
    
    output << "   " << format_text("Token Search", GREEN) << '\n';
    output << "      " << format_text("-findTOKEN", BLUE) << "                    " << format_text("<blockchain> <DEX> <token> Find token in DEX", CYAN) << '\n';
    output << "      " << format_text("-findTOKENS", BLUE) << "                   " << format_text("<blockchain> <token>       Find token across DEXes", CYAN) << '\n\n';
}

void HelpDisplay::show_performance_tips(std::ostream& output) {
    output << format_text("PERFORMANCE TIPS", YELLOW) << '\n';
    output << "   " << format_text("*", BLUE) << " Start with small block ranges (1000-5000) for testing\n";
    output << "   " << format_text("*", BLUE) << " Use 10,000-50,000 blocks for discovery\n";
    output << "   " << format_text("*", BLUE) << " 100,000+ blocks for deep analysis\n";
    output << "   " << format_text("*", BLUE) << " Use network IDs (250 for Fantom, 1 for Ethereum) for faster typing\n\n";
}

void HelpDisplay::show_supported_blockchains(std::ostream& output) {
    output << format_text("SUPPORTED BLOCKCHAINS", YELLOW) << '\n';
    output << "   +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+\n";
    output << "   | " << format_text("Ethereum (1)", BLUE) << " | " << format_text("Fantom (250)", BLUE) << " | " << format_text("BSC (56)", BLUE) << " | " << format_text("Polygon (137)", BLUE) << " |\n";
    output << "   | " << format_text("Avalanche (43114)", BLUE) << " | " << format_text("Solana (101)", BLUE) << " |\n";
    output << "   +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+\n\n";
}

void HelpDisplay::show_banner(std::ostream& output) {
    output << GREEN << "+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+\n";
    output << "|                                                                             |\n";
    output << "|                    DEX Arbitrage Scanner v" << get_project_version() << "                    |\n";
    output << "|                                                                             |\n";
    output << "+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+" << RESET << '\n';
}

void HelpDisplay::show_separator(std::ostream& output) {
    output << std::string(80, '=') << '\n';
}

std::string HelpDisplay::format_text(const std::string& text, const std::string& color) {
    return color + text + RESET;
}

std::string HelpDisplay::format_column(const std::string& text, int width) {
    std::string result = text;
    while (result.length() < width) {
        result += " ";
    }
    return result;
}

std::string HelpDisplay::get_project_version() {
    return "1.0.7";
}

} // namespace cli
