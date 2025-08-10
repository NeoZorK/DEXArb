//
//  help_display.h
//  NeoZorKDEXArb
//
//  Created by Rostyslav S. on 10.08.2025.
//
#ifndef HELP_DISPLAY_H
#define HELP_DISPLAY_H

#include <string>
#include <string_view>
#include <ostream>
#include <iostream>
#include <vector>

namespace cli {

// Help display class
class HelpDisplay {
public:
    // Show main help information
    static void show_help(std::ostream& output = std::cout);
    
    // Show version information
    static void show_version(std::ostream& output = std::cout);
    
    // Show usage examples
    static void show_usage_examples(std::ostream& output = std::cout);
    
    // Show supported blockchains
    static void show_supported_blockchains(std::ostream& output = std::cout);
    
    // Show command flags
    static void show_command_flags(std::ostream& output = std::cout);
    
    // Show performance tips
    static void show_performance_tips(std::ostream& output = std::cout);
    
    // Show error message
    static void show_error(std::string_view error_message, std::ostream& output = std::cout);
    
    // Show success message
    static void show_success(std::string_view message, std::ostream& output = std::cout);
    
    // Show info message
    static void show_info(std::string_view message, std::ostream& output = std::cout);
    
    // Show warning message
    static void show_warning(std::string_view message, std::ostream& output = std::cout);
    
    // Show progress bar
    static void show_progress_bar(int current, int total, std::string_view label = "", 
                                 std::ostream& output = std::cout);
    
    // Show table header
    static void show_table_header(const std::vector<std::string>& headers, 
                                 std::ostream& output = std::cout);
    
    // Show table row
    static void show_table_row(const std::vector<std::string>& values, 
                              std::ostream& output = std::cout);
    
    // Show separator line
    static void show_separator(std::ostream& output = std::cout);
    
    // Show banner
    static void show_banner(std::ostream& output = std::cout);
    
private:
    // Get project version
    static std::string get_project_version();
    
    // Get build date
    static std::string get_build_date();
    
    // Get compiler info
    static std::string get_compiler_info();
    
    // Format text with colors
    static std::string format_text(std::string_view text, std::string_view color);
    
    // Calculate table column widths
    static std::vector<size_t> calculate_column_widths(
        const std::vector<std::vector<std::string>>& rows);
};

} // namespace cli

#endif // HELP_DISPLAY_H
