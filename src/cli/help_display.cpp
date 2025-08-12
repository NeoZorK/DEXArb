//
//  help_display.cpp
//  NeoZorKDEXArb
//
//  Created by Rostyslav S. on 10.08.2025.
//
#include "cli/help_display.h"
#include "main.h"
#include <iomanip>
#include <sstream>

namespace cli {

void HelpDisplay::show_help(std::ostream& output) {
    show_banner(output);
    
    output << "\n";
    output << format_text("📖 USAGE", YELLOW) << '\n';
    output << "   neozork [flag] <blockchain> [value]\n\n";
    
    show_supported_blockchains(output);
    show_command_flags(output);
    show_usage_examples(output);
    show_performance_tips(output);
    
    show_separator(output);
    output << format_text("🔗 Built for blockchain arbitrage opportunities", GREEN) << '\n';
    show_separator(output);
    output << "\n";
}

void HelpDisplay::show_version(std::ostream& output) {
    show_banner(output);
    
    output << "\n";
    output << format_text("📋 BUILD INFORMATION", YELLOW) << '\n';
    output << "   " << format_text("•", BLUE) << " Version: " << format_text(get_project_version(), GREEN) << '\n';
    output << "   " << format_text("•", BLUE) << " Purpose: Blockchain arbitrage opportunities detection\n";
    output << "   " << format_text("•", BLUE) << " Language: C++23 (Modern C++)\n";
    output << "   " << format_text("•", BLUE) << " Platform: Cross-platform\n";
    output << "   " << format_text("•", BLUE) << " Features: Modern logging, JSON parsing, Error handling\n\n";
    
    show_separator(output);
    output << format_text("🔗 Built for blockchain arbitrage opportunities", GREEN) << '\n';
    show_separator(output);
    output << "\n";
}

void HelpDisplay::show_usage_examples(std::ostream& output) {
    output << format_text("💡 EXAMPLES", YELLOW) << '\n';
    output << "   " << format_text("•", BLUE) << " neozork -h                              # Show this help\n";
    output << "   " << format_text("•", BLUE) << " neozork -v                              # Show version\n";
    output << "   " << format_text("•", BLUE) << " neozork -scan ethereum 5000            # Scan Ethereum\n";
    output << "   " << format_text("•", BLUE) << " neozork -showDEXES bsc                 # Show BSC DEXes\n\n";
}

void HelpDisplay::show_supported_blockchains(std::ostream& output) {
    output << format_text("🌐 SUPPORTED BLOCKCHAINS", YELLOW) << '\n';
    output << "   " << format_text("•", BLUE) << " Ethereum    " << format_text("•", BLUE) << " Fantom      " << format_text("•", BLUE) << " BSC\n";
    output << "   " << format_text("•", BLUE) << " Polygon     " << format_text("•", BLUE) << " Avalanche   " << format_text("•", BLUE) << " Solana\n\n";
}

void HelpDisplay::show_command_flags(std::ostream& output) {
    output << format_text("🎯 COMMAND FLAGS", YELLOW) << '\n';
    output << "   " << format_text("📋 Help & Info", GREEN) << '\n';
    output << "      " << format_text("-h, -help", BLUE) << "     Show this help message\n";
    output << "      " << format_text("-v, -version", BLUE) << "  Show application version\n\n";
    
    output << "   " << format_text("🔍 Scanning & Discovery", GREEN) << '\n';
    output << "      " << format_text("-scan", BLUE) << "        <blockchain> <blocks>     Scan last <blocks> (1000-1000000)\n";
    output << "      " << format_text("-showSCAN-CONFIG", BLUE) << " <blockchain>           Show config settings\n";
    output << "      " << format_text("-showSCAN", BLUE) << "     <blockchain>              Show scan results\n";
    output << "      " << format_text("-showSCAN-STAT", BLUE) << " <blockchain>              Show scan stats\n\n";
    
    output << "   " << format_text("🏦 DEX Analysis", GREEN) << '\n';
    output << "      " << format_text("-showDEXES", BLUE) << "    <blockchain>              Show all DEXes\n";
    output << "      " << format_text("-showPOOLS", BLUE) << "    <blockchain> <DEX>         Show DEX pools\n";
    output << "      " << format_text("-showTOKENS", BLUE) << "   <blockchain> <DEX>         Show DEX tokens\n\n";
    
    output << "   " << format_text("🔎 Token Search", GREEN) << '\n';
    output << "      " << format_text("-findTOKEN", BLUE) << "    <blockchain> <DEX> <token> Find token in DEX\n";
    output << "      " << format_text("-findTOKENS", BLUE) << "   <blockchain> <token>       Find token across DEXes\n\n";
}

void HelpDisplay::show_performance_tips(std::ostream& output) {
    output << format_text("⚡ PERFORMANCE TIPS", YELLOW) << '\n';
    output << "   " << format_text("•", BLUE) << " Start with small block ranges (1000-5000) for testing\n";
    output << "   " << format_text("•", BLUE) << " Use 10,000-50,000 blocks for discovery\n";
    output << "   " << format_text("•", BLUE) << " 100,000+ blocks for deep analysis\n\n";
}

void HelpDisplay::show_error(std::string_view error_message, std::ostream& output) {
    output << format_text("ERROR: ", RED) << error_message << '\n';
}

void HelpDisplay::show_success(std::string_view message, std::ostream& output) {
    output << format_text("SUCCESS: ", GREEN) << message << '\n';
}

void HelpDisplay::show_info(std::string_view message, std::ostream& output) {
    output << format_text("INFO: ", BLUE) << message << '\n';
}

void HelpDisplay::show_warning(std::string_view message, std::ostream& output) {
    output << format_text("WARNING: ", YELLOW) << message << '\n';
}

void HelpDisplay::show_progress_bar(int current, int total, std::string_view label, std::ostream& output) {
    const int bar_width = 50;
    float progress = static_cast<float>(current) / total;
    int pos = static_cast<int>(bar_width * progress);
    
    output << "\r" << label << " [";
    for (int i = 0; i < bar_width; ++i) {
        if (i < pos) output << "=";
        else if (i == pos) output << ">";
        else output << " ";
    }
    output << "] " << std::fixed << std::setprecision(1) << (progress * 100.0) << "%";
    output.flush();
}

void HelpDisplay::show_table_header(const std::vector<std::string>& headers, std::ostream& output) {
    output << "| ";
    for (const auto& header : headers) {
        output << format_text(header, YELLOW) << " | ";
    }
    output << '\n';
    
    // Separator line
    output << "|";
    for (size_t i = 0; i < headers.size(); ++i) {
        output << "---|";
    }
    output << '\n';
}

void HelpDisplay::show_table_row(const std::vector<std::string>& values, std::ostream& output) {
    output << "| ";
    for (const auto& value : values) {
        output << value << " | ";
    }
    output << '\n';
}

void HelpDisplay::show_separator(std::ostream& output) {
    output << '\n';
}

void HelpDisplay::show_banner(std::ostream& output) {
    output << "\n";
    output << format_text("🚀 DEX Arbitrage Scanner v" + get_project_version(), GREEN) << '\n';
}

std::string HelpDisplay::get_project_version() {
    return "1.0.7";
}

std::string HelpDisplay::get_build_date() {
    return __DATE__ " " __TIME__;
}

std::string HelpDisplay::get_compiler_info() {
    return "C++23";
}

std::string HelpDisplay::format_text(std::string_view text, std::string_view color) {
    return std::string(color) + std::string(text) + RESET;
}

std::vector<size_t> HelpDisplay::calculate_column_widths(const std::vector<std::vector<std::string>>& rows) {
    if (rows.empty()) return {};
    
    std::vector<size_t> widths(rows[0].size(), 0);
    
    for (const auto& row : rows) {
        for (size_t i = 0; i < row.size() && i < widths.size(); ++i) {
            widths[i] = std::max(widths[i], row[i].length());
        }
    }
    
    return widths;
}

} // namespace cli
