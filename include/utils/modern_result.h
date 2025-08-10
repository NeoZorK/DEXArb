//
//  modern_result.h
//  NeoZorKDEXArb
//
//  Created by Rostyslav S. on 10.08.2025.
//
#ifndef MODERN_RESULT_H
#define MODERN_RESULT_H

#include <string>
#include <string_view>
#include <stdexcept>
#include <memory>

namespace modern {

// Simple Result type for error handling
template<typename T, typename E = std::string>
class Result {
private:
    union {
        T value_;
        E error_;
    };
    bool is_ok_;

public:
    // Constructors
    Result() : error_(), is_ok_(false) {}
    
    Result(const T& value) : value_(value), is_ok_(true) {}
    Result(T&& value) : value_(std::move(value)), is_ok_(true) {}
    
    Result(const E& error) : error_(error), is_ok_(false) {}
    Result(E&& error) : error_(std::move(error)), is_ok_(false) {}
    
    // Copy constructor
    Result(const Result& other) : is_ok_(other.is_ok_) {
        if (is_ok_) {
            new (&value_) T(other.value_);
        } else {
            new (&error_) E(other.error_);
        }
    }
    
    // Move constructor
    Result(Result&& other) : is_ok_(other.is_ok_) {
        if (is_ok_) {
            new (&value_) T(std::move(other.value_));
        } else {
            new (&error_) E(std::move(other.error_));
        }
    }
    
    // Destructor
    ~Result() {
        if (is_ok_) {
            value_.~T();
        } else {
            error_.~E();
        }
    }
    
    // Copy assignment
    Result& operator=(const Result& other) {
        if (this != &other) {
            this->~Result();
            is_ok_ = other.is_ok_;
            if (is_ok_) {
                new (&value_) T(other.value_);
            } else {
                new (&error_) E(other.error_);
            }
        }
        return *this;
    }
    
    // Move assignment
    Result& operator=(Result&& other) {
        if (this != &other) {
            this->~Result();
            is_ok_ = other.is_ok_;
            if (is_ok_) {
                new (&value_) T(std::move(other.value_));
            } else {
                new (&error_) E(std::move(other.error_));
            }
        }
        return *this;
    }
    
    // Static factory methods
    static Result<T, E> ok(const T& value) {
        return Result<T, E>(value);
    }
    
    static Result<T, E> ok(T&& value) {
        return Result<T, E>(std::move(value));
    }
    
    static Result<T, E> err(const E& error) {
        return Result<T, E>(error);
    }
    
    static Result<T, E> err(E&& error) {
        return Result<T, E>(std::move(error));
    }
    
    // Status checking
    bool is_ok() const noexcept { return is_ok_; }
    bool is_err() const noexcept { return !is_ok_; }
    explicit operator bool() const noexcept { return is_ok(); }
    
    // Value access
    T& unwrap() & {
        if (!is_ok()) {
            throw std::runtime_error("Attempted to unwrap error result");
        }
        return value_;
    }
    
    const T& unwrap() const& {
        if (!is_ok()) {
            throw std::runtime_error("Attempted to unwrap error result");
        }
        return value_;
    }
    
    T&& unwrap() && {
        if (!is_ok()) {
            throw std::runtime_error("Attempted to unwrap error result");
        }
        return std::move(value_);
    }
    
    // Error access
    E& unwrap_err() & {
        if (is_ok()) {
            throw std::runtime_error("Attempted to unwrap error from ok result");
        }
        return error_;
    }
    
    const E& unwrap_err() const& {
        if (is_ok()) {
            throw std::runtime_error("Attempted to unwrap error from ok result");
        }
        return error_;
    }
    
    E&& unwrap_err() && {
        if (is_ok()) {
            throw std::runtime_error("Attempted to unwrap error from ok result");
        }
        return std::move(error_);
    }
    
    // Safe value access with default
    T unwrap_or(const T& default_value) const& {
        return is_ok() ? value_ : default_value;
    }
    
    T unwrap_or(T&& default_value) && {
        return is_ok() ? std::move(value_) : std::move(default_value);
    }
    
    // Safe value access with function
    template<typename F>
    T unwrap_or_else(F&& f) const& {
        return is_ok() ? value_ : std::forward<F>(f)();
    }
    
    template<typename F>
    T unwrap_or_else(F&& f) && {
        return is_ok() ? std::move(value_) : std::forward<F>(f)();
    }
    
    // Value and error getters
    const T* value_ptr() const noexcept { 
        return is_ok() ? &value_ : nullptr; 
    }
    
    T* value_ptr() noexcept { 
        return is_ok() ? &value_ : nullptr; 
    }
    
    const E* error_ptr() const noexcept { 
        return is_err() ? &error_ : nullptr; 
    }
    
    E* error_ptr() noexcept { 
        return is_err() ? &error_ : nullptr; 
    }
};

// Type aliases for common use cases
template<typename T>
using ResultT = Result<T, std::string>;

template<typename T>
using ResultStr = Result<T, std::string>;

// Utility functions
template<typename T, typename E>
Result<T, E> ok(const T& value) {
    return Result<T, E>::ok(value);
}

template<typename T, typename E>
Result<T, E> ok(T&& value) {
    return Result<T, E>::ok(std::move(value));
}

template<typename T, typename E>
Result<T, E> err(const E& error) {
    return Result<T, E>::err(error);
}

template<typename T, typename E>
Result<T, E> err(E&& error) {
    return Result<T, E>::err(std::move(error));
}

} // namespace modern

#endif // MODERN_RESULT_H
