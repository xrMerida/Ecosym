#pragma once

#include "ecosym/organism.h"
#include "ecosym/simulation.h"

namespace ecosym {

/// @brief Base class for mobile, feeding organisms.
class animal : public organism {
public:
    /// @brief Creates an animal.
    /// @param symbol The character used to draw the animal.
    /// @param location The animal's initial location.
    /// @param energy The animal's initial energy.
    animal(char symbol, position location, int energy)
        : organism(symbol, location, energy) {}

    /// @brief Performs the animal's movement and feeding action.
    /// @param world The simulation containing the animal.
    void act(simulation &world) override;

protected:
    /// @brief Gets the trophic type this animal can consume.
    virtual trophic_t prey_type() const = 0;
    /// @brief Gets the energy cost of one turn.
    virtual int upkeep() const = 0;
};

} // namespace ecosym
