#pragma once

#include "ansi.h"
#include <span>
#include <string_view>
namespace repl {
struct Command {
    const char *str;
    const char *desc;
    void (*const action)();
};

/// @brief Print the help message for the given commands.
/// @param commands A span of Command objects representing the available
/// commands.
void help(const std::span<const Command> commands);

/// @brief Check if the standard input stream has failed.
/// @return true if the standard input stream has failed, false otherwise.
bool cin_fail();

/// @brief Run a REPL (Read-Eval-Print Loop) with the given commands.
/// @param commands A span of Command objects representing the available
/// commands.
/// @param status An optional string_view representing the status to display in
/// the prompt.
/// @param color An optional string_view representing the color to use for the
/// prompt.
void run(const std::span<const Command> commands,
         const std::string_view &status, const ansi::esc *style = "");

/// @brief Prompt the user for confirmation with a yes/no question.
/// @param text A string_view representing the confirmation question to display.
/// @return true if the user answers "yes", false if the answer is "no". An
/// input error will return false.
[[nodiscard]] bool confirm(const std::string_view &text);
} // namespace repl
