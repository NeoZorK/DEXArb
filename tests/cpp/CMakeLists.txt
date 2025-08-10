# CMakeLists.txt for C++ tests
cmake_minimum_required(VERSION 3.16)
project(NeoZorKDEXArbTests)

# Set C++ standard
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Find required packages
find_package(Threads REQUIRED)

# Include directories
include_directories(${CMAKE_SOURCE_DIR}/include)
include_directories(${CMAKE_SOURCE_DIR}/src)

# Test executables
add_executable(test_all_flags_and_results test_all_flags_and_results.cpp)
add_executable(test_command_line_flags test_command_line_flags.cpp)
add_executable(test_cli_commands test_cli_commands.cpp)
add_executable(test_help_display test_help_display.cpp)
add_executable(test_modern_result test_modern_result.cpp)
add_executable(test_modern_result_extended test_modern_result_extended.cpp)
add_executable(test_modern_format test_modern_format.cpp)

# Link libraries
target_link_libraries(test_all_flags_and_results Threads::Threads)
target_link_libraries(test_command_line_flags Threads::Threads)
target_link_libraries(test_cli_commands Threads::Threads)
target_link_libraries(test_help_display Threads::Threads)
target_link_libraries(test_modern_result Threads::Threads)
target_link_libraries(test_modern_result_extended Threads::Threads)
target_link_libraries(test_modern_format Threads::Threads)

# Enable testing
enable_testing()

# Add tests
add_test(NAME AllFlagsAndResults COMMAND test_all_flags_and_results)
add_test(NAME CommandLineFlags COMMAND test_command_line_flags)
add_test(NAME CliCommands COMMAND test_cli_commands)
add_test(NAME HelpDisplay COMMAND test_help_display)
add_test(NAME ModernResult COMMAND test_modern_result)
add_test(NAME ModernResultExtended COMMAND test_modern_result_extended)
add_test(NAME ModernFormat COMMAND test_modern_format)

# Set test properties
set_tests_properties(AllFlagsAndResults PROPERTIES TIMEOUT 300)
set_tests_properties(CommandLineFlags PROPERTIES TIMEOUT 300)
set_tests_properties(CliCommands PROPERTIES TIMEOUT 300)
set_tests_properties(HelpDisplay PROPERTIES TIMEOUT 300)
set_tests_properties(ModernResult PROPERTIES TIMEOUT 300)
set_tests_properties(ModernResultExtended PROPERTIES TIMEOUT 300)
set_tests_properties(ModernFormat PROPERTIES TIMEOUT 300)

# Custom target to run all tests
add_custom_target(run_tests
    COMMAND ${CMAKE_CTEST_COMMAND} --output-on-failure
    DEPENDS test_all_flags_and_results test_command_line_flags test_cli_commands 
            test_help_display test_modern_result test_modern_result_extended test_modern_format
    COMMENT "Running all tests"
)
