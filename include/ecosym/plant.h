#pragma once

#include "ecosym/organism.h"
#include "ecosym/simulation.h"

namespace ecosym {

/// @brief A stationary organism that can spread to nearby cells.
class plant : public organism {
public:
    /// @brief Creates a plant.
    /// @param symbol The character used to draw the plant.
    /// @param location The plant's initial location.
    explicit plant(char symbol, position location)
        : organism(symbol, location, 0) {}

    /// @brief Performs the plant's spreading action.
    /// @param world The simulation containing the plant.
    void act(simulation &world) override;

    [[nodiscard]] int max_energy() const override { return 4; }
    /// @brief Gets the plant's food value.
    /// @return The energy provided to an animal that consumes the plant.
    [[nodiscard]] int food_value() const override { return max_energy(); }
    /// @brief Returns whether the plant is dead
    /// @return Always `false`.
    /// @note Plants cannot die unless eaten.
    [[nodiscard]] bool dead() const override { return false; }
    /// @brief Gets the plant's spreading range.
    [[nodiscard]] int range() const override { return 1; }
    /// @brief Gets the plant's trophic type.
    /// @return The plant trophic type.
    [[nodiscard]] trophic_t trophic() const override {
        return trophic_t::plant;
    }

private:
    void spread(simulation &world);
};

} // namespace ecosym
