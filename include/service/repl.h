#pragma once

#include "ansi.h"
#include <span>
#include <string_view>

/// @brief A Read-Eval-Print Loop (REPL) implementation for command-line
/// interfaces.
namespace repl {
/// @brief A command that can be executed in the REPL.
struct command {
    /// @brief The command string that the user can type to execute the command.
    std::string_view cmd;
    /// @brief A description of the command.
    std::string_view desc;
    /// @brief A pointer to the function that will be executed when the
    /// command is called.
    void (*const action)();
};

/// @brief Print the help message for the given commands.
/// @param commands A span of Command objects representing the available
/// commands.
constexpr void help(std::span<const command> commands);

/// @brief Check if the standard input stream has failed.
/// @return true if the standard input stream has failed, false otherwise.
[[nodiscard]] bool cin_fail();

/// @brief Run the REPL with the given commands and status message.
/// @param commands A span of Command objects representing the available
/// commands.
/// @param status A string_view representing the status message to display in
/// the REPL prompt.
/// @param style An optional ANSI escape sequence to style the REPL prompt.
void run(std::span<const command> commands,
         std::string_view status, ansi::esc_sequence style = "");

/// @brief Prompt the user for confirmation with a yes/no question.
/// @param text A string_view representing the confirmation question to
/// display.
/// @return true if the user answers "yes", false if the answer is "no". An
/// input error will return false.
[[nodiscard]] bool confirm(std::string_view text);
}; // namespace repl
