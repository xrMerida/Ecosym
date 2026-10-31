#pragma once

#include "ecosym/animal.h"

namespace ecosym {

class herbivore : public animal {
public:
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
