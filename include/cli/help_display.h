//
//  help_display.h
//  NeoZorKDEXArb
//
//  Created by Rostyslav S. on 10.08.2025.
//
#ifndef HELP_DISPLAY_H
#define HELP_DISPLAY_H

#include <string>
#include <ostream>
#include <iostream>

namespace cli {

class HelpDisplay {
public:
    // Show comprehensive help information
    static void show_help(std::ostream& output = std::cout);
    
    // Show version information
    static void show_version(std::ostream& output = std::cout);
    
    // Show detailed examples for all commands
    static void show_examples(std::ostream& output = std::cout);
    
    // Show error message
    static void show_error(const std::string& message, std::ostream& output = std::cerr);
    
    // Show info message
    static void show_info(const std::string& message, std::ostream& output = std::cout);

private:
    // Helper methods for formatting
    static void show_banner(std::ostream& output);
    static void show_separator(std::ostream& output);
    static void show_supported_blockchains(std::ostream& output);
    static void show_command_flags(std::ostream& output);
    static void show_usage_examples(std::ostream& output);
    static void show_performance_tips(std::ostream& output);
    
    // Format text with color
    static std::string format_text(const std::string& text, const std::string& color);
    
    // Get project version
    static std::string get_project_version();
};

} // namespace cli

#endif // HELP_DISPLAY_H
