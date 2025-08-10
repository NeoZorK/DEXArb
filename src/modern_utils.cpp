//
//  modern_utils.cpp
//  NeoZorKDEXArb
//
//  Created by Rostyslav S. on 26.02.2025.
//
#include "modern_utils.h"
#include <cctype>
#include <sstream>
#include <stdexcept>

namespace modern {

// Static member initialization for Logger
const char* Logger::level_strings[] = {"TRACE", "DEBUG", "INFO", "WARN", "ERROR", "FATAL"};
const char* Logger::level_colors[] = {
    "\033[36m",  // TRACE - Cyan
    "\033[34m",  // DEBUG - Blue
    "\033[32m",  // INFO - Green
    "\033[33m",  // WARN - Yellow
    "\033[31m",  // ERROR - Red
    "\033[35m"   // FATAL - Magenta
};

} // namespace modern

// Global logger instance
modern::Logger g_logger(modern::Logger::INFO);

namespace modern {

// JsonParser implementation
modern::JsonParser::JsonValue JsonParser::parse(const std::string& json) {
    size_t pos = 0;
    skip_whitespace(json, pos);
    return parse_value(json, pos);
}

std::string JsonParser::extract_field(const std::string& json, const std::string& field) {
    std::string field_key = "\"" + field + "\":\"";
    size_t start = json.find(field_key);
    if (start == std::string::npos) return "";
    
    start += field_key.length();
    size_t end = json.find('"', start);
    if (end == std::string::npos) return "";
    
    return json.substr(start, end - start);
}

modern::JsonParser::JsonValue JsonParser::parse_value(const std::string& json, size_t& pos) {
    skip_whitespace(json, pos);
    
    if (pos >= json.length()) return JsonValue();
    
    char c = json[pos];
    if (c == '"') {
        return JsonValue(parse_string(json, pos));
    } else if (c == '-' || std::isdigit(c)) {
        return JsonValue(parse_number(json, pos));
    } else if (c == 't' || c == 'f') {
        return JsonValue(parse_bool(json, pos));
    } else if (c == 'n') {
        // Handle null
        if (json.substr(pos, 4) == "null") {
            pos += 4;
            return JsonValue();
        }
    } else if (c == '{') {
        // Parse object
        JsonValue obj;
        pos++; // skip '{'
        skip_whitespace(json, pos);
        
        while (pos < json.length() && json[pos] != '}') {
            skip_whitespace(json, pos);
            if (json[pos] != '"') break;
            
            std::string key = parse_string(json, pos);
            skip_whitespace(json, pos);
            
            if (pos < json.length() && json[pos] == ':') {
                pos++; // skip ':'
                JsonValue value = parse_value(json, pos);
                obj.object[key] = value;
            }
            
            skip_whitespace(json, pos);
            if (pos < json.length() && json[pos] == ',') {
                pos++; // skip ','
            }
        }
        
        if (pos < json.length() && json[pos] == '}') {
            pos++; // skip '}'
        }
        return obj;
    } else if (c == '[') {
        // Parse array
        JsonValue arr;
        pos++; // skip '['
        skip_whitespace(json, pos);
        
        while (pos < json.length() && json[pos] != ']') {
            JsonValue value = parse_value(json, pos);
            arr.array.push_back(value);
            
            skip_whitespace(json, pos);
            if (pos < json.length() && json[pos] == ',') {
                pos++; // skip ','
            }
        }
        
        if (pos < json.length() && json[pos] == ']') {
            pos++; // skip ']'
        }
        return arr;
    }
    
    return JsonValue();
}

std::string JsonParser::parse_string(const std::string& json, size_t& pos) {
    if (pos >= json.length() || json[pos] != '"') return "";
    
    pos++; // skip opening quote
    std::string result;
    
    while (pos < json.length() && json[pos] != '"') {
        if (json[pos] == '\\') {
            pos++; // skip backslash
            if (pos < json.length()) {
                char c = json[pos];
                switch (c) {
                    case '"': result += '"'; break;
                    case '\\': result += '\\'; break;
                    case '/': result += '/'; break;
                    case 'b': result += '\b'; break;
                    case 'f': result += '\f'; break;
                    case 'n': result += '\n'; break;
                    case 'r': result += '\r'; break;
                    case 't': result += '\t'; break;
                    default: result += c; break;
                }
                pos++;
            }
        } else {
            result += json[pos++];
        }
    }
    
    if (pos < json.length() && json[pos] == '"') {
        pos++; // skip closing quote
    }
    
    return result;
}

double JsonParser::parse_number(const std::string& json, size_t& pos) {
    size_t start = pos;
    
    // Handle negative numbers
    if (pos < json.length() && json[pos] == '-') {
        pos++;
    }
    
    // Parse integer part
    while (pos < json.length() && std::isdigit(json[pos])) {
        pos++;
    }
    
    // Parse fractional part
    if (pos < json.length() && json[pos] == '.') {
        pos++;
        while (pos < json.length() && std::isdigit(json[pos])) {
            pos++;
        }
    }
    
    // Parse exponent
    if (pos < json.length() && (json[pos] == 'e' || json[pos] == 'E')) {
        pos++;
        if (pos < json.length() && (json[pos] == '+' || json[pos] == '-')) {
            pos++;
        }
        while (pos < json.length() && std::isdigit(json[pos])) {
            pos++;
        }
    }
    
    std::string num_str = json.substr(start, pos - start);
    try {
        return std::stod(num_str);
    } catch (const std::exception&) {
        return 0.0;
    }
}

bool JsonParser::parse_bool(const std::string& json, size_t& pos) {
    if (pos + 3 < json.length() && json.substr(pos, 4) == "true") {
        pos += 4;
        return true;
    } else if (pos + 4 < json.length() && json.substr(pos, 5) == "false") {
        pos += 5;
        return false;
    }
    return false;
}

void JsonParser::skip_whitespace(const std::string& json, size_t& pos) {
    while (pos < json.length() && std::isspace(json[pos])) {
        pos++;
    }
}

// Utility functions implementation
namespace utils {

std::string to_lower(std::string str) {
    std::transform(str.begin(), str.end(), str.begin(), ::tolower);
    return str;
}

std::string to_upper(std::string str) {
    std::transform(str.begin(), str.end(), str.begin(), ::toupper);
    return str;
}

std::vector<std::string> split(const std::string& str, char delimiter) {
    std::vector<std::string> result;
    std::stringstream ss(str);
    std::string item;
    
    while (std::getline(ss, item, delimiter)) {
        result.push_back(item);
    }
    
    return result;
}

std::string trim(const std::string& str) {
    size_t start = str.find_first_not_of(" \t\n\r");
    if (start == std::string::npos) return "";
    
    size_t end = str.find_last_not_of(" \t\n\r");
    return str.substr(start, end - start + 1);
}

bool starts_with(const std::string& str, const std::string& prefix) {
    if (prefix.length() > str.length()) return false;
    return str.compare(0, prefix.length(), prefix) == 0;
}

bool ends_with(const std::string& str, const std::string& suffix) {
    if (suffix.length() > str.length()) return false;
    return str.compare(str.length() - suffix.length(), suffix.length(), suffix) == 0;
}

} // namespace utils

} // namespace modern
