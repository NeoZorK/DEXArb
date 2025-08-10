//
//  modern_utils.h
//  NeoZorKDEXArb
//
//  Created by Rostyslav S. on 18.03.2025.
//

#ifndef MODERN_UTILS_H
#define MODERN_UTILS_H

#include <string>
#include <iostream>
#include <sstream>
#include <memory>

namespace modern_utils {

// Simple logger class
class Logger {
public:
    enum class Level {
        DEBUG,
        INFO,
        WARNING,
        ERROR
    };
    
    static void log(Level level, const std::string& message) {
        std::string level_str;
        switch (level) {
            case Level::DEBUG:   level_str = "DEBUG"; break;
            case Level::INFO:    level_str = "INFO"; break;
            case Level::WARNING: level_str = "WARNING"; break;
            case Level::ERROR:   level_str = "ERROR"; break;
        }
        
        std::cout << "[" << level_str << "] " << message << std::endl;
    }
    
    static void debug(const std::string& message) { log(Level::DEBUG, message); }
    static void info(const std::string& message) { log(Level::INFO, message); }
    static void warning(const std::string& message) { log(Level::WARNING, message); }
    static void error(const std::string& message) { log(Level::ERROR, message); }
};

// Simple JSON parser utilities
class JsonUtils {
public:
    // Extract value from JSON string by key
    static std::string extract_value(const std::string& json, const std::string& key) {
        std::string pattern = "\"" + key + "\":";
        size_t pos = json.find(pattern);
        if (pos == std::string::npos) {
            return "";
        }
        
        pos += pattern.length();
        
        // Skip whitespace
        while (pos < json.length() && (json[pos] == ' ' || json[pos] == '\t' || json[pos] == '\n')) {
            pos++;
        }
        
        if (pos >= json.length()) {
            return "";
        }
        
        // Handle string values
        if (json[pos] == '"') {
            pos++; // Skip opening quote
            size_t end_pos = json.find('"', pos);
            if (end_pos == std::string::npos) {
                return "";
            }
            return json.substr(pos, end_pos - pos);
        }
        
        // Handle numeric values
        size_t end_pos = pos;
        while (end_pos < json.length() && 
               ((json[end_pos] >= '0' && json[end_pos] <= '9') || 
                (json[end_pos] >= 'a' && json[end_pos] <= 'f') || 
                (json[end_pos] >= 'A' && json[end_pos] <= 'F') || 
                json[end_pos] == 'x' || json[end_pos] == '.')) {
            end_pos++;
        }
        
        return json.substr(pos, end_pos - pos);
    }
    
    // Check if JSON contains a key
    static bool has_key(const std::string& json, const std::string& key) {
        std::string pattern = "\"" + key + "\":";
        return json.find(pattern) != std::string::npos;
    }
    
    // Validate JSON structure (basic)
    static bool is_valid_json(const std::string& json) {
        if (json.empty()) return false;
        
        // Check for basic JSON structure
        size_t open_braces = 0;
        size_t open_brackets = 0;
        bool in_string = false;
        bool escaped = false;
        
        for (char c : json) {
            if (escaped) {
                escaped = false;
                continue;
            }
            
            if (c == '\\') {
                escaped = true;
                continue;
            }
            
            if (c == '"' && !escaped) {
                in_string = !in_string;
                continue;
            }
            
            if (!in_string) {
                if (c == '{') open_braces++;
                else if (c == '}') open_braces--;
                else if (c == '[') open_brackets++;
                else if (c == ']') open_brackets--;
            }
        }
        
        return open_braces == 0 && open_brackets == 0 && !in_string;
    }
};

// String utilities
class StringUtils {
public:
    // Convert to string with any type
    template<typename T>
    static std::string to_string(const T& value) {
        std::ostringstream oss;
        oss << value;
        return oss.str();
    }
    
    // Check if string is empty or whitespace only
    static bool is_empty_or_whitespace(const std::string& str) {
        return str.empty() || str.find_first_not_of(" \t\n\r") == std::string::npos;
    }
    
    // Remove quotes from string
    static std::string remove_quotes(const std::string& str) {
        if (str.length() >= 2 && str.front() == '"' && str.back() == '"') {
            return str.substr(1, str.length() - 2);
        }
        return str;
    }
};

} // namespace modern_utils

#endif // MODERN_UTILS_H
