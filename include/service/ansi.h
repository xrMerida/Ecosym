#pragma once

#ifdef _WIN32
#include <windows.h>
#endif

namespace ansi {
/// ANSI escape sequence
using esc = const char;
using esc_sequence = const char *const;

/// Reset all attributes and colors to default
constexpr esc res[] = "\033[0m";
/// Clear the screen and move the cursor to the home position
constexpr esc clear[] = "\033[2J\033[H";
constexpr esc black[] = "\033[30m";
constexpr esc red[] = "\033[31m";
constexpr esc green[] = "\033[32m";
constexpr esc yellow[] = "\033[33m";
constexpr esc blue[] = "\033[34m";
constexpr esc magenta[] = "\033[35m";
constexpr esc cyan[] = "\033[36m";
constexpr esc white[] = "\033[37m";
constexpr esc underline[] = "\033[4m";
constexpr esc bold[] = "\033[1m";
constexpr esc faint[] = "\033[2m";

// @brief Allow windows terminals interpret ANSI codes
// @note This function is a no-op on non-Windows platforms.
constexpr void setup() {
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
