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

# All test executables for 100% coverage
add_executable(test_modern_result test_modern_result.cpp ${SOURCES})
add_executable(test_modern_result_extended test_modern_result_extended.cpp ${SOURCES})
add_executable(test_modern_format test_modern_format.cpp ${SOURCES})
add_executable(test_help_display test_help_display.cpp ${SOURCES})
add_executable(test_cli_commands test_cli_commands.cpp ${SOURCES})
add_executable(test_command_line_flags test_command_line_flags.cpp ${SOURCES})
add_executable(test_all_flags_and_results test_all_flags_and_results.cpp ${SOURCES})
add_executable(test_universal_build_script test_universal_build_script.cpp ${SOURCES})

# Additional test executables for complete coverage
add_executable(test_application test_application.cpp ${SOURCES})
add_executable(test_arbitrage test_arbitrage.cpp ${SOURCES})
add_executable(test_blockchain test_blockchain.cpp ${SOURCES})
add_executable(test_command_parser test_command_parser.cpp ${SOURCES})
add_executable(test_config_manager test_config_manager.cpp ${SOURCES})
add_executable(test_dex_pools test_dex_pools.cpp ${SOURCES})
add_executable(test_dex_scanner test_dex_scanner.cpp ${SOURCES})
add_executable(test_dex_stats test_dex_stats.cpp ${SOURCES})
add_executable(test_dex_tokens test_dex_tokens.cpp ${SOURCES})
add_executable(test_input test_input.cpp ${SOURCES})
add_executable(test_interfaces test_interfaces.cpp ${SOURCES})
add_executable(test_main_structures test_main_structures.cpp ${SOURCES})
add_executable(test_measure test_measure.cpp ${SOURCES})
add_executable(test_platform test_platform.cpp ${SOURCES})
add_executable(test_platform_compatibility test_platform_compatibility.cpp ${SOURCES})
add_executable(test_profit_analyzer test_profit_analyzer.cpp ${SOURCES})
add_executable(test_queries test_queries.cpp ${SOURCES})
add_executable(test_rpc_core test_rpc_core.cpp ${SOURCES})
add_executable(test_service_container test_service_container.cpp ${SOURCES})
add_executable(test_wallet test_wallet.cpp ${SOURCES})

# Link libraries for all test executables
set(ALL_TEST_TARGETS
    test_modern_result test_modern_result_extended test_modern_format
    test_help_display test_cli_commands test_command_line_flags test_all_flags_and_results
    test_universal_build_script test_application test_arbitrage test_blockchain test_command_parser test_config_manager
    test_dex_pools test_dex_scanner test_dex_stats test_dex_tokens test_input
    test_interfaces test_main_structures test_measure test_platform test_platform_compatibility
    test_profit_analyzer test_queries test_rpc_core test_service_container test_wallet
)

foreach(target ${ALL_TEST_TARGETS})
    target_link_libraries(${target} 
        Threads::Threads 
        GTest::gtest 
        GTest::gtest_main 
        CURL::libcurl
    )
endforeach()

# Enable testing
enable_testing()

# Add all tests with proper names
add_test(NAME ModernResult COMMAND test_modern_result)
add_test(NAME ModernResultExtended COMMAND test_modern_result_extended)
add_test(NAME ModernFormat COMMAND test_modern_format)
add_test(NAME HelpDisplay COMMAND test_help_display)
add_test(NAME CliCommands COMMAND test_cli_commands)
add_test(NAME CommandLineFlags COMMAND test_command_line_flags)
add_test(NAME AllFlagsAndResults COMMAND test_all_flags_and_results)
add_test(NAME UniversalBuildScript COMMAND test_universal_build_script)

# Additional tests for complete coverage
add_test(NAME Application COMMAND test_application)
add_test(NAME Arbitrage COMMAND test_arbitrage)
add_test(NAME Blockchain COMMAND test_blockchain)
add_test(NAME CommandParser COMMAND test_command_parser)
add_test(NAME ConfigManager COMMAND test_config_manager)
add_test(NAME DexPools COMMAND test_dex_pools)
add_test(NAME DexScanner COMMAND test_dex_scanner)
add_test(NAME DexStats COMMAND test_dex_stats)
add_test(NAME DexTokens COMMAND test_dex_tokens)
add_test(NAME Input COMMAND test_input)
add_test(NAME Interfaces COMMAND test_interfaces)
add_test(NAME MainStructures COMMAND test_main_structures)
add_test(NAME Measure COMMAND test_measure)
add_test(NAME Platform COMMAND test_platform)
add_test(NAME PlatformCompatibility COMMAND test_platform_compatibility)
add_test(NAME ProfitAnalyzer COMMAND test_profit_analyzer)
add_test(NAME Queries COMMAND test_queries)
add_test(NAME RpcCore COMMAND test_rpc_core)
add_test(NAME ServiceContainer COMMAND test_service_container)
add_test(NAME Wallet COMMAND test_wallet)

# Set test properties with reduced timeouts for faster execution
foreach(test_name ModernResult ModernResultExtended ModernFormat HelpDisplay CliCommands 
                    CommandLineFlags AllFlagsAndResults UniversalBuildScript Application Arbitrage Blockchain 
                    CommandParser ConfigManager DexPools DexScanner DexStats DexTokens 
                    Input Interfaces MainStructures Measure Platform PlatformCompatibility 
                    ProfitAnalyzer Queries RpcCore ServiceContainer Wallet)
    set_tests_properties(${test_name} PROPERTIES TIMEOUT 60)
endforeach()

# Custom target to run all tests with parallel execution
add_custom_target(run_all_tests
    COMMAND ${CMAKE_CTEST_COMMAND} --output-on-failure --parallel $(nproc)
    DEPENDS ${ALL_TEST_TARGETS}
    COMMENT "Running all tests in parallel for 100% coverage"
)

# Custom target to run basic tests with parallel execution
add_custom_target(run_basic_tests
    COMMAND ${CMAKE_CTEST_COMMAND} --output-on-failure --parallel $(nproc)
    DEPENDS test_modern_result test_modern_result_extended test_modern_format
            test_help_display test_cli_commands test_command_line_flags test_all_flags_and_results
    COMMENT "Running basic tests in parallel"
)
