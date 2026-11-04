#pragma once

#include "data/list.h"
#include "ecosym/logger.h"
#include "ecosym/map.h"
#include "ecosym/organism.h"
#include "types.h"
#include <ostream>

namespace ecosym {

/// @brief Coordinates the map, organisms, turns, and event logging.
class simulation {
public:
    /// @brief Function type used to generate a random value below a limit.
    using randomizer = int (*)(int);

    simulation(const simulation &) = delete;
    simulation(simulation &&) = delete;
    simulation &operator=(const simulation &) = delete;
    simulation &operator=(simulation &&) = delete;

    /// @brief Creates a simulation with an empty map.
    /// @param rows The number of map rows.
    /// @param columns The number of map columns.
    /// @param random The random-value generator used by the simulation.
    simulation(int rows, int columns, randomizer random);
    ~simulation();

    /// @brief Gets the number of organisms in the simulation.
    [[nodiscard]] size_t organism_count() const;
    /// @brief Gets the map's height.
    [[nodiscard]] int rows() const;
    /// @brief Gets the map's width.
    [[nodiscard]] int columns() const;
    /// @brief Counts organisms of a trophic type.
    /// @param type The trophic type to count.
    /// @return The number of matching organisms.
    [[nodiscard]] size_t count(trophic_t type) const;
    /// @brief Gets a read-write reference to the simulation's event logger.
    [[nodiscard]] logger &logs();
    /// @brief Gets the current turn.
    [[nodiscard]] int turn() const;
    /// @brief Finds the organism at a location.
    /// @param location The location to inspect.
    /// @return a read-only pointer to the organism, or `nullptr` when the
    /// location is empty.
    [[nodiscard]] const organism *lookup(position location) const;
    /// @brief Checks whether a location contains no organism.
    /// @param location The location to inspect.
    /// @return `true` when the location is empty.
    [[nodiscard]] bool empty(position location) const;
    /// @brief Finds a random empty location within a range.
    /// @param center The center of the search area.
    /// @param range The maximum search range.
    /// @return An empty location, or `no_position` when none is available.
    [[nodiscard]] position random_empty(position center, int range) const;
    /// @brief Finds a random empty location anywhere on the map.
    /// @return An empty location, or `no_position` when none is available.
    [[nodiscard]] position random_empty() const;
    /// @brief Checks whether a location is within the map.
    /// @param location The location to check.
    /// @return `true` when the location is within the map's bounds.
    [[nodiscard]] bool in_bounds(position location) const;
    /// @brief Creates and places an organism.
    /// @param type The trophic type to create.
    /// @param symbol The character used to draw the organism.
    /// @param location The organism's location.
    void spawn(trophic_t type, char symbol, position location);
    /// @brief Removes a matching prey organism and transfers its food value.
    /// @param hunter The organism attempting to consume prey.
    /// @param prey_type The trophic type of prey to consume.
    /// @return `true` when prey was consumed.
    bool kill(organism *hunter, trophic_t prey_type);
    /// @brief Moves an organism to a random in-range empty location.
    /// @param who The organism to move.
    /// @return `true` when the organism moved.
    bool move_random(organism *who);
    /// @brief Moves an organism toward nearby prey.
    /// @param who The organism to move.
    /// @param prey_type The trophic type of prey to approach.
    /// @return `true` when the organism moved.
    bool stalk(organism *who, trophic_t prey_type);
    /// @brief Advances the simulation by one turn.
    void step();
    /// @brief Draws the current map state.
    /// @param output The stream receiving the map.
    void draw(std::ostream &output);


private:
    map map_;
    data::list<organism *> organisms_;
    logger logger_;
    randomizer random_;
    int turn_ = 0;

    static int distance(position first, position second);
    bool move(organism *who, position destination);
    void remove(organism *target);
    [[nodiscard]] organism *find(trophic_t type, position center, int range);
    [[nodiscard]] organism *find(trophic_t type, position center);
    [[nodiscard]] bool owns(const organism *candidate) const;

    void remove_dead();
};

} // namespace ecosym
