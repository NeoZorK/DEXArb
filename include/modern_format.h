//
//  modern_format.h
//  NeoZorKDEXArb
//
//  Created by Rostyslav S. on 10.08.2025.
//
#ifndef MODERN_FORMAT_H
#define MODERN_FORMAT_H

#include <string>
#include <string_view>
#include <sstream>
#include <iomanip>
#include <chrono>

namespace modern {

// Modern formatting utilities
class Format {
public:
    // Format string with arguments (C++20 style)
    template<typename... Args>
    static std::string format(std::string_view fmt, Args&&... args) {
        std::ostringstream oss;
        format_impl(oss, fmt, std::forward<Args>(args)...);
        return oss.str();
    }
    
    // Format with color codes
    template<typename... Args>
    static std::string format_colored(std::string_view color, std::string_view fmt, Args&&... args) {
        return color + format(fmt, std::forward<Args>(args)...) + "\033[0m";
    }
    
    // Format number with thousands separator
    static std::string format_number(uint64_t number) {
        std::ostringstream oss;
        oss << std::fixed << std::setprecision(0) << number;
        std::string str = oss.str();
        
        // Add thousands separator
        for (int i = str.length() - 3; i > 0; i -= 3) {
            str.insert(i, ",");
        }
        return str;
    }
    
    // Format bytes to human readable format
    static std::string format_bytes(uint64_t bytes) {
        const char* units[] = {"B", "KB", "MB", "GB", "TB"};
        int unit_index = 0;
        double size = static_cast<double>(bytes);
        
        while (size >= 1024.0 && unit_index < 4) {
            size /= 1024.0;
            unit_index++;
        }
        
        std::ostringstream oss;
        if (unit_index == 0) {
            oss << static_cast<int>(size) << " " << units[unit_index];
        } else {
            oss << std::fixed << std::setprecision(2) << size << " " << units[unit_index];
        }
        return oss.str();
    }
    
    // Format duration
    template<typename Rep, typename Period>
    static std::string format_duration(const std::chrono::duration<Rep, Period>& duration) {
        auto ms = std::chrono::duration_cast<std::chrono::milliseconds>(duration);
        auto seconds = std::chrono::duration_cast<std::chrono::seconds>(duration);
        auto minutes = std::chrono::duration_cast<std::chrono::minutes>(duration);
        auto hours = std::chrono::duration_cast<std::chrono::hours>(duration);
        
        if (hours.count() > 0) {
            return format("{}h {}m {}s", hours.count(), minutes.count() % 60, seconds.count() % 60);
        } else if (minutes.count() > 0) {
            return format("{}m {}s", minutes.count(), seconds.count() % 60);
        } else if (seconds.count() > 0) {
            return format("{}s {}ms", seconds.count(), ms.count() % 1000);
        } else {
            return format("{}ms", ms.count());
        }
    }
    
    // Format percentage
    static std::string format_percentage(double value, int precision = 2) {
        std::ostringstream oss;
        oss << std::fixed << std::setprecision(precision) << (value * 100.0) << "%";
        return oss.str();
    }
    
    // Format progress bar
    static std::string format_progress_bar(double progress, int width = 50) {
        int filled = static_cast<int>(progress * width);
        std::string bar;
        bar.reserve(width + 10);
        
        bar += "[";
        for (int i = 0; i < width; ++i) {
            if (i < filled) {
                bar += "=";
            } else if (i == filled) {
                bar += ">";
            } else {
                bar += " ";
            }
        }
        bar += "] ";
        bar += format_percentage(progress);
        
        return bar;
    }
    
    // Format table row
    template<typename... Args>
    static std::string format_table_row(Args&&... args) {
        std::ostringstream oss;
        format_table_row_impl(oss, std::forward<Args>(args)...);
        return oss.str();
    }
    
    // Format address (truncate middle)
    static std::string format_address(std::string_view address, int max_length = 20) {
        if (address.length() <= max_length) {
            return std::string(address);
        }
        
        int prefix_length = (max_length - 3) / 2;
        int suffix_length = max_length - 3 - prefix_length;
        
        return format("{:.{}}...{:.{}}", 
                     address.substr(0, prefix_length), prefix_length,
                     address.substr(address.length() - suffix_length), suffix_length);
    }
    
    // Format timestamp
    static std::string format_timestamp(const std::chrono::system_clock::time_point& time_point) {
        auto time_t = std::chrono::system_clock::to_time_t(time_point);
        auto ms = std::chrono::duration_cast<std::chrono::milliseconds>(
            time_point.time_since_epoch()) % 1000;
        
        std::ostringstream oss;
        oss << std::put_time(std::localtime(&time_t), "%Y-%m-%d %H:%M:%S");
        oss << "." << std::setfill('0') << std::setw(3) << ms.count();
        return oss.str();
    }

private:
    // Implementation of format with variadic arguments
    static void format_impl(std::ostringstream& oss, std::string_view fmt) {
        oss << fmt;
    }
    
    template<typename T, typename... Args>
    static void format_impl(std::ostringstream& oss, std::string_view fmt, T&& value, Args&&... args) {
        size_t pos = fmt.find("{}");
        if (pos == std::string::npos) {
            oss << fmt;
            return;
        }
        
        oss << fmt.substr(0, pos);
        oss << std::forward<T>(value);
        format_impl(oss, fmt.substr(pos + 2), std::forward<Args>(args)...);
    }
    
    // Implementation of table row formatting
    static void format_table_row_impl(std::ostringstream& oss) {
        oss << "|";
    }
    
    template<typename T, typename... Args>
    static void format_table_row_impl(std::ostringstream& oss, T&& value, Args&&... args) {
        oss << " " << std::forward<T>(value) << " |";
        format_table_row_impl(oss, std::forward<Args>(args)...);
    }
};

// String utilities
class StringUtils {
public:
    // Join strings with separator
    template<typename Container>
    static std::string join(const Container& container, std::string_view separator) {
        std::ostringstream oss;
        auto it = container.begin();
        if (it != container.end()) {
            oss << *it;
            ++it;
        }
        for (; it != container.end(); ++it) {
            oss << separator << *it;
        }
        return oss.str();
    }
    
    // Split string by delimiter
    static std::vector<std::string> split(std::string_view str, std::string_view delimiter) {
        std::vector<std::string> result;
        size_t start = 0;
        size_t end = str.find(delimiter);
        
        while (end != std::string::npos) {
            result.emplace_back(str.substr(start, end - start));
            start = end + delimiter.length();
            end = str.find(delimiter, start);
        }
        
        result.emplace_back(str.substr(start));
        return result;
    }
    
    // Trim whitespace
    static std::string trim(std::string_view str) {
        size_t start = str.find_first_not_of(" \t\n\r");
        if (start == std::string::npos) {
            return "";
        }
        size_t end = str.find_last_not_of(" \t\n\r");
        return std::string(str.substr(start, end - start + 1));
    }
    
    // Convert to lowercase
    static std::string to_lower(std::string_view str) {
        std::string result(str);
        std::transform(result.begin(), result.end(), result.begin(), ::tolower);
        return result;
    }
    
    // Convert to uppercase
    static std::string to_upper(std::string_view str) {
        std::string result(str);
        std::transform(result.begin(), result.end(), result.begin(), ::toupper);
        return result;
    }
    
    // Check if string starts with prefix
    static bool starts_with(std::string_view str, std::string_view prefix) {
        return str.length() >= prefix.length() && 
               str.substr(0, prefix.length()) == prefix;
    }
    
    // Check if string ends with suffix
    static bool ends_with(std::string_view str, std::string_view suffix) {
        return str.length() >= suffix.length() && 
               str.substr(str.length() - suffix.length()) == suffix;
    }
    
    // Replace all occurrences
    static std::string replace_all(std::string_view str, std::string_view from, std::string_view to) {
        std::string result(str);
        size_t pos = 0;
        while ((pos = result.find(from, pos)) != std::string::npos) {
            result.replace(pos, from.length(), to);
            pos += to.length();
        }
        return result;
    }
};

} // namespace modern

#endif // MODERN_FORMAT_H
