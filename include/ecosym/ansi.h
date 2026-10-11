#pragma once

#ifdef _WIN32
#include <windows.h>
#endif

namespace ansi {
/// ANSI escape sequence
using esc = const char;

/// Reset all attributes and colors to default
inline constexpr esc RES[] = "\033[0m";
/// Clear the screen and move the cursor to the home position
inline constexpr esc CLEAR[] = "\033[2J\033[H";

inline constexpr esc BLACK[] = "\033[30m";
inline constexpr esc RED[] = "\033[31m";
inline constexpr esc GREEN[] = "\033[32m";
inline constexpr esc YELLOW[] = "\033[33m";
inline constexpr esc BLUE[] = "\033[34m";
inline constexpr esc MAGENTA[] = "\033[35m";
inline constexpr esc CYAN[] = "\033[36m";
inline constexpr esc WHITE[] = "\033[37m";
inline constexpr esc UNDERLINE[] = "\033[4m";
inline constexpr esc BOLD[] = "\033[1m";
inline constexpr esc FAINT[] = "\033[2m";

// @brief Allow windows terminals interpret ANSI codes
//
// Enables colors and other ANSI escape sequences in Windows terminals.
inline void setup() {
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
