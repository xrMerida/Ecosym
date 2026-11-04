#pragma once

#include "types.h"

namespace ecosym {

class simulation;
/// @brief Base class for all organisms in a simulation.
class organism {
public:
    /// @brief Creates an organism.
    /// @param symbol The character used to draw the organism.
    /// @param location The organism's initial location.
    /// @param energy The organism's initial energy.
    organism(char symbol, position location, int energy)
        : symbol_(symbol), location_(location), energy_(energy) {}
    virtual ~organism() = default;

    organism(const organism &) = delete;
    organism &operator=(const organism &) = delete;

    [[nodiscard]] char symbol() const { return symbol_; }
    [[nodiscard]] int energy() const { return energy_; }
    [[nodiscard]] position location() const { return location_; }
    /// @brief Gets the amount of energy provided when consumed.
    /// @return The food value.
    [[nodiscard]] virtual int food_value() const { return energy_; }
    /// @brief Checks weather the organism is dead or not.
    /// @return `true` when the organism is dead.
    [[nodiscard]] virtual bool dead() const { return energy_ == 0; }

    [[nodiscard]] virtual int max_energy() const = 0;
    /// @brief Gets the organism's range of action.
    /// @note It represents the maximun distance in map cells including
    /// diagonal neighbors.
    [[nodiscard]] virtual int range() const = 0;
    [[nodiscard]] virtual trophic_t trophic() const = 0;
    /// @brief Performs the organism's action for one turn.
    /// @param world The simulation containing the organism.
    virtual void act(simulation &world) = 0;


    void set_location(position location) { location_ = location; }
    /// @brief Adds energy up to the organism's maximum.
    /// @param amount The amount of energy to add.
    /// @note The energy is capped at the a maximum.
    void add_energy(int amount) {
        energy_ += amount;
        if (energy_ > max_energy()) {
            energy_ = max_energy();
        }
    }

    /// @brief Consumes energy from the organism.
    /// @param amount The amount of energy to consume.
    /// @note The energy is capped at zero.
    void consume_energy(int amount) {
        if (amount > energy_) {
            energy_ = 0;
        } else {
            energy_ -= amount;
        }
    }

private:
    const char symbol_;
    position location_;
    int energy_;
};

} // namespace ecosym
