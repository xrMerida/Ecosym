#pragma once

#include "types.h"

namespace ecosym {

class simulation;
class organism {
public:
    organism(char symbol, position location, int energy)
        : symbol_(symbol), location_(location), energy_(energy) {}
    virtual ~organism() = default;

    organism(const organism &) = delete;
    organism &operator=(const organism &) = delete;

    [[nodiscard]] char symbol() const { return symbol_; }
    [[nodiscard]] int energy() const { return energy_; }
    [[nodiscard]] position location() const { return location_; }
    [[nodiscard]] virtual int food_value() const { return energy_; }
    [[nodiscard]] virtual bool dead() const { return energy_ == 0; }

    [[nodiscard]] virtual int max_energy() const = 0;
    [[nodiscard]] virtual int range() const = 0;
    [[nodiscard]] virtual trophic_t trophic() const = 0;
    virtual void act(simulation &world) = 0;

    void set_location(position location) { location_ = location; }
    void add_energy(int amount) {
        energy_ += amount;
        if (energy_ > max_energy()) {
            energy_ = max_energy();
        }
    }
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
