//
//  service_container.cpp
//  NeoZorKDEXArb
//
//  Created by Rostyslav S. on 10.08.2025.
//
#include "di/service_container.h"

namespace di {

// Global service container instance
std::unique_ptr<ServiceContainer> g_service_container;

ServiceContainer::ServiceContainer() {
    // Initialize empty container
}

ServiceContainer::~ServiceContainer() {
    clear();
}

void ServiceContainer::clear() {
    services_.clear();
    singletons_.clear();
    created_singletons_.clear();
}

size_t ServiceContainer::get_service_count() const {
    return services_.size();
}

size_t ServiceContainer::get_singleton_count() const {
    return singletons_.size();
}

void initialize_service_container() {
    if (!g_service_container) {
        g_service_container = std::make_unique<ServiceContainer>();
    }
}

ServiceContainer& get_service_container() {
    if (!g_service_container) {
        initialize_service_container();
    }
    return *g_service_container;
}

void shutdown_service_container() {
    g_service_container.reset();
}

} // namespace di
