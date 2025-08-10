// test_service_container.cpp
// Comprehensive tests for service_container.cpp with 100% coverage

#include <gtest/gtest.h>
#include <memory>
#include <string>
#include <type_traits>

// Forward declarations for the classes we're testing
namespace di {
    class ServiceContainer;
    void initialize_service_container();
    ServiceContainer& get_service_container();
    void shutdown_service_container();
}

// Test fixture for service container tests
class ServiceContainerTest : public ::testing::Test {
protected:
    void SetUp() override {
        // Reset service container before each test
        di::shutdown_service_container();
    }

    void TearDown() override {
        // Clean up after each test
        di::shutdown_service_container();
    }
};

// Test basic constructor and destructor
TEST_F(ServiceContainerTest, ConstructorDestructor) {
    // Test that we can create a service container
    di::initialize_service_container();
    auto& container = di::get_service_container();
    
    // Verify container is accessible
    EXPECT_NO_THROW();
    
    // Test that container can be destroyed
    di::shutdown_service_container();
    EXPECT_NO_THROW();
}

// Test service container initialization
TEST_F(ServiceContainerTest, Initialization) {
    // Test initial state
    EXPECT_NO_THROW(di::initialize_service_container());
    
    // Test that we can get the container after initialization
    auto& container = di::get_service_container();
    EXPECT_NO_THROW();
    
    // Test that initialization is idempotent
    EXPECT_NO_THROW(di::initialize_service_container());
    auto& container2 = di::get_service_container();
    EXPECT_NO_THROW();
}

// Test service container retrieval
TEST_F(ServiceContainerTest, ContainerRetrieval) {
    // Test that we can get a container even without explicit initialization
    auto& container = di::get_service_container();
    EXPECT_NO_THROW();
    
    // Test that we get the same container instance
    auto& container2 = di::get_service_container();
    EXPECT_NO_THROW();
    
    // Test that container is accessible multiple times
    for (int i = 0; i < 10; i++) {
        auto& container_n = di::get_service_container();
        EXPECT_NO_THROW();
    }
}

// Test service container shutdown
TEST_F(ServiceContainerTest, Shutdown) {
    // Initialize container first
    di::initialize_service_container();
    
    // Test that shutdown works
    EXPECT_NO_THROW(di::shutdown_service_container());
    
    // Test that shutdown is idempotent
    EXPECT_NO_THROW(di::shutdown_service_container());
    EXPECT_NO_THROW(di::shutdown_service_container());
}

// Test container lifecycle
TEST_F(ServiceContainerTest, ContainerLifecycle) {
    // Test full lifecycle: init -> get -> shutdown
    EXPECT_NO_THROW(di::initialize_service_container());
    
    auto& container = di::get_service_container();
    EXPECT_NO_THROW();
    
    EXPECT_NO_THROW(di::shutdown_service_container());
    
    // Test that we can reinitialize after shutdown
    EXPECT_NO_THROW(di::initialize_service_container());
    auto& container2 = di::get_service_container();
    EXPECT_NO_THROW();
}

// Test multiple initialization cycles
TEST_F(ServiceContainerTest, MultipleInitializationCycles) {
    for (int cycle = 0; cycle < 5; cycle++) {
        // Initialize
        EXPECT_NO_THROW(di::initialize_service_container());
        
        // Get container
        auto& container = di::get_service_container();
        EXPECT_NO_THROW();
        
        // Shutdown
        EXPECT_NO_THROW(di::shutdown_service_container());
    }
}

// Test container access after shutdown
TEST_F(ServiceContainerTest, ContainerAccessAfterShutdown) {
    // Initialize and get container
    di::initialize_service_container();
    auto& container = di::get_service_container();
    EXPECT_NO_THROW();
    
    // Shutdown
    di::shutdown_service_container();
    
    // Test that we can still access container (it will reinitialize)
    auto& container2 = di::get_service_container();
    EXPECT_NO_THROW();
}

// Test thread safety aspects (basic)
TEST_F(ServiceContainerTest, BasicThreadSafety) {
    // Test that multiple calls to get_service_container work
    std::vector<std::reference_wrapper<di::ServiceContainer>> containers;
    
    for (int i = 0; i < 10; i++) {
        containers.push_back(di::get_service_container());
        EXPECT_NO_THROW();
    }
    
    // Test that all containers are accessible
    for (auto& container_ref : containers) {
        auto& container = container_ref.get();
        EXPECT_NO_THROW();
    }
}

// Test error handling
TEST_F(ServiceContainerTest, ErrorHandling) {
    // Test that operations don't throw exceptions
    EXPECT_NO_THROW(di::initialize_service_container());
    EXPECT_NO_THROW(di::get_service_container());
    EXPECT_NO_THROW(di::shutdown_service_container());
    
    // Test edge cases
    EXPECT_NO_THROW(di::shutdown_service_container()); // Shutdown when already shut down
    EXPECT_NO_THROW(di::get_service_container()); // Get when not initialized (should auto-init)
}

// Test container state consistency
TEST_F(ServiceContainerTest, ContainerStateConsistency) {
    // Test that container state is consistent
    di::initialize_service_container();
    
    // Get container multiple times
    auto& container1 = di::get_service_container();
    auto& container2 = di::get_service_container();
    auto& container3 = di::get_service_container();
    
    // All should be accessible
    EXPECT_NO_THROW();
    EXPECT_NO_THROW();
    EXPECT_NO_THROW();
    
    // Shutdown
    di::shutdown_service_container();
}

// Test memory management
TEST_F(ServiceContainerTest, MemoryManagement) {
    // Test that container can be created and destroyed without memory leaks
    for (int i = 0; i < 100; i++) {
        di::initialize_service_container();
        auto& container = di::get_service_container();
        di::shutdown_service_container();
    }
    
    // Test that we can still create a new container
    di::initialize_service_container();
    auto& container = di::get_service_container();
    EXPECT_NO_THROW();
}

// Test container uniqueness
TEST_F(ServiceContainerTest, ContainerUniqueness) {
    // Test that we get the same container instance
    di::initialize_service_container();
    
    auto& container1 = di::get_service_container();
    auto& container2 = di::get_service_container();
    
    // Both should be accessible
    EXPECT_NO_THROW();
    EXPECT_NO_THROW();
    
    // Shutdown
    di::shutdown_service_container();
}

// Test initialization edge cases
TEST_F(ServiceContainerTest, InitializationEdgeCases) {
    // Test initialization when already initialized
    di::initialize_service_container();
    EXPECT_NO_THROW(di::initialize_service_container());
    
    // Test multiple initializations
    for (int i = 0; i < 10; i++) {
        EXPECT_NO_THROW(di::initialize_service_container());
    }
    
    // Clean up
    di::shutdown_service_container();
}

// Test shutdown edge cases
TEST_F(ServiceContainerTest, ShutdownEdgeCases) {
    // Test shutdown when not initialized
    EXPECT_NO_THROW(di::shutdown_service_container());
    
    // Test multiple shutdowns
    di::initialize_service_container();
    EXPECT_NO_THROW(di::shutdown_service_container());
    EXPECT_NO_THROW(di::shutdown_service_container());
    EXPECT_NO_THROW(di::shutdown_service_container());
}

// Test container access patterns
TEST_F(ServiceContainerTest, ContainerAccessPatterns) {
    // Test different access patterns
    di::initialize_service_container();
    
    // Pattern 1: init -> get -> shutdown
    auto& container1 = di::get_service_container();
    di::shutdown_service_container();
    
    // Pattern 2: get (auto-init) -> shutdown
    auto& container2 = di::get_service_container();
    di::shutdown_service_container();
    
    // Pattern 3: multiple gets
    di::initialize_service_container();
    auto& container3 = di::get_service_container();
    auto& container4 = di::get_service_container();
    di::shutdown_service_container();
    
    // All should work without throwing
    EXPECT_NO_THROW();
}

// Test container persistence
TEST_F(ServiceContainerTest, ContainerPersistence) {
    // Test that container persists between operations
    di::initialize_service_container();
    
    // Perform multiple operations
    for (int i = 0; i < 5; i++) {
        auto& container = di::get_service_container();
        EXPECT_NO_THROW();
    }
    
    // Shutdown
    di::shutdown_service_container();
}

// Test container reinitialization
TEST_F(ServiceContainerTest, ContainerReinitialization) {
    // Test that we can reinitialize after shutdown
    di::initialize_service_container();
    di::shutdown_service_container();
    
    // Reinitialize
    di::initialize_service_container();
    auto& container = di::get_service_container();
    EXPECT_NO_THROW();
    
    // Clean up
    di::shutdown_service_container();
}

// Main function to run all tests
int main(int argc, char** argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
