#include "service/repl.h"
#include "service/ansi.h"
#include <cstdlib>
#include <iostream>
#include <limits>
#include <ostream>
#include <stdexcept>
#include <string>
#include <string_view>

constexpr void repl::help(std::span<const command> commands) {
    for (auto &command : commands)
        std::cout << command.cmd << '\t' << command.desc << '\n';
    std::cout << ansi::faint << "help\tPrint this help message\n"
              << "clear\tClear the screen\n"
              << "exit\tExit the current repl\n";
    std::cout << ansi::res << std::endl;
}

bool repl::cin_fail() {
    if (!std::cin.fail()) return false;


    std::cerr << ansi::red << "repl: input error" << ansi::res << std::endl;
    std::cin.clear();
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    if (std::cin.eof()) {
        std::cout << '\n';
        exit(0);
    }
    return true;
}

bool repl::try_read_int(int &result, const int &min, const int &max) {
    result = 0;
    std::string buffer;
    std::getline(std::cin, buffer);

    if (cin_fail() || buffer.empty()) return false;

    try {
        int parsed = std::stoi(buffer);

        if (parsed < min || parsed > max) {
            throw std::out_of_range("out of range");
        }

        result = parsed;
        return true;
    } catch (const std::out_of_range &) {
    } catch (const std::invalid_argument &) {
    }

    std::cerr << ansi::red << "invalid input: number between " << ansi::res
              << ansi::bold << '[' << min << " - " << max << ']' << ansi::res
              << std::endl;

    return false;
}

void repl::run(std::span<const command> commands, std::string_view status,
               ansi::esc_sequence style) {
    std::cout << ansi::faint << "log into " << status << ansi::res << std::endl;
    do {
        std::string buffer;
        std::cout << style << ansi::bold << status << ansi::res << "> ";
        std::getline(std::cin, buffer);

        if (buffer == "exit") break;
        if (buffer.empty()) continue;
        if (buffer == "help") {
            help(commands);
            continue;
        }
        if (buffer == "clear") {
            std::cout << ansi::clear << std::flush;
            continue;
        }

        function_t action = no_op;
        for (auto &command : commands) {
            if (buffer != command.cmd) continue;

            action = command.action;
        }

        if (action != no_op) {
            action();
            continue;
        }

        std::cerr << ansi::red << "repl: " << buffer << ": command not found"
                  << ansi::res << std::endl;
    } while (!cin_fail());

    std::cout << ansi::faint << "logout" << ansi::res << std::endl;
}
