#include "repl.h"
#include "ansi.h"
#include <cstdint>
#include <iostream>
#include <limits>
#include <ostream>
#include <string_view>

constexpr void repl::help(std::span<const command> commands) {
    for (auto &command : commands)
        std::cout << command.cmd << '\t' << command.desc << '\n';
    std::cout << ansi::kFaint << "help\tPrint this help message\n"
              << "clear\tClear the screen\n"
              << "exit\tExit the current repl\n";
    std::cout << ansi::kRes << std::endl;
}

bool repl::cin_fail() {
    if (!std::cin.fail()) return false;

    std::cerr << ansi::kRed << "repl: input error" << ansi::kRes << std::endl;
    std::cin.clear();
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    return true;
}

void repl::run(std::span<const command> commands, std::string_view status,
               ansi::esc_sequence style) {
    static uint8_t nest_lv = 0;
    nest_lv++;
    if (nest_lv > 1) {
        std::cout << ansi::kFaint << "nested into " << status << ansi::kRes
                  << std::endl;
    } else {
        std::cout << ansi::kFaint << "log into " << status << ansi::kRes
                  << std::endl;
    }
    do {
        std::string buffer;
        std::cout << style << ansi::kBold << status << ansi::kRes;
        for (uint8_t i = 0; i < nest_lv; i++) std::cout << '>';
        std::cout << ' ';
        std::getline(std::cin, buffer);

        if (buffer == "exit") break;
        if (buffer.empty()) continue;
        if (buffer == "help") {
            help(commands);
            continue;
        }
        if (buffer == "clear") {
            std::cout << ansi::kClear << std::flush;
            continue;
        }

        bool found = false;
        for (auto &command : commands) {
            found = (buffer == command.cmd);
            if (!found) continue;

            command.action();
            break;
        }

        if (!found) {
            std::cerr << ansi::kRed << status << ": " << buffer
                      << ": command not found" << ansi::kRes << std::endl;
        }
    } while (!cin_fail());

    nest_lv--;
    if (nest_lv == 0) {
        std::cout << ansi::kFaint << "logout" << ansi::kRes << std::endl;
    } else {
        std::cout << ansi::kFaint << "unnested" << ansi::kRes << std::endl;
    }
}

bool repl::confirm(std::string_view text) {
    std::string buffer;
    do {
        std::cout << ansi::kYellow << text << " [Y/N]: " << ansi::kRes;
        std::getline(std::cin, buffer);

        switch (tolower(buffer[0])) {
        case 'y':
            return true;
        case 'n':
            return false;
        }
    } while (!cin_fail());

    return false;
}
