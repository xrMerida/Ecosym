#pragma once

#include <string_view>
#ifdef _WIN32
#include <windows.h>
#endif

namespace ansi {
/// ANSI escape sequence
using esc = const char;
using esc_view = std::string_view;

/// Reset all attributes and colors to default
inline constexpr esc kRes[] = "\033[0m";
/// Clear the screen and move the cursor to the home position
inline constexpr esc kClear[] = "\033[2J\033[H";
inline constexpr esc kBalck[] = "\033[30m";
inline constexpr esc kRed[] = "\033[31m";
inline constexpr esc kGreen[] = "\033[32m";
inline constexpr esc kYellow[] = "\033[33m";
inline constexpr esc kBlue[] = "\033[34m";
inline constexpr esc kMagenta[] = "\033[35m";
inline constexpr esc kCyan[] = "\033[36m";
inline constexpr esc kWite[] = "\033[37m";
inline constexpr esc kUnderline[] = "\033[4m";
inline constexpr esc kBold[] = "\033[1m";
inline constexpr esc kFaint[] = "\033[2m";

// @brief Allow windows terminals interpret ANSI codes
// @note This function is a no-op on non-Windows platforms.
void setup();
} // namespace ansi
