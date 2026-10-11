#include "repl.h"
#include "ansi.h"
#include <iostream>
#include <limits>
#include <ostream>
#include <string_view>

void repl::help(const std::span<const Command> commands) {
    for (const auto &command : commands)
        std::cout << command.str << '\t' << command.desc << '\n';
    std::cout << std::endl;
}

bool repl::cin_fail() {
    if (!std::cin.fail()) return false;

    std::cerr << ansi::RED << "repl: input error" << ansi::RES << std::endl;
    std::cin.clear();
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    return true;
}

void repl::run(const std::span<const repl::Command> commands,
               const std::string_view &status, const ansi::esc *style) {
    std::string input;
    std::cout << ansi::FAINT << "logging into " << status << ansi::RES
              << std::endl;
    do {
        std::cout << style << ansi::BOLD << status << ansi::RES << "> "
                  << std::flush;
        std::getline(std::cin, input);

        if (input.empty()) continue;

        bool found = false;
        for (const auto &command : commands) {
            found = input == command.str;
            if (found) {
                command.action();
                break;
            }
        }

        if (found) continue;
        if (input == "exit") break;

        if (input == "help") {
            help(commands);
        } else if (input == "clear" || input == "cls") {
            std::cout << ansi::CLEAR << std::flush;
        } else {
            std::cerr << ansi::RED << status << ": " << input
                      << ": command not found" << ansi::RES << std::endl;
        }
    } while (!cin_fail());
    std::cout << ansi::FAINT << "logging out" << ansi::RES << std::endl;
}

bool repl::confirm(const std::string_view &text) {
    std::string input;
    do {
        std::cout << ansi::YELLOW << text << " [Y/N]: " << ansi::RES
                  << std::flush;
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
