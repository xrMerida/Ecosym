#include "repl.h"
#include "ansi.h"
#include <cstdint>
#include <iostream>
#include <limits>
#include <ostream>
#include <string_view>

void repl::help(const std::span<const command> commands) {
    for (const auto &command : commands)
        std::cout << command.str << '\t' << command.desc << '\n';
    std::cout << std::endl;
}

bool repl::cin_fail() {
    if (!std::cin.fail()) return false;

    std::cerr << ansi::kRed << "repl: input error" << ansi::kRes << std::endl;
    std::cin.clear();
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    return true;
}

void repl::run(const std::span<const repl::command> commands,
               const std::string_view &status, const ansi::esc_view &style) {
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
        std::string input;
        std::cout << style << ansi::kBold << status << ansi::kRes;
        for (uint8_t i = 0; i < nest_lv; i++) std::cout << '>';
        std::cout << ' ';
        std::getline(std::cin, input);

        if (input == "exit") break;
        if (input.empty()) continue;
        if (input == "help") {
            help(commands);
            continue;
        }
        if (input == "clear") {
            std::cout << ansi::kClear << std::flush;
            continue;
        }

        bool found = false;
        for (const auto &command : commands) {
            found = (input == command.str);
            if (found) {
                command.action();
                break;
            }
        }

        if (!found) {
            std::cerr << ansi::kRed << status << ": " << input
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

bool repl::confirm(const std::string_view &text) {
    std::string input;
    do {
        std::cout << ansi::kYellow << text << " [Y/N]: " << ansi::kRes;
        std::getline(std::cin, input);

        switch (tolower(input[0])) {
        case 'y':
            return true;
        case 'n':
            return false;
        }
    } while (!cin_fail());

    return false;
}
