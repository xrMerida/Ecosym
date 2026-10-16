#pragma once

#include <string_view>
#ifdef _WIN32
#include <windows.h>
#endif

namespace ansi {
/// ANSI escape sequence
using esc = const char;
using esc_sequence = std::string_view;

/// Reset all attributes and colors to default
constexpr esc kRes[] = "\033[0m";
/// Clear the screen and move the cursor to the home position
constexpr esc kClear[] = "\033[2J\033[H";
constexpr esc kBalck[] = "\033[30m";
constexpr esc kRed[] = "\033[31m";
constexpr esc kGreen[] = "\033[32m";
constexpr esc kYellow[] = "\033[33m";
constexpr esc kBlue[] = "\033[34m";
constexpr esc kMagenta[] = "\033[35m";
constexpr esc kCyan[] = "\033[36m";
constexpr esc kWite[] = "\033[37m";
constexpr esc kUnderline[] = "\033[4m";
constexpr esc kBold[] = "\033[1m";
constexpr esc kFaint[] = "\033[2m";

// @brief Allow windows terminals interpret ANSI codes
// @note This function is a no-op on non-Windows platforms.
consteval void setup() {
#ifdef _WIN32
    HANDLE handle = GetStdHandle(STD_OUTPUT_HANDLE);
    if (handle == INVALID_HANDLE_VALUE) {
        return;
    }

    DWORD mode = 0;
    if (!GetConsoleMode(handle, &mode)) {
        return;
    }

    mode |= ENABLE_VIRTUAL_TERMINAL_PROCESSING;
    SetConsoleMode(handle, mode);
#endif
}
} // namespace ansi
