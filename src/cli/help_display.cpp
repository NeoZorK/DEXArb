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
    output << "   neozork [flag] <blockchain> [value]\n\n";
    
    show_supported_blockchains(output);
    show_command_flags(output);
    show_usage_examples(output);
    show_performance_tips(output);
    
    show_separator(output);
    output << format_text("Built for blockchain arbitrage opportunities", GREEN) << '\n';
    show_separator(output);
    output << "\n";
}

void HelpDisplay::show_version(std::ostream& output) {
    show_banner(output);
    
    output << "\n";
    output << format_text("BUILD INFORMATION", YELLOW) << '\n';
    output << "   " << format_text("•", BLUE) << " Version: " << format_text(get_project_version(), GREEN) << '\n';
    output << "   " << format_text("•", BLUE) << " Purpose: Blockchain arbitrage opportunities detection\n";
    output << "   " << format_text("•", BLUE) << " Language: C++23 (Modern C++)\n";
    output << "   " << format_text("•", BLUE) << " Platform: Cross-platform\n";
    output << "   " << format_text("•", BLUE) << " Features: Modern logging, JSON parsing, Error handling\n\n";
    
    show_separator(output);
    output << format_text("Built for blockchain arbitrage opportunities", GREEN) << '\n';
    show_separator(output);
    output << "\n";
}

void HelpDisplay::show_examples(std::ostream& output) {
    show_banner(output);
    
    output << "\n";
    output << format_text("DETAILED EXAMPLES", YELLOW) << '\n';
    output << "   Modern, aligned examples for all commands\n\n";
    
    output << format_text("Help & Information", GREEN) << '\n';
    output << "   " << format_text("*", BLUE) << " neozork -h                    " << format_text("Show help", CYAN) << '\n';
    output << "   " << format_text("*", BLUE) << " neozork -help                 " << format_text("Show help (alternative)", CYAN) << '\n';
    output << "   " << format_text("*", BLUE) << " neozork -v                    " << format_text("Show version", CYAN) << '\n';
    output << "   " << format_text("*", BLUE) << " neozork -version              " << format_text("Show version (alternative)", CYAN) << '\n';
    output << "   " << format_text("*", BLUE) << " neozork -examples             " << format_text("Show detailed examples", CYAN) << '\n\n';
    
    output << format_text("Scanning & Discovery", GREEN) << '\n';
    output << "   " << format_text("*", BLUE) << " neozork -scan fantom         " << format_text(" Scan Fantom (default 1000 blocks)", CYAN) << '\n';
    output << "   " << format_text("*", BLUE) << " neozork -scan 250 5000       " << format_text(" Scan Fantom by network ID", CYAN) << '\n';
    output << "   " << format_text("*", BLUE) << " neozork -scan ethereum 10000 " << format_text(" Scan Ethereum (10k blocks)", CYAN) << '\n';
    output << "   " << format_text("*", BLUE) << " neozork -scan 1 50000        " << format_text(" Scan Ethereum by network ID", CYAN) << '\n';
    output << "   " << format_text("*", BLUE) << " neozork -scan bsc 25000      " << format_text(" Scan BSC (25k blocks)", CYAN) << '\n';
    output << "   " << format_text("*", BLUE) << " neozork -scan 56 100000      " << format_text(" Scan BSC by network ID", CYAN) << '\n';
    output << "   " << format_text("*", BLUE) << " neozork -scan polygon 15000  " << format_text(" Scan Polygon (15k blocks)", CYAN) << '\n';
    output << "   " << format_text("*", BLUE) << " neozork -scan 137 75000      " << format_text(" Scan Polygon by network ID", CYAN) << '\n';
    output << "   " << format_text("*", BLUE) << " neozork -scan avalanche 30000" << format_text(" Scan Avalanche (30k blocks)", CYAN) << '\n';
    output << "   " << format_text("*", BLUE) << " neozork -scan 43114 50000   " << format_text(" Scan Avalanche by network ID", CYAN) << '\n\n';
    
    output << format_text("DEX Analysis", GREEN) << '\n';
    output << "   " << format_text("*", BLUE) << " neozork -showDEXES fantom    " << format_text("Show all Fantom DEXes", CYAN) << '\n';
    output << "   " << format_text("*", BLUE) << " neozork -showDEXES 250       " << format_text("Show Fantom DEXes by network ID", CYAN) << '\n';
    output << "   " << format_text("*", BLUE) << " neozork -showPOOLS ethereum 0x5C69bEe701ef814a2B6a3EDD4B1652CB9cc5aA6f" << format_text("Show Uniswap V2 pools", CYAN) << '\n';
    output << "   " << format_text("*", BLUE) << " neozork -showTOKENS bsc 0x10ED43C718714eb63d5aA57B78B54704E256024E" << format_text("Show PancakeSwap tokens", CYAN) << '\n\n';
    
    output << format_text("Token Search", GREEN) << '\n';
    output << "   " << format_text("*", BLUE) << " neozork -findTOKEN ethereum 0x5C69bEe701ef814a2B6a3EDD4B1652CB9cc5aA6f 0xA0b86a33E6441b8C4C8C8C8C8C8C8C8C8C8C8C8C8" << format_text(" Find token in specific DEX", CYAN) << '\n';
    output << "   " << format_text("*", BLUE) << " neozork -findTOKENS fantom 0x04068DA6C83AFCFA0e13ba15A6696662335D5B75" << format_text(" Find USDC across all Fantom DEXes", CYAN) << '\n';
    output << "   " << format_text("*", BLUE) << " neozork -findTOKENS 250 0x04068DA6C83AFCFA0e13ba15A6696662335D5B75" << format_text(" Find USDC by network ID", CYAN) << '\n\n';
    
    output << format_text("Configuration & Statistics", GREEN) << '\n';
    output << "   " << format_text("*", BLUE) << " neozork -showSCAN-CONFIG fantom" << format_text("Show Fantom scan configuration", CYAN) << '\n';
    output << "   " << format_text("*", BLUE) << " neozork -showSCAN fantom      " << format_text("Show Fantom scan results", CYAN) << '\n';
    output << "   " << format_text("*", BLUE) << " neozork -showSCAN-STAT fantom " << format_text("Show Fantom scan statistics", CYAN) << '\n\n';
    
    output << format_text("Supported Network IDs", GREEN) << '\n';
    output << "   " << format_text("*", BLUE) << " 1        " << format_text("Ethereum Mainnet", CYAN) << '\n';
    output << "   " << format_text("*", BLUE) << " 56       " << format_text("Binance Smart Chain", CYAN) << '\n';
    output << "   " << format_text("*", BLUE) << " 137      " << format_text("Polygon (Matic)", CYAN) << '\n';
    output << "   " << format_text("*", BLUE) << " 250      " << format_text("Fantom Opera", CYAN) << '\n';
    output << "   " << format_text("*", BLUE) << " 43114    " << format_text("Avalanche C-Chain", CYAN) << '\n';
    output << "   " << format_text("*", BLUE) << " 101      " << format_text("Solana (limited support)", CYAN) << '\n\n';
    
    show_separator(output);
    output << format_text("Pro Tips", YELLOW) << '\n';
    output << "   " << format_text("*", BLUE) << " Use network IDs for faster typing and automation\n";
    output << "   " << format_text("*", BLUE) << " Start with small block ranges (1000-5000) for testing\n";
    output << "   " << format_text("*", BLUE) << " Use 10,000-50,000 blocks for discovery\n";
    output << "   " << format_text("*", BLUE) << " 100,000+ blocks for deep analysis\n";
    output << "   " << format_text("*", BLUE) << " Default blockchain is Fantom (250) for -scan command\n\n";
    
    show_separator(output);
    output << format_text("Built for blockchain arbitrage opportunities", GREEN) << '\n';
    show_separator(output);
    output << "\n";
}

void HelpDisplay::show_error(const std::string& message, std::ostream& output) {
    output << RED << "ERROR: " << message << RESET << '\n';
}

void HelpDisplay::show_info(const std::string& message, std::ostream& output) {
    output << BLUE << "INFO: " << message << RESET << '\n';
}

void HelpDisplay::show_usage_examples(std::ostream& output) {
    output << format_text("EXAMPLES", YELLOW) << '\n';
    output << "   " << format_text("*", BLUE) << " neozork -h                              " << format_text("Show this help", CYAN) << '\n';
    output << "   " << format_text("*", BLUE) << " neozork -v                              " << format_text("Show version", CYAN) << '\n';
    output << "   " << format_text("*", BLUE) << " neozork -examples                       " << format_text("Show detailed examples", CYAN) << '\n';
    output << "   " << format_text("*", BLUE) << " neozork -scan fantom                    " << format_text(" Scan Fantom (default 1000)", CYAN) << '\n';
    output << "   " << format_text("*", BLUE) << " neozork -scan ethereum 5000            " << format_text(" Scan Ethereum", CYAN) << '\n';
    output << "   " << format_text("*", BLUE) << " neozork -showDEXES bsc                 " << format_text("Show BSC DEXes", CYAN) << '\n';
    output << "   " << format_text("*", BLUE) << " neozork -scan 250 10000                " << format_text(" Scan Fantom by network ID", CYAN) << '\n\n';
}

void HelpDisplay::show_command_flags(std::ostream& output) {
    output << format_text("COMMAND FLAGS", YELLOW) << '\n';
    output << "   " << format_text("Help & Info", GREEN) << '\n';
    output << "      " << format_text("-h, -help", BLUE) << "                    " << format_text("Show this help message", CYAN) << '\n';
    output << "      " << format_text("-v, -version", BLUE) << "                 " << format_text("Show application version", CYAN) << '\n';
    output << "      " << format_text("-examples", BLUE) << "                    " << format_text("Show detailed examples", CYAN) << '\n\n';
    
    output << "   " << format_text("Scanning & Discovery", GREEN) << '\n';
    output << "      " << format_text("-scan", BLUE) << format_column("", 8) << "<blockchain> <blocks>     " << format_text("Scan last <blocks> (1000-1000000)", CYAN) << '\n';
    output << "      " << format_text("-showSCAN-CONFIG", BLUE) << format_column("", 1) << "<blockchain>           " << format_text("Show config settings", CYAN) << '\n';
    output << "      " << format_text("-showSCAN", BLUE) << format_column("", 5) << "<blockchain>              " << format_text("Show scan results", CYAN) << '\n';
    output << "      " << format_text("-showSCAN-STAT", BLUE) << format_column("", 2) << "<blockchain>              " << format_text("Show scan stats", CYAN) << '\n\n';
    
    output << "   " << format_text("DEX Analysis", GREEN) << '\n';
    output << "      " << format_text("-showDEXES", BLUE) << format_column("", 4) << "<blockchain>              " << format_text("Show all DEXes", CYAN) << '\n';
    output << "      " << format_text("-showPOOLS", BLUE) << format_column("", 4) << "<blockchain> <DEX>         " << format_text("Show DEX pools", CYAN) << '\n';
    output << "      " << format_text("-showTOKENS", BLUE) << format_column("", 3) << "<blockchain> <DEX>         " << format_text("Show DEX tokens", CYAN) << '\n\n';
    
    output << "   " << format_text("Token Search", GREEN) << '\n';
    output << "      " << format_text("-findTOKEN", BLUE) << format_column("", 4) << "<blockchain> <DEX> <token> " << format_text("Find token in DEX", CYAN) << '\n';
    output << "      " << format_text("-findTOKENS", BLUE) << format_column("", 3) << "<blockchain> <token>       " << format_text("Find token across DEXes", CYAN) << '\n\n';
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
    output << "   " << format_text("•", BLUE) << " Ethereum (1)    " << format_text("•", BLUE) << " Fantom (250)      " << format_text("•", BLUE) << " BSC (56)\n";
    output << "   " << format_text("•", BLUE) << " Polygon (137)   " << format_text("•", BLUE) << " Avalanche (43114)  " << format_text("•", BLUE) << " Solana (101)\n\n";
}

void HelpDisplay::show_banner(std::ostream& output) {
    output << GREEN << "DEX Arbitrage Scanner v" << get_project_version() << RESET << '\n';
}

void HelpDisplay::show_separator(std::ostream& output) {
    output << std::string(60, '=') << '\n';
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
