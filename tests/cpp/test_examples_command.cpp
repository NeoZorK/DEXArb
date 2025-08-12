#include <gtest/gtest.h>
#include <iostream>
#include <sstream>
#include <string>

// Include the application headers
#include "../../include/core/application.h"
#include "../../include/cli/command_parser.h"
#include "../../include/cli/help_display.h"

using namespace core;
using namespace cli;

class ExamplesCommandTest : public ::testing::Test {
protected:
    void SetUp() override {
        // Setup any common test data
    }
    
    void TearDown() override {
        // Cleanup any test data
    }
};

// Test that EXAMPLES command type is properly defined
TEST_F(ExamplesCommandTest, ExamplesCommandType) {
    EXPECT_EQ(static_cast<int>(CommandType::EXAMPLES), 2); // Should be after HELP(0) and VERSION_CMD(1)
}

// Test that EXAMPLES flag is properly parsed
TEST_F(ExamplesCommandTest, ExamplesFlagParsing) {
    const char* argv[] = {"dexarb", "-examples"};
    ParsedCommand cmd = CommandParser::parse(2, argv);
    
    EXPECT_EQ(cmd.type, CommandType::EXAMPLES);
    EXPECT_EQ(cmd.flag, "-examples");
    EXPECT_TRUE(cmd.is_valid);
    EXPECT_TRUE(cmd.blockchain.empty());
    EXPECT_TRUE(cmd.value.empty());
}

// Test that --examples flag is properly parsed
TEST_F(ExamplesCommandTest, ExamplesLongFlagParsing) {
    const char* argv[] = {"dexarb", "--examples"};
    ParsedCommand cmd = CommandParser::parse(2, argv);
    
    EXPECT_EQ(cmd.type, CommandType::EXAMPLES);
    EXPECT_EQ(cmd.flag, "--examples");
    EXPECT_TRUE(cmd.is_valid);
    EXPECT_TRUE(cmd.blockchain.empty());
    EXPECT_TRUE(cmd.value.empty());
}

// Test that EXAMPLES command doesn't require blockchain
TEST_F(ExamplesCommandTest, ExamplesNoBlockchainRequired) {
    EXPECT_FALSE(CommandParser::requires_blockchain(CommandType::EXAMPLES));
}

// Test that EXAMPLES command doesn't require value
TEST_F(ExamplesCommandTest, ExamplesNoValueRequired) {
    EXPECT_FALSE(CommandParser::requires_value(CommandType::EXAMPLES));
}

// Test that EXAMPLES command doesn't require DEX
TEST_F(ExamplesCommandTest, ExamplesNoDexRequired) {
    EXPECT_FALSE(CommandParser::requires_dex(CommandType::EXAMPLES));
}

// Test that EXAMPLES command doesn't require token
TEST_F(ExamplesCommandTest, ExamplesNoTokenRequired) {
    EXPECT_FALSE(CommandParser::requires_token(CommandType::EXAMPLES));
}

// Test that EXAMPLES command has proper description
TEST_F(ExamplesCommandTest, ExamplesDescription) {
    std::string description = CommandParser::get_command_description(CommandType::EXAMPLES);
    EXPECT_EQ(description, "Display detailed examples");
}

// Test that EXAMPLES command is properly handled by Application
TEST_F(ExamplesCommandTest, ExamplesCommandHandling) {
    Application app;
    
    // Test that the command is handled without throwing
    const char* argv[] = {"dexarb", "-examples"};
    ParsedCommand cmd = CommandParser::parse(2, argv);
    
    EXPECT_NO_THROW({
        int result = app.handle_examples_command();
        EXPECT_EQ(result, 0); // Should return success
    });
}

// Test that EXAMPLES command works in full application flow
TEST_F(ExamplesCommandTest, ExamplesFullApplicationFlow) {
    Application app;
    
    const char* argv[] = {"dexarb", "-examples"};
    int result = app.run(2, argv);
    
    EXPECT_EQ(result, 0); // Should return success
}

// Test that EXAMPLES command is different from HELP command
TEST_F(ExamplesCommandTest, ExamplesDifferentFromHelp) {
    EXPECT_NE(CommandType::EXAMPLES, CommandType::HELP);
    EXPECT_NE(CommandType::EXAMPLES, CommandType::VERSION_CMD);
}

// Test that EXAMPLES command is properly validated
TEST_F(ExamplesCommandTest, ExamplesCommandValidation) {
    const char* argv[] = {"dexarb", "-examples"};
    ParsedCommand cmd = CommandParser::parse(2, argv);
    
    EXPECT_TRUE(CommandParser::validate_command(cmd));
}

// Test that EXAMPLES command works with extra parameters (should ignore them)
TEST_F(ExamplesCommandTest, ExamplesWithExtraParameters) {
    const char* argv[] = {"dexarb", "-examples", "ethereum", "1000"};
    ParsedCommand cmd = CommandParser::parse(4, argv);
    
    EXPECT_EQ(cmd.type, CommandType::EXAMPLES);
    EXPECT_TRUE(cmd.is_valid);
    // Extra parameters should be ignored for EXAMPLES command
}

// Test that EXAMPLES command is properly converted from string
TEST_F(ExamplesCommandTest, ExamplesStringConversion) {
    EXPECT_EQ(CommandParser::string_to_command_type("-examples"), CommandType::EXAMPLES);
    EXPECT_EQ(CommandParser::string_to_command_type("--examples"), CommandType::EXAMPLES);
    EXPECT_NE(CommandParser::string_to_command_type("-h"), CommandType::EXAMPLES);
    EXPECT_NE(CommandParser::string_to_command_type("-v"), CommandType::EXAMPLES);
}

// Test that EXAMPLES command is properly handled in switch statement
TEST_F(ExamplesCommandTest, ExamplesSwitchHandling) {
    Application app;
    
    // Create a command with EXAMPLES type
    ParsedCommand cmd;
    cmd.type = CommandType::EXAMPLES;
    cmd.flag = "-examples";
    cmd.is_valid = true;
    
    // Test that it's handled properly in the application flow
    EXPECT_NO_THROW({
        // This should not throw and should handle EXAMPLES command
        // We can't directly test the switch statement, but we can test the handler
        int result = app.handle_examples_command();
        EXPECT_EQ(result, 0);
    });
}

// Test that EXAMPLES command is properly documented in help
TEST_F(ExamplesCommandTest, ExamplesHelpDocumentation) {
    // Capture output from help display
    std::ostringstream output;
    HelpDisplay::show_help(output);
    std::string help_text = output.str();
    
    // Check that EXAMPLES is mentioned in help
    EXPECT_NE(help_text.find("-examples"), std::string::npos);
    EXPECT_NE(help_text.find("Show detailed examples"), std::string::npos);
}

// Test that EXAMPLES command is properly documented in usage examples
TEST_F(ExamplesCommandTest, ExamplesUsageDocumentation) {
    // Capture output from help display (which includes usage examples)
    std::ostringstream output;
    HelpDisplay::show_help(output);
    std::string help_text = output.str();
    
    // Check that EXAMPLES is mentioned in help
    EXPECT_NE(help_text.find("-examples"), std::string::npos);
    EXPECT_NE(help_text.find("Show detailed examples"), std::string::npos);
}

// Test that EXAMPLES command is properly documented in detailed examples
TEST_F(ExamplesCommandTest, ExamplesDetailedDocumentation) {
    // Capture output from detailed examples
    std::ostringstream output;
    HelpDisplay::show_examples(output);
    std::string detailed_text = output.str();
    
    // Check that EXAMPLES is mentioned in detailed examples
    EXPECT_NE(detailed_text.find("-examples"), std::string::npos);
    EXPECT_NE(detailed_text.find("Show detailed examples"), std::string::npos);
}

// Test that EXAMPLES command is properly documented in help message
TEST_F(ExamplesCommandTest, ExamplesHelpMessageDocumentation) {
    // Capture output from help message
    std::ostringstream output;
    HelpDisplay::show_help(output);
    std::string help_text = output.str();
    
    // Check that EXAMPLES is mentioned in help message
    EXPECT_NE(help_text.find("-examples"), std::string::npos);
    EXPECT_NE(help_text.find("Show detailed examples"), std::string::npos);
    // Check that help mentions it includes examples
    EXPECT_NE(help_text.find("includes examples"), std::string::npos);
}

int main(int argc, char** argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
