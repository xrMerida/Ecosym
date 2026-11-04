#pragma once

#include "ecosym/animal.h"

namespace ecosym {

/// @brief An animal that feeds on herbivores.
class carnivore : public animal {
public:
    /// @brief Creates a carnivore.
    /// @param symbol The character used to draw the carnivore.
    /// @param location The carnivore's initial location.
    explicit carnivore(char symbol, position location)
        : animal(symbol, location, 10) {}

    [[nodiscard]] int max_energy() const override { return 15; }
    [[nodiscard]] int range() const override { return 3; }
    [[nodiscard]] trophic_t trophic() const override {
        return trophic_t::carnivore;
    }

private:
    trophic_t prey_type() const override { return trophic_t::herbivore; }
    int upkeep() const override { return 3; }
};

} // namespace ecosym
