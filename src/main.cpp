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
    size_t plants;
    size_t herbivores;
    size_t carnivores;
};

simulation *world = nullptr;
stats initial_stats{};

inline void print_logs() {
    world->logs().print_logs(std::cout);
}

void stats_cmd() {
    std::cout << "plants: " << world->count(trophic_t::plant) << '\n'
              << "herbivores: " << world->count(trophic_t::herbivore) << '\n'
              << "carnivores: " << world->count(trophic_t::carnivore) << '\n';
}

void exit_cmd() {
    std::cout << "== Simulation terminated ==\n";
    world->logs().print_history(std::cout);
    std::cout << "--- Current statistics ---\n";
    stats_cmd();
    std::cout << "--- Initial statistics ---\n"
              << "plants: " << initial_stats.plants << '\n'
              << "herbivores: " << initial_stats.herbivores << '\n'
              << "carnivores: " << initial_stats.carnivores << '\n';
}

inline int random_value(int max) {
    return std::rand() % max;
}

int read_number(std::string_view name, int min, int max) {
    int result;
    do {
        std::cout << name << " [" << min << " - " << max << "]: ";
    } while (!repl::try_read_int(result, min, max));
    return result;
}

void lookup_cmd() {
    int row = read_number("row", 0, world->rows() - 1);
    int column = read_number("column", 0, world->columns() - 1);
    const organism *current = world->lookup({row, column});

    if (current == nullptr) {
        std::cout << "empty\n";
        return;
    }

    std::cout << "symbol: " << current->symbol() << '\n'
              << "type: " << current->trophic() << '\n'
              << "energy: " << current->energy() << '\n'
              << "location: (" << row << ", " << column << ")\n";
}

void reset_cmd() {
    if (world != nullptr) {
        stats_cmd();
        exit_cmd();
        delete world;
    }

    constexpr int min_length = 12;
    constexpr int max_length = 25;

    int rows = read_number("matrix rows", min_length, max_length);
    int columns = read_number("matrix columns", min_length, max_length);
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
        int count = read_number(fct.name, 1, capacity - i);
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
    std::srand(static_cast<unsigned>(std::time(nullptr)));

    constexpr function_t step_cmd = [] {
        world->step();
        print_logs();
    };
    constexpr function_t draw_cmd = [] { world->draw(std::cout); };

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
