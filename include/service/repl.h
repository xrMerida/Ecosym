#pragma once

#include "ansi.h"
#include <span>
#include <string_view>

/// @brief A Read-Eval-Print Loop (REPL) implementation for command-line
/// interfaces.
namespace repl {

using cstring = const char *const;
using function_t = void (*)();

/// @brief A command that can be executed in the REPL.
struct command {
    /// @brief The command string that the user can type to execute the command.
    std::string_view cmd;
    /// @brief A description of the command.
    std::string_view desc;
    /// @brief A pointer to the function that will be executed when the
    /// command is called.
    function_t action;
};

/// @brief Function pointer to a no-operation non-null value
constexpr function_t no_op = [] {};

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
void run(std::span<const command> commands, std::string_view status,
         ansi::esc_sequence style = "");

/// @brief Parses a user input from cin to result
/// @param result A reference to the variable storing the parse
/// @param min The minimum value accepted
/// @param max The maximin value accepted
/// @returns true if the parse succeeds, false otherwise
/// Reads a line from cin and stores its integer value in result.
/// It fails if the input is out of range, if cin fails, or if it does not
/// contain a number.
[[nodiscard]] bool try_read_int(int &result, const int &min, const int &max);

/// @brief Display a menu to cout and return the answer from cin
/// @param options A span of constant null-terminated strings that represent the
/// options to display
/// @param status A string_view representing the status message to display in
/// the prompt.
/// @param style An optional ANSI escape sequence to style the prompt.
[[nodiscard]] int menu(std::span<cstring> options, std::string_view status,
                       ansi::esc_sequence style);
}; // namespace repl
