#pragma once

#include "ecosym/animal.h"

namespace ecosym {

/// @brief An animal that feeds on plants.
class herbivore : public animal {
public:
    /// @brief Creates a herbivore.
    /// @param symbol The character used to draw the herbivore.
    /// @param location The herbivore's initial location.
    herbivore(char symbol, position location) : animal(symbol, location, 5) {}

    [[nodiscard]] int max_energy() const override { return 10; }
    [[nodiscard]] int range() const override { return 2; }
    [[nodiscard]] trophic_t trophic() const override {
        return trophic_t::herbivore;
    }

private:
    trophic_t prey_type() const override { return trophic_t::plant; }
    int upkeep() const override { return 1; }
};

} // namespace ecosym
