//
//  service_container.h
//  NeoZorKDEXArb
//
//  Created by Rostyslav S. on 10.08.2025.
//
#ifndef SERVICE_CONTAINER_H
#define SERVICE_CONTAINER_H

#include <memory>
#include <unordered_map>
#include <typeindex>
#include <functional>
#include <string>

namespace di {

// Service container for dependency injection
class ServiceContainer {
public:
    // Constructor
    ServiceContainer();
    
    // Destructor
    ~ServiceContainer();
    
    // Register a service with factory function
    template<typename T>
    void register_service(std::function<std::unique_ptr<T>()> factory) {
        services_[std::type_index(typeid(T))] = [factory]() -> std::unique_ptr<void> {
            auto service = factory();
            return std::unique_ptr<void>(service.release());
        };
    }
    
    // Register a singleton service
    template<typename T>
    void register_singleton(std::function<std::unique_ptr<T>()> factory) {
        singletons_[std::type_index(typeid(T))] = [factory]() -> std::unique_ptr<void> {
            auto service = factory();
            return std::unique_ptr<void>(service.release());
        };
    }
    
    // Resolve a service
    template<typename T>
    std::unique_ptr<T> resolve() {
        auto type_id = std::type_index(typeid(T));
        
        // Check singletons first
        auto singleton_it = singletons_.find(type_id);
        if (singleton_it != singletons_.end()) {
            auto service = singleton_it->second();
            return std::unique_ptr<T>(static_cast<T*>(service.release()));
        }
        
        // Check regular services
        auto service_it = services_.find(type_id);
        if (service_it != services_.end()) {
            auto service = service_it->second();
            return std::unique_ptr<T>(static_cast<T*>(service.release()));
        }
        
        return nullptr;
    }
    
    // Resolve a singleton (returns the same instance)
    template<typename T>
    T* resolve_singleton() {
        auto type_id = std::type_index(typeid(T));
        
        // Check if already created
        auto created_it = created_singletons_.find(type_id);
        if (created_it != created_singletons_.end()) {
            return static_cast<T*>(created_it->second.get());
        }
        
        // Create new singleton
        auto singleton_it = singletons_.find(type_id);
        if (singleton_it != singletons_.end()) {
            auto service = singleton_it->second();
            auto* service_ptr = static_cast<T*>(service.get());
            created_singletons_[type_id] = std::move(service);
            return service_ptr;
        }
        
        return nullptr;
    }
    
    // Check if service is registered
    template<typename T>
    bool is_registered() const {
        auto type_id = std::type_index(typeid(T));
        return services_.find(type_id) != services_.end() || 
               singletons_.find(type_id) != singletons_.end();
    }
    
    // Clear all services
    void clear();
    
    // Get registered service count
    size_t get_service_count() const;
    
    // Get registered singleton count
    size_t get_singleton_count() const;

private:
    // Service factories
    std::unordered_map<std::type_index, std::function<std::unique_ptr<void>()>> services_;
    
    // Singleton factories
    std::unordered_map<std::type_index, std::function<std::unique_ptr<void>()>> singletons_;
    
    // Created singleton instances
    std::unordered_map<std::type_index, std::unique_ptr<void>> created_singletons_;
};

// Global service container instance
extern std::unique_ptr<ServiceContainer> g_service_container;

// Initialize global service container
void initialize_service_container();

// Get global service container
ServiceContainer& get_service_container();

// Shutdown global service container
void shutdown_service_container();

} // namespace di

#endif // SERVICE_CONTAINER_H
