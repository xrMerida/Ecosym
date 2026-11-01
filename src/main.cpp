#include "ecosym/organism.h"
#include "ecosym/simulation.h"
#include "service/ansi.h"
#include "service/repl.h"
#include "types.h"
#include <cstddef>
#include <cstdlib>
#include <ctime>
#include <iostream>
#include <iterator>
#include <string_view>

using namespace ecosym;
using function_t = repl::function_t;
using command = repl::command;

struct factory {
    const char *name;
    char symbol;
    trophic_t type;
};

struct stats {
    int turn;
    size_t plants;
    size_t herbivores;
    size_t carnivores;
    size_t total() { return plants + herbivores + carnivores; }
};

simulation *world = nullptr;
stats initial_stats{};

inline void print_logs() {
    world->logs().print_logs(std::cout);
}

inline std::ostream &operator<<(std::ostream &output, stats stat) {
    return output << ansi::green << "plants: " << ansi::res << stat.plants
                  << '\n'
                  << ansi::yellow << "herbivores: " << ansi::res
                  << stat.herbivores << '\n'
                  << ansi::magenta << "carnivores: " << ansi::res
                  << stat.carnivores << '\n'
                  << ansi::bold << "total: " << stat.total() << ansi::res
                  << '\n';
}

[[nodiscard]] stats get_stats() {
    return {world->turn(), world->count(trophic_t::plant),
            world->count(trophic_t::herbivore),
            world->count(trophic_t::carnivore)};
}

void exit_cmd() {
    world->logs().print_history(std::cout);
    std::cout << ansi::bold << "--- Current statistics ---\n";
    std::cout << ansi::res << get_stats() << '\n';
    std::cout << ansi::bold << "--- Initial statistics ---\n";
    std::cout << ansi::res << initial_stats << '\n';
    std::cout << ansi::bold << "== Simulation terminated ==\n" << ansi::res;
}

inline int random_value(int max) {
    return std::rand() % max;
}

int read_int(std::string_view name, int min, int max) {
    int result;
    do {
        std::cout << name << ansi::res << ansi::faint << " [" << min << " - "
                  << max << ']' << ansi::res << ": ";
    } while (!repl::try_read_int(result, min, max));
    return result;
}

void lookup_cmd() {
    int column = read_int("x-axis", 0, world->columns() - 1);
    int row = read_int("y-axis", 0, world->rows() - 1);
    const organism *current = world->lookup({row, column});

    if (current == nullptr) {
        std::cout << "empty\n";
        return;
    }

    std::cout << "symbol: " << current->symbol() << '\n'
              << "type: " << current->trophic() << '\n'
              << "energy: " << current->energy() << '\n'
              << "location: (" << column << ", " << row << ")\n";
}

void reset_cmd() {
    if (world != nullptr) {
        exit_cmd();
        delete world;
    }

    constexpr int min_length = 12;
    constexpr int max_length = 25;

    int columns = read_int("matrix x-axis", min_length, max_length);
    int rows = read_int("matrix y-axis", min_length, max_length);
    world = new simulation(rows, columns, random_value);

    constexpr factory factories[] = {
        {"plants", '*', trophic_t::plant},
        {"rabbits", 'R', trophic_t::herbivore},
        {"sheep", 'S', trophic_t::herbivore},
        {"wolves", 'W', trophic_t::carnivore},
    };

    int capacity = (rows * columns) / 2;
    for (int i = std::size(factories) - 1; i >= 0; i--) {
        auto &fct = factories[i];
        constexpr int min = 5;
        int count = read_int(fct.name, min, capacity - (i * min));
        capacity -= count;
        for (; count > 0; count--) {
            position location = world->random_empty();
            if (location == no_position) break;
            world->spawn(fct.type, fct.symbol, location);
        }
    }
    print_logs();
    initial_stats.plants = world->count(trophic_t::plant);
    initial_stats.herbivores = world->count(trophic_t::herbivore);
    initial_stats.carnivores = world->count(trophic_t::carnivore);
}

int main() {
    ansi::setup();
    std::srand(static_cast<unsigned>(std::time(nullptr)));

    constexpr function_t step_cmd = [] {
        world->step();
        print_logs();
    };
    constexpr function_t draw_cmd = [] { world->draw(std::cout); };
    constexpr function_t stats_cmd = [] { std::cout << get_stats(); };

    constexpr command commands[] = {
        {"reset", "Reset the simulation", reset_cmd},
        {"draw", "Draw the current world", draw_cmd},
        {"lookup", "Inspect an organism at a position", lookup_cmd},
        {"step", "Advance the simulation one turn", step_cmd},
        {"stats", "Show the current statistics of the world", stats_cmd},
    };

    reset_cmd();
    repl::run(commands, "ecosym", ansi::cyan);
    exit_cmd();
    delete world;
    return 0;
}
