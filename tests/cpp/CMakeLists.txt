# CMakeLists.txt for C++ tests
cmake_minimum_required(VERSION 3.16)
project(NeoZorKDEXArbTests)

# Set C++ standard
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Build optimizations for faster compilation
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O1")
set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -O0")
set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -O2")

# Enable parallel compilation
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -pipe")

# Find required packages
find_package(Threads REQUIRED)
find_package(PkgConfig REQUIRED)
find_package(CURL REQUIRED)

# Find GoogleTest
find_package(GTest REQUIRED)

# Include directories
include_directories(${CMAKE_SOURCE_DIR}/include)
include_directories(${CMAKE_SOURCE_DIR}/src)

# Source files for linking (excluding main.cpp)
file(GLOB_RECURSE SOURCES "${CMAKE_SOURCE_DIR}/src/*.cpp")
list(REMOVE_ITEM SOURCES "${CMAKE_SOURCE_DIR}/src/main.cpp")

# Test executables with proper linking - only basic tests for now
add_executable(test_modern_result test_modern_result.cpp ${SOURCES})
add_executable(test_modern_result_extended test_modern_result_extended.cpp ${SOURCES})
add_executable(test_modern_format test_modern_format.cpp ${SOURCES})
add_executable(test_help_display test_help_display.cpp ${SOURCES})
add_executable(test_cli_commands test_cli_commands.cpp ${SOURCES})
add_executable(test_command_line_flags test_command_line_flags.cpp ${SOURCES})
add_executable(test_all_flags_and_results test_all_flags_and_results.cpp ${SOURCES})

# Link libraries for basic test executables
foreach(target test_modern_result test_modern_result_extended test_modern_format
         test_help_display test_cli_commands test_command_line_flags test_all_flags_and_results)
    target_link_libraries(${target} 
        Threads::Threads 
        GTest::gtest 
        GTest::gtest_main 
        CURL::libcurl
    )
endforeach()

# Enable testing
enable_testing()

# Add basic tests with proper names
add_test(NAME ModernResult COMMAND test_modern_result)
add_test(NAME ModernResultExtended COMMAND test_modern_result_extended)
add_test(NAME ModernFormat COMMAND test_modern_format)
add_test(NAME HelpDisplay COMMAND test_help_display)
add_test(NAME CliCommands COMMAND test_cli_commands)
add_test(NAME CommandLineFlags COMMAND test_command_line_flags)
add_test(NAME AllFlagsAndResults COMMAND test_all_flags_and_results)

# Set test properties with reduced timeouts for faster execution
set_tests_properties(ModernResult PROPERTIES TIMEOUT 60)
set_tests_properties(ModernResultExtended PROPERTIES TIMEOUT 60)
set_tests_properties(ModernFormat PROPERTIES TIMEOUT 60)
set_tests_properties(HelpDisplay PROPERTIES TIMEOUT 60)
set_tests_properties(CliCommands PROPERTIES TIMEOUT 60)
set_tests_properties(CommandLineFlags PROPERTIES TIMEOUT 60)
set_tests_properties(AllFlagsAndResults PROPERTIES TIMEOUT 60)

# Custom target to run basic tests with parallel execution
add_custom_target(run_basic_tests
    COMMAND ${CMAKE_CTEST_COMMAND} --output-on-failure --parallel $(nproc)
    DEPENDS test_modern_result test_modern_result_extended test_modern_format
            test_help_display test_cli_commands test_command_line_flags test_all_flags_and_results
    COMMENT "Running basic tests in parallel"
)
